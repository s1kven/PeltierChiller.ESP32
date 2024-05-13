#pragma once

#include "ChillerService.h"

Services::ChillerService::ChillerService(uint8_t _temperatureSensorsPin, uint8_t _tSensorsCount,
	Models::TemperatureSensors::BaseSensor* _tSensors[], Communication::Models::ChillerConfiguration* chillerConfiguration,
	uint8_t powerButtonPin, uint8_t chillerPsSignalPin, uint8_t chillerSignalPin, uint8_t powerSignalPin)
{
	_temperatureService = new Services::TemperatureService(_temperatureSensorsPin, _tSensorsCount, _tSensors);
	_state = Models::Enums::ChillerState::off;
	_chillerConfiguration = chillerConfiguration;

	_powerButtonPin = powerButtonPin;
	_chillerPsSignalPin = chillerPsSignalPin;
	_chillerSignalPin = chillerSignalPin;
	_powerSignalPin = powerSignalPin;

	_powerButton = new Models::Button(_powerButtonPin, "POWER");
	_powerButton->setLastPressTime(0);
	initConfiguration();
}

void Services::ChillerService::manageChiller(float pcVoltage)
{
	handlePowerButton();
	manageChillerLoad();
	(*_temperatureService).requestSensors(_sensorsRequestDelay);
	handleChillerState(pcVoltage);
}

void Services::ChillerService::initConfiguration()
{
	_targetTemperature = _chillerConfiguration->getTargetCircuitTemperature();
}

void Services::ChillerService::handlePowerButton()
{
	_powerButton->setState(!digitalRead(_powerButton->getSignalPin()));
	if (_powerButton->getState() && !_powerButton->getFlag())
	{
		_powerButton->setFlag(true);
		_powerButton->setLastPressTime(millis());
	}
	else if (!_powerButton->getState() && _powerButton->getFlag() && millis() - _powerButton->getLastPressTime() > 20)
	{
		_powerButton->setFlag(false);
		_powerButton->setLastPressMillis(millis() - _powerButton->getLastPressTime());
	}
}

void Services::ChillerService::manageChillerLoad()
{
	_varResistorValue = 150;
	if (millis() - _chillerLoadTimer >= 5000)
	{
		float coldT = (*_temperatureService).getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::coldCircuit);

		_varResistorValue = computePID(coldT, _targetTemperature,
			-50, -0.1, -100.0, 20) + 20;
		_chillerLoadTimer = millis();

		if (_varResistorValue > 150)
		{
			_varResistorValue = 150;
		}
		else if (_varResistorValue < 50)
		{
			_varResistorValue = 50;
		}
		Wire.beginTransmission(0x2E);
		Wire.write(byte(0x00));
		Wire.write(_varResistorValue);
		Wire.endTransmission();
	}
}

int Services::ChillerService::computePID(float _currentT, float _targetT, float _kp, float _ki, float _kd, float _dt)
{
	float errT = _targetT - _currentT;
	static float integral = 0;
	static float prevErr = 0;
	integral += errT * _dt;
	if (integral > 1000.0)
	{
		integral = 1000.0;
	}
	else if (integral < -1000.0)
	{
		integral = -1000.0;
	}
	float D = (errT - prevErr) / _dt;
	prevErr = errT;
	return (errT * _kp + integral * _ki + D * _kd);
}

void Services::ChillerService::handleChillerState(float pcVoltage)
{
	if (pcVoltage > 250 && _state != Models::Enums::ChillerState::enabling)
	{
		_state = Models::Enums::ChillerState::temperatureMaintaining;
	}

	switch (_state)
	{
	case Models::Enums::ChillerState::off:
		if (_powerButton->getLastPressMillis() > 0)
		{
			_state = Models::Enums::ChillerState::enabling;
		}
		else
		{
			digitalWrite(_chillerPsSignalPin, LOW);
			digitalWrite(_chillerSignalPin, LOW);
		}
		break;

	case Models::Enums::ChillerState::enabling:

		digitalWrite(_chillerPsSignalPin, HIGH);
		digitalWrite(_chillerSignalPin, HIGH);
		if (_powerButton->getLastPressMillis() > 0 &&
			_temperatureService->getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::coldCircuit) <=
			_targetTemperature)
		{
			digitalWrite(_powerSignalPin, HIGH);
			if (_powerSignalTimer == _oldPowerSignalTimer)
			{
				_powerSignalTimer = millis();
			}
			if (_powerSignalFlag)
			{
				digitalWrite(_powerSignalPin, LOW);
				_powerSignalTimer = millis();
				_oldPowerSignalTimer = _powerSignalTimer;
				_powerButton->setLastPressMillis(0);
				_powerSignalFlag = false;
				_state = Models::Enums::ChillerState::temperatureMaintaining;
			}
			else if (millis() - _powerButton->getLastPressMillis() >= _powerSignalTimer)
			{
				_powerSignalFlag = true;
			}
		}
		break;

	case Models::Enums::ChillerState::temperatureMaintaining:

		digitalWrite(_chillerPsSignalPin, HIGH);
		digitalWrite(_chillerSignalPin, HIGH);
		if (pcVoltage < 250)
		{
			_state = Models::Enums::ChillerState::off;
		}
		/*if (_powerButton->getLastPressMillis() != 0)
		{
			Serial.println("temperatureMaintaining high");
			digitalWrite(_powerSignalPin, HIGH);
			(*_chillerService).SetChillerState(Models::Enums::ChillerState::disabling);
		}*/
		break;
	case Models::Enums::ChillerState::disabling:

		//Serial.println("disabling");
		//digitalWrite(_chillerPsSignalPin, HIGH);
		//digitalWrite(_chillerSignalPin, HIGH);
		if (!_powerButton->getFlag() && millis() - _powerButton->getLastTime() < _powerButton->getLastPressMillis() &&
			_powerButton->getLastPressMillis() != 0)
		{
			//Serial.println("disabling high");
			digitalWrite(_powerSignalPin, HIGH);
		}
		else
		{
			//Serial.println("disabling low");
			digitalWrite(_powerSignalPin, LOW);
			_state = Models::Enums::ChillerState::off;
		}
		break;

	default:
		break;
	}
}