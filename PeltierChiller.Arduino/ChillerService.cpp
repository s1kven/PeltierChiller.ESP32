#include "ChillerService.h"

Services::ChillerService::ChillerService(LinkedList<int8_t> _peltierPins, LinkedList<int8_t> _temperatureSensorsPin, float _targetCircuitTemperature)
{
	_peltierModules = new LinkedList<Models::PeltierModule *>();

	_temperatureService = new Services::TemperatureService(_temperatureSensorsPin, _targetCircuitTemperature); 
	Serial.println();

	for (uint8_t index = 0; index < _peltierPins.size(); index++)
	{
		Models::PeltierModule* module = new Models::PeltierModule(_peltierPins.get(index));

		(*_peltierModules).add(module);
	}
}

void Services::ChillerService::handleChillerState()
{
	Serial.println((*_temperatureService).getColdCircuitTemperature());

	static uint32_t timer;

	for (uint8_t index = 0; index < (*_peltierModules).size(); index++)
	{
		if ((*_temperatureService).getColdCircuitTemperature() >= (*_temperatureService).getTargetTemperature())
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
