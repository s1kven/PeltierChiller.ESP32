#pragma once

#include "NTC.h"

Models::TemperatureSensors::NTC::NTC(uint8_t sensorAddress, uint32_t resistance, uint16_t bCoefficient,
	uint32_t resistanceNTC, Models::Enums::TemperatureSensorTarget sensorTarget, uint8_t baseNTCTemp) :
	Models::TemperatureSensors::BaseSensor(sensorTarget, Models::Enums::TemperatureSensorType::NTC, _payloadSize)
{
	_sensorAddress = sensorAddress;
	_resistance = resistance;
	_bCoefficient = bCoefficient;
	_resistanceNTC = resistanceNTC;
	_baseNTCTemp = baseNTCTemp;
}

float Models::TemperatureSensors::NTC::getTemperature()
{
	return BaseSensor::getTemperature();
}

void Models::TemperatureSensors::NTC::sensorRequest()
{
	float value = analogRead(_sensorAddress);
	value = ((float) _resistanceNTC / _resistance) / (1023.0f / value - 1.0);
	value = (log(value) / _bCoefficient) + 1.0 / (_baseNTCTemp + _zeroCbyK);
	_temperature = (1.0 / value - _zeroCbyK);
}

DynamicJsonDocument Models::TemperatureSensors::NTC::createPayload()
{
	return BaseSensor::createPayload();
}

uint8_t Models::TemperatureSensors::NTC::getSensorAddress()
{
	return uint8_t();
}