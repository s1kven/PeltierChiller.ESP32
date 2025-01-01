#pragma once

#include "NTC.h"

Models::TemperatureSensors::NTC::NTC(uint8_t sensorAddress, uint32_t resistance, uint16_t bCoefficient,
	uint32_t resistanceNTC, Models::Enums::TemperatureSensorTarget sensorTarget, uint8_t baseNTCTemp,
	float ntcVoltage, uint8_t adcResolution) :
	Models::TemperatureSensors::BaseSensor(sensorTarget, Models::Enums::TemperatureSensorType::NTC)
{
	_sensorAddress = sensorAddress;
	_resistance = resistance;
	_bCoefficient = bCoefficient;
	_resistanceNTC = resistanceNTC;
	_baseNTCTemp = baseNTCTemp;
	_ntcVoltage = ntcVoltage;
	_adcMaxValue = pow(2.0, adcResolution);
	Communication::Abstractions::BaseSerializableObject::setJsonSize(
		Models::TemperatureSensors::BaseSensor::getCommonPayloadSize());
}

float Models::TemperatureSensors::NTC::getTemperature()
{
	return BaseSensor::getTemperature();
}

void Models::TemperatureSensors::NTC::sensorRequest()
{
	float Vout, Rt = 0;

	float value = analogRead(_sensorAddress);
	value = ADC_LUT[(int)value];
	Vout = value * _ntcVoltage / _adcMaxValue;
	Rt = _resistance * Vout / (_ntcVoltage - Vout);
	_temperature = (1 / (1 / (_baseNTCTemp + _zeroCbyK) + log(Rt / _resistanceNTC) / _bCoefficient))-_zeroCbyK;
}

void Models::TemperatureSensors::NTC::clear()
{
}

DynamicJsonDocument Models::TemperatureSensors::NTC::createPayload()
{
	return BaseSensor::createPayload();
}

uint8_t Models::TemperatureSensors::NTC::getSensorAddress()
{
	return uint8_t();
}