#pragma once

#include <LinkedList.h>
#include "TemperatureSensor.h"
#include "PeltierModule.h"

namespace Services
{
	class ChillerService
	{
	private:
		const uint8_t _temperaturePrecision = 12;
		const uint32_t _delayForDisablePeltier = 30000;

		LinkedList<Models::PeltierModule *> * _peltierModules;
		LinkedList<Models::TemperatureSensor *> * _temperatureSensors;

		float _targetTemperature;

		float getColdCircuitTemperature();

	protected:


	public:

		ChillerService(LinkedList<int8_t> _peltierPins, LinkedList<int8_t> _temperatureSensorsPin, float _targetCircuitTemperature);

		void handleChillerState();
	};
}