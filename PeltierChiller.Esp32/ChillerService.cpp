#pragma once

#include "ChillerService.h"

Services::ChillerService::ChillerService(Communication::Models::Configurations::Configuration* configuration)
{
	_state = Models::Enums::ChillerState::off;
	_configuration = configuration;

	initConfiguration();

	_varResistorValue = _maxPotentiometerValue;

	pinMode(_powerButtonPin, INPUT_PULLUP);
	pinMode(_chillerPsSignalPin, OUTPUT);
	pinMode(_chillerSignalPin, OUTPUT);
	pinMode(_powerSignalPin, OUTPUT);

	_temperatureService = new Services::TemperatureService(_configuration->getTemperatureSensorsConfiguration());

	_powerButton = new Models::Button(_powerButtonPin, "POWER");
	_powerButton->setLastPressTime(0);
}

void Services::ChillerService::manageChiller()
{
	handlePcVoltage();
	handlePowerButton();
	manageChillerLoad();
	(*_temperatureService).requestSensors(_sensorsRequestDelay);
	handleChillerState(_vin);
}

Services::TemperatureService* Services::ChillerService::getTemperatureService()
{
	return _temperatureService;
}

void Services::ChillerService::initConfiguration()
{
	_chillerType = _configuration->getChillerType();
	_setTemperature = _configuration->getTargetCircuitTemperature();
	if (_chillerType == ChillerType::fixedTemperature)
	{
		_targetTemperature = _setTemperature;
	}

	Communication::Models::Configurations::PinsConfiguration* pinsConfiguration = _configuration->getPinsConfiguration();
	_powerButtonPin = pinsConfiguration->getPowerButton();
	_chillerPsSignalPin = pinsConfiguration->getChillerPsSignal();
	_chillerSignalPin = pinsConfiguration->getChillerSignal();
	_powerSignalPin = pinsConfiguration->getPowerSignal();
	_pcVoltagePin = pinsConfiguration->getPcVoltage();

	_voltmeterThreshold = _configuration->getVoltmeterThreshold();
	_adc.attach(_pcVoltagePin);
	_voltmeterR1 = _configuration->getVoltmeterR1();
	_voltmeterR2 = _configuration->getVoltmeterR2();
	_isDelayEnablingPc = _configuration->getIsDelayEnablingPc();

	Communication::Models::Configurations::TimersConfiguration* timersConfiguration = _configuration->getTimersConfiguration();
	_sensorsRequestDelay = timersConfiguration->getTemperatureSensorsRequestDelay();
	_buttonMinPressTime = timersConfiguration->getButtonMinPressTime();

	Communication::Models::Configurations::ChillerConfiguration* chillerConfiguration = _configuration->getChillerConfiguration();
	_computePidDelay = chillerConfiguration->getComputePidDelay();
	_potentiometerAddress = chillerConfiguration->getPotentiometerAddress();
	_maxPotentiometerValue = chillerConfiguration->getMaxPotentiometerValue();
	_minPotentiometerValue = chillerConfiguration->getMinPotentiometerValue();
	_kp = chillerConfiguration->getKp();
	_ki = chillerConfiguration->getKi();
	_kd = chillerConfiguration->getKd();
	_dt = chillerConfiguration->getDt();
	_pidRatio = chillerConfiguration->getPidRatio();
	_minIntegral = chillerConfiguration->getMinIntegral();
	_maxIntegral = chillerConfiguration->getMaxIntegral();
}

void Services::ChillerService::handlePcVoltage()
{
	_vin = _adc.readVoltage() / (float(_voltmeterR2) / (float(_voltmeterR1) + float(_voltmeterR2)));
}

