// BME280.h
#pragma once

#include <GyverBME280.h>
#include "BaseSensor.h"

namespace Models::TemperatureSensors
{

	class BME280 :
		public Models::TemperatureSensors::BaseSensor
	{
	private:

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
		DynamicJsonDocument createPayload() override;
		uint8_t getSensorAddress();
	};
}
