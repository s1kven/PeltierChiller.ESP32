#pragma once
#ifndef _TemperatureService_
#define _TemperatureService_ 

#include <LinkedList.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "BaseSensor.h"
#include "DS18B20.h"
#include "BME280.h"
#include "NTC.h"
#include "TemperatureSensorsConfiguration.h"

namespace Services
{
	class TemperatureService
	{
	private:
		float const a = 17.27;
		float const b = 237.7;
		uint8_t _temperaturePrecision;

		OneWire* _oneWire;
		DallasTemperature* _dallasSensors;
		uint8_t _dallasTemperatureSensorsPin;
		LinkedList<Models::TemperatureSensors::BaseSensor*>* _temperatureSensors;
		uint32_t _sensorsRequestTimer = 0;

		Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* _configuration;

		void dallasInit();
		void initConfiguration();

	public:
		float getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget);
		float getHumidityForSpecificTarget(Models::Enums::TemperatureSensorTarget);
		float getPressureForSpecificTarget(Models::Enums::TemperatureSensorTarget);
		float getDewPointTemperature(Models::Enums::TemperatureSensorTarget);
		float getSensorTemperature(uint8_t);
		String getSensorTarget(uint8_t);
		const char* getTemperatureSensorTargetName(Models::Enums::TemperatureSensorTarget);
		void requestSensors(uint16_t _sensorsRequestDelay);
		TemperatureService(Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* configuration);
		LinkedList<Models::TemperatureSensors::BaseSensor*>* getTemperatureSensors();

		void clear();
	};
}
#endif