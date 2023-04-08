#include "ChillerService.h"

Services::ChillerService::ChillerService(uint8_t _temperatureSensorsPin, Models::TemperatureSensor* _tSensors[], float _targetCircuitTemperature)
{
	_temperatureService = new Services::TemperatureService(_temperatureSensorsPin, _tSensors);
	_targetTemperature = _targetCircuitTemperature;
	Serial.println();
}

void Services::ChillerService::handleChillerState()
{
	static uint32_t tmr;
	static float coldT;
	static int potIncrement = 130;
	if (millis() - tmr >= 5000)
	{
		coldT = (*_temperatureService).getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::coldCircuit);
		Serial.println("--------------------------------------------");
		Serial.print("COlD: ");
		Serial.println(coldT);
		Serial.print("HOT: ");
		Serial.println((*_temperatureService).getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::hotCircuit));
		(*_temperatureService).getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::hotCircuit);
		Serial.println();

		potIncrement = computePID(coldT, _targetTemperature,
			-45, -0.1, -100.0, 20) + 45;
		tmr = millis();

		if (potIncrement > 170)
		{
			potIncrement = 170;
		}
		else if (potIncrement < 45)
		{
			potIncrement = 45;
		}
		Wire.beginTransmission(0x2E);
		Wire.write(byte(0x00));
		Wire.write(potIncrement);
		Serial.println(potIncrement);
		Wire.endTransmission();
	}
}

int Services::ChillerService::computePID(float _currentT, float _targetT, float _kp, float _ki, float _kd, float _dt)
{
	float errT = _targetT - _currentT;
	static float integral = 0;
	static float prevErr = 0;
	integral += errT * _dt;
	float D = (errT - prevErr) / _dt;
	prevErr = errT;

	Serial.println(errT * _kp);
	Serial.println(integral * _ki);
	Serial.println(D * _kd);
	return (errT * _kp + integral * _ki + D * _kd);
}

float Services::ChillerService::getTargetTemperature()
{
	return _targetTemperature;
}
