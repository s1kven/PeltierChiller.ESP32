#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>
#include <stdint.h>
#include "BaseSensor.h"

namespace Models::TemperatureSensors
{

	class DS18B20 :
		public Models::TemperatureSensors::BaseSensor
	{
	private:

		DallasTemperature* _sensor;
		DeviceAddress* _sensorAddress;

	protected:


	public:

		DS18B20(DeviceAddress* sensorAddress, Models::Enums::TemperatureSensorTarget _sensorTarget);
		void init(DallasTemperature* sensor, uint8_t _temperaturePrecision) ;
		float getTemperature();
		void sensorRequest() override;
		DynamicJsonDocument createPayload() override;
		DeviceAddress* getSensorAddress();
	};
}

