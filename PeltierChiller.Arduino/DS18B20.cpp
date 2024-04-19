#pragma once

#include "DS18B20.h"

Models::TemperatureSensors::DS18B20::DS18B20(DeviceAddress* sensorAddress, Models::Enums::TemperatureSensorTarget sensorTarget) :
	Models::TemperatureSensors::BaseSensor(sensorTarget, Models::Enums::TemperatureSensorType::DS18B20, _payloadSize)
{
	_sensorAddress = sensorAddress;
}

void Models::TemperatureSensors::DS18B20::init(DallasTemperature* sensor, uint8_t _temperaturePrecision)
{
	_sensor = sensor;
	(*_sensor).begin();
	(*_sensor).setResolution((*_sensorAddress), _temperaturePrecision);
	BaseSensor::init();
}

float Models::TemperatureSensors::DS18B20::getTemperature()
{
	return BaseSensor::getTemperature();
}

void Models::TemperatureSensors::DS18B20::sensorRequest()
{
	if (millis() - _sensorRequestTimer >= 1000)
	{
		_sensorRequestTimer = millis();
		(*_sensor).requestTemperaturesByAddress(*getSensorAddress());
		_temperature = float((*_sensor).getTempC((*_sensorAddress)));
	}
}

DynamicJsonDocument Models::TemperatureSensors::DS18B20::createPayload()
{
	return BaseSensor::createPayload();
}

DeviceAddress* Models::TemperatureSensors::DS18B20::getSensorAddress()
{
	return _sensorAddress;
}