#pragma once
#ifndef _DS18B20_
#define _DS18B20_ 

#include <OneWire.h>
#include <DallasTemperature.h>
#include <stdint.h>
#include "BaseSensor.h"

namespace Models
{
	namespace TemperatureSensors
	{
		class DS18B20 :
			public Models::TemperatureSensors::BaseSensor
		{
		private:
			DallasTemperature* _sensor;
			uint8_t* _sensorAddress;

		protected:


		public:

			DS18B20(uint8_t* sensorAddress, Models::Enums::TemperatureSensorTarget _sensorTarget);
			void init(DallasTemperature* sensor, uint8_t _temperaturePrecision);
			float getTemperature();
			void sensorRequest() override;
			DynamicJsonDocument createPayload() override;
			uint8_t* getSensorAddress();
		};
	}
}
#endif

