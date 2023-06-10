#include "TemperatureService.h"

Services::TemperatureService::TemperatureService(uint8_t _dallasTemperatureSensorsPin,
	uint8_t _tSensorsCount, Models::TemperatureSensors::BaseSensor* _tSensors[])
{ 
	_temperatureSensors = new LinkedList<Models::TemperatureSensors::BaseSensor*>();

	dallasInit(_dallasTemperatureSensorsPin);

	for (uint8_t index = 0; index < _tSensorsCount; index++)
	{
		Models::TemperatureSensors::BaseSensor* currentSensor = _tSensors[index];
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
			bme->sensorRequest();
			_humidity = _humidity + bme->getHumidity();
			_sensorsCounter++;
		}
	}

	return _humidity / _sensorsCounter;
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
			bme->sensorRequest();
			_pressure = _pressure + bme->getPressure();
			_sensorsCounter++;
		}
	}

	return _pressure / _sensorsCounter;
}

float Services::TemperatureService::getSensorTemperature(uint8_t sensorIndex)
{
	Models::TemperatureSensors::BaseSensor* sensor = (*_temperatureSensors).get(sensorIndex);
	sensor->sensorRequest();
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

void Services::TemperatureService::dallasInit(uint8_t _dallasTemperatureSensorsPin)
{
	_oneWire = new OneWire(_dallasTemperatureSensorsPin);
	_dallasSensors = new DallasTemperature(_oneWire);

	_dallasSensors->begin();
}
