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

		int8_t _dataPin;
		uint8_t _sensorIndex;

		OneWire* oneWire;
		DallasTemperature* sensor;
		DeviceAddress* sensorAddress;
		Models::Enums::TemperatureSensorTarget::TemperatureSensorTarget _sensorTarget;


	protected:


	public:

		TemperatureSensor(int8_t _temperaturePin, uint8_t _onBusIndex);
		void init(uint8_t _temperaturePrecision, Models::Enums::TemperatureSensorTarget::TemperatureSensorTarget _sensorTarget);
		void setDataPin(int8_t _temperaturePin);
		int8_t getDataPin();
		float getTemperature();
		Models::Enums::TemperatureSensorTarget::TemperatureSensorTarget getSensorTarget();
		DeviceAddress* getSensorAddress();
	};
}