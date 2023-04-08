#pragma once
// DS18B20
#include "TemperatureSensor.h"


Models::TemperatureSensor::TemperatureSensor(DeviceAddress* sensorAddress, Models::Enums::TemperatureSensorTarget _sensorTarget)
{
	_sensorAddress = sensorAddress;
	this->_sensorTarget = _sensorTarget;
}

void Models::TemperatureSensor::init(DallasTemperature* sensor, uint8_t _temperaturePrecision)
{
	_sensor = sensor;
	(*_sensor).begin();
	(*_sensor).setResolution((*_sensorAddress), _temperaturePrecision);
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
