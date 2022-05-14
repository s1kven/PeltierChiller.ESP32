#include "ChillerService.h"

Services::ChillerService::ChillerService(LinkedList<int8_t> _peltierPins, LinkedList<int8_t> _temperatureSensorsPin, float _targetCircuitTemperature)
{
	_peltierModules = new LinkedList<Models::PeltierModule *>();
	_temperatureSensors = new LinkedList<Models::TemperatureSensor *>();

	_targetTemperature = _targetCircuitTemperature;

	for (uint8_t index = 0; index < _peltierPins.size(); index++)
	{
		Models::PeltierModule* module = new Models::PeltierModule(_peltierPins.get(index));

		(*_peltierModules).add(module);
	}


	for (uint8_t index = 0; index < _temperatureSensorsPin.size(); index++)
	{
		Models::TemperatureSensor* sensor = new Models::TemperatureSensor(_temperatureSensorsPin.get(index), index);
		(*sensor).init(_temperaturePrecision, Models::Enums::TemperatureSensorTarget::coldCircuit);

		(*_temperatureSensors).add(sensor);
	}
}

void Services::ChillerService::handleChillerState()
{
	Serial.println(getColdCircuitTemperature());

	static uint32_t timer;

	for (uint8_t index = 0; index < (*_peltierModules).size(); index++)
	{
		if ((*(*_temperatureSensors).get(index)).getSensorTarget() == Models::Enums::TemperatureSensorTarget::TemperatureSensorTarget::coldCircuit)
		{
			if (getColdCircuitTemperature() >= _targetTemperature)
			{
				(*(*_peltierModules).get(index)).switchingOn();

				timer = millis();
			}
			else
			{
				if (millis() - timer >= _delayForDisablePeltier)
				{
					(*(*_peltierModules).get(index)).switchingOff(); 
				}
			}
		}
	}
}


float Services::ChillerService::getColdCircuitTemperature()
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
