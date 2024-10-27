#pragma once

#include "TemperatureService.h"

Services::TemperatureService::TemperatureService(
	Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* configuration)
{
	_configuration = configuration;
	_temperatureSensors = new LinkedList<Models::TemperatureSensors::BaseSensor*>();

	initConfiguration();
}

LinkedList<Models::TemperatureSensors::BaseSensor*>* Services::TemperatureService::getTemperatureSensors()
{
	return _temperatureSensors;
}

float Services::TemperatureService::getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget _sensorsTarget)
{
	float _temperature = 0;
	uint8_t _sensorsCounter = 0;

	for (uint8_t index = 0; index < (*_temperatureSensors).size(); index++)
	{
		if ((*(*_temperatureSensors).get(index)).getSensorTarget() == _sensorsTarget)
		{
			_temperature = _temperature + getSensorTemperature(index);
			_sensorsCounter++;
		}
	}

	return _temperature / _sensorsCounter;
}

float Services::TemperatureService::getHumidityForSpecificTarget(Models::Enums::TemperatureSensorTarget _sensorsTarget)
{
	float _humidity = 0;
	uint8_t _sensorsCounter = 0;

	for (uint8_t index = 0; index < (*_temperatureSensors).size(); index++)
	{
		if ((*(*_temperatureSensors).get(index)).getSensorTarget() == _sensorsTarget && 
			(*(*_temperatureSensors).get(index)).getSensorType() == Models::Enums::TemperatureSensorType::BME280)
		{
			Models::TemperatureSensors::BME280* bme = (Models::TemperatureSensors::BME280*)(*_temperatureSensors).get(index);
			_humidity = _humidity + bme->getHumidity();
			_sensorsCounter++;
		}
	}

	return _humidity / _sensorsCounter;
}

void Services::TemperatureService::initConfiguration()
{
	_dallasTemperatureSensorsPin = _configuration->getDs18b20ListConfiguration()->getPin();
	_temperaturePrecision = _configuration->getDs18b20ListConfiguration()->getTemperaturePrecision();

	dallasInit();

	LinkedList<Models::TemperatureSensors::BaseSensor*>* sensors = new LinkedList<Models::TemperatureSensors::BaseSensor*>();
	LinkedList<Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration*>* ds18b20Items =
		_configuration->getDs18b20ListConfiguration()->getItems();

	for (int i = 0; i < ds18b20Items->size(); i++)
	{
		Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration* currentConfiguration = ds18b20Items->get(i);
		sensors->add(new Models::TemperatureSensors::DS18B20(currentConfiguration->getAddress(),
			currentConfiguration->getTarget()));
	}

	LinkedList<Communication::Models::Configurations::TemperatureSensors::NtcConfiguration*>* ntcItems =
		_configuration->getNtcListConfiguration()->getItems();

	for (int i = 0; i < ntcItems->size(); i++)
	{
		Communication::Models::Configurations::TemperatureSensors::NtcConfiguration* currentConfiguration = ntcItems->get(i);
		sensors->add(new Models::TemperatureSensors::NTC(currentConfiguration->getAddress(),
			currentConfiguration->getResistance(), currentConfiguration->getBCoefficient(), currentConfiguration->getResistanceNtc(),
			currentConfiguration->getTarget(), currentConfiguration->getBaseTemperature(), currentConfiguration->getSupplyVoltage(),
			_configuration->getNtcListConfiguration()->getAdcResolution()));
	}

	LinkedList < Communication::Models::Configurations::TemperatureSensors::Bme280Configuration*>* bme280Items =
		_configuration->getBmeListConfiguration()->getItems();

	for (int i = 0; i < bme280Items->size(); i++)
	{
		Communication::Models::Configurations::TemperatureSensors::Bme280Configuration* currentConfiguration = bme280Items->get(i);
		sensors->add(new Models::TemperatureSensors::BME280(currentConfiguration->getAddress(),
			currentConfiguration->getTarget()));
	}

	for (uint8_t index = 0; index < sensors->size(); index++)
	{
		Models::TemperatureSensors::BaseSensor* currentSensor = sensors->get(index);
		if (currentSensor->getSensorType() == Models::Enums::TemperatureSensorType::DS18B20)
		{
			Models::TemperatureSensors::DS18B20* ds = (Models::TemperatureSensors::DS18B20*)currentSensor;
			ds->init(_dallasSensors, _temperaturePrecision);
		}
		else if (currentSensor->getSensorType() == Models::Enums::TemperatureSensorType::BME280)
		{
			Models::TemperatureSensors::BME280* bme = (Models::TemperatureSensors::BME280*)currentSensor;
			bme->init();
		}
		else if (currentSensor->getSensorType() == Models::Enums::TemperatureSensorType::NTC)
		{
			// Init don't needed
		}

		(*_temperatureSensors).add(currentSensor);
	}

	delete ds18b20Items;
	delete ntcItems;
	delete bme280Items;
	delete sensors;
}

