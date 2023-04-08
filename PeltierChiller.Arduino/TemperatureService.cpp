#include "TemperatureService.h"

Services::TemperatureService::TemperatureService(uint8_t _temperatureSensorsPin, Models::TemperatureSensor* _tSensors[])
{ 
	_temperatureSensors = new LinkedList<Models::TemperatureSensor*>();

	_oneWire = new OneWire(_temperatureSensorsPin);
	_dallasSensors = new DallasTemperature(_oneWire);

	_dallasSensors->begin();

	for (uint8_t index = 0; index < _dallasSensors->getDeviceCount(); index++)
	{
		_tSensors[index]->init(_dallasSensors, _temperaturePrecision);

		(*_temperatureSensors).add(_tSensors[index]);
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
			//Serial.print(getSensorTemperature(index));
			//Serial.println(getTemperatureSensorTargetName((*_temperatureSensors).get(index)->getSensorTarget()));
			_temperature = _temperature + getSensorTemperature(index);
			_sensorsCounter++;
		}
	}

	return _temperature / _sensorsCounter;
}

float Services::TemperatureService::getSensorTemperature(uint8_t sensorIndex)
{
	return (*_temperatureSensors).get(sensorIndex)->getTemperature();
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
	case Models::Enums::TemperatureSensorTarget::coldCircuit: return "Cold circuit";
	case Models::Enums::TemperatureSensorTarget::hotCircuit: return "Hot circuit";
	}
}
