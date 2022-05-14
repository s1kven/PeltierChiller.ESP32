#pragma once
// DS18B20
#include "TemperatureSensor.h"


Models::TemperatureSensor::TemperatureSensor(int8_t _temperaturePin, uint8_t _onBusIndex)
{
	_dataPin = _temperaturePin;
	_sensorIndex = _onBusIndex;

	oneWire = new OneWire(_dataPin);
	sensor = new DallasTemperature(oneWire);
}

void Models::TemperatureSensor::init(uint8_t _temperaturePrecision, Models::Enums::TemperatureSensorTarget::TemperatureSensorTarget _sensorTarget)
{
	(*sensor).begin();
	(*sensor).getAddress((*sensorAddress), _sensorIndex);
	(*sensor).setResolution((*sensorAddress), _temperaturePrecision);
	this->_sensorTarget = _sensorTarget;
}

void Models::TemperatureSensor::setDataPin(int8_t _temperaturePin)
{
	_dataPin = _temperaturePin;
}

int8_t Models::TemperatureSensor::getDataPin()
{
	return _dataPin;
}

float Models::TemperatureSensor::getTemperature()
{
	(*sensor).requestTemperatures();
	return float((*sensor).getTempCByIndex(_sensorIndex));
}

Models::Enums::TemperatureSensorTarget::TemperatureSensorTarget Models::TemperatureSensor::getSensorTarget()
{
	return _sensorTarget;
}

DeviceAddress *Models::TemperatureSensor::getSensorAddress()
{
	return sensorAddress;
}
