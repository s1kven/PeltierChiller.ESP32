// DS18B20
#pragma once


#include <OneWire.h>
#include <DallasTemperature.h>
#include <stdint.h>
#include "TemperatureSensorTarget.cpp"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace Models
{

	class TemperatureSensor
	{
	private:

		DallasTemperature* _sensor;
		DeviceAddress* _sensorAddress;
		Models::Enums::TemperatureSensorTarget _sensorTarget;


	protected:


	public:

		TemperatureSensor(DeviceAddress* sensorAddress, DallasTemperature* sensor);
		void init(uint8_t _temperaturePrecision, Models::Enums::TemperatureSensorTarget _sensorTarget);
		float getTemperature();
		Models::Enums::TemperatureSensorTarget getSensorTarget();
		DeviceAddress* getSensorAddress();
	};
}