#pragma once
#include <LinkedList.h>
#include "TemperatureSensor.h"

namespace Services
{
	class TemperatureService
	{
	private:

		const uint8_t _temperaturePrecision = 12;

		float _targetTemperature;
		LinkedList<Models::TemperatureSensor*>* _temperatureSensors;

	public:

		float getTargetTemperature();
		float getColdCircuitTemperature();
		TemperatureService(LinkedList<int8_t> _temperatureSensorsPin, float _targetCircuitTemperature);
	};
}