void Services::ChillerService::handlePowerButton()
{
	_powerButton->setState(!digitalRead(_powerButton->getSignalPin()));
	if (_powerButton->getState() && !_powerButton->getFlag())
	{
		_powerButton->setFlag(true);
		_powerButton->setLastPressTime(millis());
	}
	else if (!_powerButton->getState() && _powerButton->getFlag() && millis() - _powerButton->getLastPressTime() > _buttonMinPressTime)
	{
		_powerButton->setFlag(false);
		_powerButton->setLastPressMillis(millis() - _powerButton->getLastPressTime());
	}
}

void Services::ChillerService::setPotentiometerValue(uint16_t address, uint16_t value)
{
	Wire.beginTransmission(address);
	Wire.write(byte(0x00));
	Wire.write(value);
	Wire.endTransmission();
}

void Services::ChillerService::manageChillerLoad()
{
	if (millis() - _chillerLoadTimer >= _computePidDelay)
	{
		if (_state == Models::Enums::ChillerState::enabling)
		{
			setPotentiometerValue(_potentiometerAddress, _maxPotentiometerValue);
		}
		else
		{
			float coldT = (*_temperatureService).getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::coldCircuit);

			if (_chillerType == ChillerType::deltaTemperature)
			{
				_targetTemperature = (*_temperatureService)
					.getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::room) + _setTemperature;
			}
			else if (_chillerType == ChillerType::dewPointTemperature)
			{
				_targetTemperature = (*_temperatureService)
					.getDewPointTemperature(Models::Enums::TemperatureSensorTarget::room) + _setTemperature;
			}
			_varResistorValue = computePID(coldT, _targetTemperature,
				_kp, _ki, _kd, _dt) + _pidRatio;
			_chillerLoadTimer = millis();

			if (_varResistorValue > _maxPotentiometerValue)
			{
				_varResistorValue = _maxPotentiometerValue;
			}
			else if (_varResistorValue < _minPotentiometerValue)
			{
				_varResistorValue = _minPotentiometerValue;
			}
			setPotentiometerValue(_potentiometerAddress, _varResistorValue);
		}
	}
}

int Services::ChillerService::computePID(float _currentT, float _targetT, float _kp, float _ki, float _kd, float _dt)
{
	float errT = _targetT - _currentT;
	static float integral = 0;
	static float prevErr = 0;
	integral += errT * _dt;
	if (integral > _maxIntegral)
	{
		integral = _maxIntegral;
	}
	else if (integral < _minIntegral)
	{
		integral = _minIntegral;
	}
	float D = (errT - prevErr) / _dt;
	prevErr = errT;
	return (errT * _kp + integral * _ki + D * _kd);
}

void Services::ChillerService::handleChillerState(float pcVoltage)
{
	if (pcVoltage < _voltmeterThreshold && _state != Models::Enums::ChillerState::enabling)
	{
		_state = Models::Enums::ChillerState::off;
	}
	else if (pcVoltage > _voltmeterThreshold &&
		_state != Models::Enums::ChillerState::enabling && _state != Models::Enums::ChillerState::disabling)
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
			(_temperatureService->getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::coldCircuit) <=
			_targetTemperature || !_isDelayEnablingPc))
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
		if (_powerButton->getLastPressMillis() != 0)
		{
			_state = Models::Enums::ChillerState::disabling;
		}
		break;
	case Models::Enums::ChillerState::disabling:

		if (_powerButton->getLastPressMillis() > 0)
		{
			digitalWrite(_powerSignalPin, HIGH);
			if (_powerSignalTimer == _oldPowerSignalTimer)
			{
				_powerSignalTimer = millis();
			}
			if (_powerSignalFlag)
			{
				_powerSignalTimer = millis();
				_oldPowerSignalTimer = _powerSignalTimer;
				_powerButton->setLastPressMillis(0);
				_powerSignalFlag = false;
			}
			else if (millis() - _powerButton->getLastPressMillis() >= _powerSignalTimer)
			{
				_powerSignalFlag = true;
			}
		}
		else
		{
			digitalWrite(_powerSignalPin, LOW);
		}
		break;

	default:
		break;
	}
}