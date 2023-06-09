// 
// 
// 

#include "BME280.h"

Models::TemperatureSensors::BME280::BME280(uint8_t sensorAddress, Models::Enums::TemperatureSensorTarget sensorTarget) :
	Models::TemperatureSensors::BaseSensor(sensorTarget, Models::Enums::TemperatureSensorType::BME280)
{
	_sensorAddress = sensorAddress;
}

void Models::TemperatureSensors::BME280::init()
{
	bme.begin(_sensorAddress);
	BaseSensor::init();
}

float Models::TemperatureSensors::BME280::getTemperature()
{
	return BaseSensor::getTemperature();
}

float Models::TemperatureSensors::BME280::getHumidity()
{
	return _humidity;
}

float Models::TemperatureSensors::BME280::getPressure()
{
	return _pressure;
}

void Models::TemperatureSensors::BME280::sensorRequest()
{
	if (millis() - _sensorRequestTimer >= 1000)
	{
		_sensorRequestTimer = millis();
		_temperature = bme.readTemperature();
		_humidity = bme.readHumidity();
		_pressure = bme.readPressure();
	}
}

uint8_t Models::TemperatureSensors::BME280::getSensorAddress()
{
	return _sensorAddress;
}