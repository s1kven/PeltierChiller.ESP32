#pragma once
// DS18B20
#include "TemperatureSensor.h"


Models::TemperatureSensor::TemperatureSensor(DeviceAddress* sensorAddress, DallasTemperature* sensor)
{
	_sensor = sensor;
	_sensorAddress = sensorAddress;
}

void Models::TemperatureSensor::init(uint8_t _temperaturePrecision, Models::Enums::TemperatureSensorTarget _sensorTarget)
{
	(*_sensor).begin();
	(*_sensor).setResolution((*_sensorAddress), _temperaturePrecision);
	this->_sensorTarget = _sensorTarget;
}

float Models::TemperatureSensor::getTemperature()
{
	(*_sensor).requestTemperatures();
	return float((*_sensor).getTempC((*_sensorAddress)));
}

Models::Enums::TemperatureSensorTarget Models::TemperatureSensor::getSensorTarget()
{
	return _sensorTarget;
}

DeviceAddress *Models::TemperatureSensor::getSensorAddress()
{
	return _sensorAddress;
}
