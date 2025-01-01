#pragma once

#include "DS18B20.h"

Models::TemperatureSensors::DS18B20::DS18B20(uint8_t* sensorAddress, Models::Enums::TemperatureSensorTarget sensorTarget, String name) :
	Models::TemperatureSensors::BaseSensor(sensorTarget, Models::Enums::TemperatureSensorType::DS18B20, name)
{
	_sensorAddress = sensorAddress;
	Communication::Abstractions::BaseSerializableObject::setJsonSize(
		Models::TemperatureSensors::BaseSensor::getCommonPayloadSize());
}

void Models::TemperatureSensors::DS18B20::init(DallasTemperature* sensor, uint8_t _temperaturePrecision)
{
	_sensor = sensor;
	(*_sensor).begin();
	(*_sensor).setResolution(_sensorAddress, _temperaturePrecision);
	(*_sensor).setWaitForConversion(false);
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
		(*_sensor).requestTemperaturesByAddress(getSensorAddress());
		_temperature = float((*_sensor).getTempC(_sensorAddress));
	}
}

void Models::TemperatureSensors::DS18B20::clear()
{

}

DynamicJsonDocument Models::TemperatureSensors::DS18B20::createPayload()
{
	return BaseSensor::createPayload();
}

uint8_t* Models::TemperatureSensors::DS18B20::getSensorAddress()
{
	return _sensorAddress;
}