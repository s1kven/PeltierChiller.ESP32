#pragma once
#include <LinkedList.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "BaseSensor.h"
#include "DS18B20.h"
#include "BME280.h"
#include "NTC.h"

namespace Services
{
	class TemperatureService
	{
	private:

		const uint8_t _temperaturePrecision = 12;

		OneWire* _oneWire;
		DallasTemperature* _dallasSensors;
		LinkedList<Models::TemperatureSensors::BaseSensor*>* _temperatureSensors;

		void dallasInit(uint8_t _dallasTemperatureSensorsPin);

	public:
		float getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget);
		float getHumidityForSpecificTarget(Models::Enums::TemperatureSensorTarget);
		float getPressureForSpecificTarget(Models::Enums::TemperatureSensorTarget);
		float getSensorTemperature(uint8_t);
		String getSensorTarget(uint8_t);
		const char* getTemperatureSensorTargetName(Models::Enums::TemperatureSensorTarget);
		TemperatureService(uint8_t _dallasTemperatureSensorsPin, uint8_t _tSensorsCount, Models::TemperatureSensors::BaseSensor* _tSensors[]);
	};
}