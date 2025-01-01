#pragma once
#ifndef _BME280_
#define _BME280_ 

#include <GyverBME280.h>
#include "BaseSensor.h"

namespace Models
{
	namespace TemperatureSensors
	{
		class BME280 :
			public Models::TemperatureSensors::BaseSensor
		{
		private:
			const uint16_t _payloadSize = JSON_OBJECT_SIZE(2);

			GyverBME280 bme;
			uint8_t _sensorAddress;
			float _humidity = 0.0;
			float _pressure = 0.0;

		protected:


		public:

			BME280(uint8_t sensorAddress, Models::Enums::TemperatureSensorTarget _sensorTarget);
			void init();
			float getTemperature();
			float getHumidity();
			float getPressure();
			void sensorRequest() override;
			void clear() override;
			DynamicJsonDocument createPayload() override;
			uint8_t getSensorAddress();
		};
	}
}
#endif
