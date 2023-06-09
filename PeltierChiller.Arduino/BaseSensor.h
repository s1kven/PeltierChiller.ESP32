// DS18B20
#pragma once

#include "TemperatureSensorTarget.cpp"
#include "TemperatureSensorType.cpp"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace Models::TemperatureSensors
{

	class BaseSensor
	{
	private:
		Models::Enums::TemperatureSensorTarget _sensorTarget;
		Models::Enums::TemperatureSensorType _sensorType;

	protected:

		float _temperature = 0.0;
		uint32_t _sensorRequestTimer = 0;

	public:

		BaseSensor(Models::Enums::TemperatureSensorTarget _sensorTarget, Models::Enums::TemperatureSensorType _sensorType);
		void init();
		float getTemperature();
		virtual void sensorRequest() = 0;
		Models::Enums::TemperatureSensorTarget getSensorTarget();
		Models::Enums::TemperatureSensorType getSensorType();
	};
}