#pragma once

#include "BaseSensor.h"


Models::TemperatureSensors::BaseSensor::BaseSensor(Models::Enums::TemperatureSensorTarget sensorTarget, 
	Models::Enums::TemperatureSensorType sensorType, uint16_t payloadSize)
{
	_payloadSize = payloadSize;
	_sensorTarget = sensorTarget;
	_sensorType = sensorType;
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize);
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

DynamicJsonDocument Models::TemperatureSensors::BaseSensor::createPayload()
{
	DynamicJsonDocument document(_payloadSize);
	JsonObject payload = document.to<JsonObject>();
	payload["Type"] = static_cast<uint16_t>(getSensorType());
	payload["Target"] = static_cast<uint16_t>(getSensorTarget());
	payload["Temperature"] = getTemperature();
	return document;
}

Models::Enums::TemperatureSensorTarget Models::TemperatureSensors::BaseSensor::getSensorTarget()
{
	return _sensorTarget;
}

Models::Enums::TemperatureSensorType Models::TemperatureSensors::BaseSensor::getSensorType()
{
	return _sensorType;
}
