#pragma once

#include "BaseSensor.h"

Models::TemperatureSensors::BaseSensor::BaseSensor(Models::Enums::TemperatureSensorTarget sensorTarget,
	Models::Enums::TemperatureSensorType sensorType, String name)
{
	_sensorTarget = sensorTarget;
	_sensorType = sensorType;
	_name = name;
}

const uint16_t Models::TemperatureSensors::BaseSensor::getCommonPayloadSize()
{
	return _commonPayloadSize + JSON_STRING_SIZE(_name.length());
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
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());
	JsonObject payload = document.to<JsonObject>();
	payload["Type"] = static_cast<uint16_t>(_sensorType);
	payload["Target"] = static_cast<uint16_t>(_sensorTarget);
	payload["Temperature"] = _temperature;
	payload["Name"] = _name;
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

String Models::TemperatureSensors::BaseSensor::getName()
{
	return _name;
}
