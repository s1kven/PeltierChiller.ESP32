#pragma once

#include "ChillerService.h"

Services::ChillerService::ChillerService(uint8_t _temperatureSensorsPin, uint8_t _tSensorsCount, Models::TemperatureSensors::BaseSensor* _tSensors[],
	float _targetCircuitTemperature, Models::Enums::ChillerState state)
{
	_temperatureService = new Services::TemperatureService(_temperatureSensorsPin, _tSensorsCount, _tSensors);
	_targetTemperature = _targetCircuitTemperature;
	_state = state;
}

void Services::ChillerService::execute()
{
	handleChillerState();
	(*_temperatureService).requestSensors(_sensorsRequestDelay);
}

void Services::ChillerService::handleChillerState()
{
	static uint32_t tmr;
	static float coldT;
	static int potIncrement = 150;
	if (millis() - tmr >= 5000)
	{
		coldT = (*_temperatureService).getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::coldCircuit);
		
		potIncrement = computePID(coldT, _targetTemperature,
			-50, -0.1, -100.0, 20) + 20;
		tmr = millis();

		if (potIncrement > 150)
		{
			potIncrement = 150;
		}
		else if (potIncrement < 50)
		{
			potIncrement = 50;
		}
		Wire.beginTransmission(0x2E);
		Wire.write(byte(0x00));
		Wire.write(potIncrement);
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

Models::Enums::ChillerState Services::ChillerService::GetChillerState()
{
	return _state;
}

void Services::ChillerService::SetChillerState(Models::Enums::ChillerState state)
{
	_state = state;
}

float Services::ChillerService::getTargetTemperature()
{
	return _targetTemperature;
}

Services::TemperatureService* Services::ChillerService::getTemperatureService()
{
	return _temperatureService;
}
