#pragma once

#include <LinkedList.h>
#include "PeltierModule.h"
#include "TemperatureService.h"

namespace Services
{
	class ChillerService
	{
	private:
		const uint32_t _delayForDisablePeltier = 30000;

		LinkedList<Models::PeltierModule *> * _peltierModules;
		Services::TemperatureService* _temperatureService;

	protected:


	public:

		ChillerService(LinkedList<int8_t> _peltierPins, LinkedList<int8_t> _temperatureSensorsPin, float _targetCircuitTemperature);

		void handleChillerState();
	};
}