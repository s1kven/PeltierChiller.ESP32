#pragma once

#include "BME280.h"

Models::TemperatureSensors::BME280::BME280(uint8_t sensorAddress, Models::Enums::TemperatureSensorTarget sensorTarget, String name) :
	Models::TemperatureSensors::BaseSensor(sensorTarget, Models::Enums::TemperatureSensorType::BME280, name)
{
	_sensorAddress = sensorAddress;
	Communication::Abstractions::BaseSerializableObject::setJsonSize(
		Models::TemperatureSensors::BaseSensor::getCommonPayloadSize() + _payloadSize);
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

void Models::TemperatureSensors::BME280::clear()
{
}

DynamicJsonDocument Models::TemperatureSensors::BME280::createPayload()
{
	DynamicJsonDocument document = Models::TemperatureSensors::BaseSensor::createPayload();
	DynamicJsonDocument bme280Document(Models::TemperatureSensors::BaseSensor::getJsonSize());
	JsonObject payload = bme280Document.to<JsonObject>();
	for (JsonPairConst kvp : document.as<JsonObjectConst>())
	{
		bme280Document[kvp.key()] = kvp.value();
	}
	payload["Pressure"] = _pressure;
	payload["Humidity"] = _humidity;
	document.clear();
	return bme280Document;
}

uint8_t Models::TemperatureSensors::BME280::getSensorAddress()
{
	return _sensorAddress;
}