float Services::TemperatureService::getPressureForSpecificTarget(Models::Enums::TemperatureSensorTarget _sensorsTarget)
{
	float _pressure = 0;
	uint8_t _sensorsCounter = 0;

	for (uint8_t index = 0; index < (*_temperatureSensors).size(); index++)
	{
		if ((*(*_temperatureSensors).get(index)).getSensorTarget() == _sensorsTarget &&
			(*(*_temperatureSensors).get(index)).getSensorType() == Models::Enums::TemperatureSensorType::BME280)
		{
			Models::TemperatureSensors::BME280* bme = (Models::TemperatureSensors::BME280*)(*_temperatureSensors).get(index);
			_pressure = _pressure + bme->getPressure();
			_sensorsCounter++;
		}
	}

	return _pressure / _sensorsCounter;
}

// a = 17.27 b = 237.7
// f(T, R) = (a * T) / (b + T) + ln(R / 100)
// Td = (b * f(T, R)) / (a - f(T, R))
float Services::TemperatureService::getDewPointTemperature(Models::Enums::TemperatureSensorTarget target)
{
	float roomTemperature = getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::room);
	double function = ((a * roomTemperature) / (b + roomTemperature)) + log((getHumidityForSpecificTarget(Models::Enums::TemperatureSensorTarget::room) / 100.0));
	double dewPointTemperature = (b * function) / (a - function);
	return dewPointTemperature;
}

float Services::TemperatureService::getSensorTemperature(uint8_t sensorIndex)
{
	Models::TemperatureSensors::BaseSensor* sensor = (*_temperatureSensors).get(sensorIndex);
	return sensor->getTemperature();
}

String Services::TemperatureService::getSensorTarget(uint8_t sensorIndex)
{
	return getTemperatureSensorTargetName((*_temperatureSensors).get(sensorIndex)->getSensorTarget());
}

const char* Services::TemperatureService::getTemperatureSensorTargetName(Models::Enums::TemperatureSensorTarget target)
{
	switch (target)
	{
	case Models::Enums::TemperatureSensorTarget::none: return "None";
	case Models::Enums::TemperatureSensorTarget::room: return "Room";
	case Models::Enums::TemperatureSensorTarget::pcCase: return "PC case";
	case Models::Enums::TemperatureSensorTarget::coldCircuit: return "Cold";
	case Models::Enums::TemperatureSensorTarget::hotCircuit: return "Hot";
	}
}

void Services::TemperatureService::requestSensors(uint16_t _sensorsRequestDelay)
{
	if (millis() - _sensorsRequestTimer >= _sensorsRequestDelay)
	{
		_sensorsRequestTimer = millis();

		for (uint8_t index = 0; index < (*_temperatureSensors).size(); index++)
		{
			(*(*_temperatureSensors).get(index)).sensorRequest();
		}
	}
}

void Services::TemperatureService::dallasInit()
{
	_oneWire = new OneWire(_dallasTemperatureSensorsPin);
	_dallasSensors = new DallasTemperature(_oneWire);

	_dallasSensors->begin();
}
