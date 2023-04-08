#pragma once
#include <LinkedList.h>
#include "TemperatureSensor.h"

namespace Services
{
	class TemperatureService
	{
	private:

		const uint8_t _temperaturePrecision = 12;

		OneWire* _oneWire;
		DallasTemperature* _dallasSensors;
		LinkedList<Models::TemperatureSensor*>* _temperatureSensors;

	public:
		float getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget);
		float getSensorTemperature(uint8_t);
		String getSensorTarget(uint8_t);
		const char* getTemperatureSensorTargetName(Models::Enums::TemperatureSensorTarget);
		TemperatureService(uint8_t _temperatureSensorsPin, Models::TemperatureSensor* _tSensors[]);
	};
}
