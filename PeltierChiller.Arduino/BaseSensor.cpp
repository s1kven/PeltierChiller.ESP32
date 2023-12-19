#pragma once
// DS18B20
#include "BaseSensor.h"


Models::TemperatureSensors::BaseSensor::BaseSensor(Models::Enums::TemperatureSensorTarget sensorTarget, Models::Enums::TemperatureSensorType sensorType)
{
	_sensorTarget = sensorTarget;
	_sensorType = sensorType;
}

void Models::TemperatureSensors::BaseSensor::init()
{

}

float Models::TemperatureSensors::BaseSensor::getTemperature()
{
	return _temperature;
}

void Models::TemperatureSensors::BaseSensor::sensorRequest()
{

}

Models::Enums::TemperatureSensorTarget Models::TemperatureSensors::BaseSensor::getSensorTarget()
{
	return _sensorTarget;
}

Models::Enums::TemperatureSensorType Models::TemperatureSensors::BaseSensor::getSensorType()
{
	return _sensorType;
}
