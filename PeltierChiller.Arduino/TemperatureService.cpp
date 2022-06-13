#include "TemperatureService.h"

Services::TemperatureService::TemperatureService(LinkedList<int8_t> _temperatureSensorsPin, float _targetCircuitTemperature)
{
	_targetTemperature = _targetCircuitTemperature;    
	_temperatureSensors = new LinkedList<Models::TemperatureSensor*>();

	for (uint8_t index = 0; index < _temperatureSensorsPin.size(); index++)
	{
		Models::TemperatureSensor* sensor = new Models::TemperatureSensor(_temperatureSensorsPin.get(index), index);
		(*sensor).init(_temperaturePrecision, Models::Enums::TemperatureSensorTarget::coldCircuit);

		(*_temperatureSensors).add(sensor);
	}
}

float Services::TemperatureService::getTargetTemperature()
{
	return _targetTemperature;
}

float Services::TemperatureService::getColdCircuitTemperature()
{
	float _coldCircuitTemperature = 0;
	uint8_t _coldCircuitSensorsCounter = 0;

	for (uint8_t index = 0; index < (*_temperatureSensors).size(); index++)
	{
		if ((*(*_temperatureSensors).get(index)).getSensorTarget() == Models::Enums::TemperatureSensorTarget::TemperatureSensorTarget::coldCircuit)
		{
			_coldCircuitTemperature = _coldCircuitTemperature + (*(*_temperatureSensors).get(index)).getTemperature();
			_coldCircuitSensorsCounter++;
		}
	}

	return _coldCircuitTemperature / _coldCircuitSensorsCounter;
}
