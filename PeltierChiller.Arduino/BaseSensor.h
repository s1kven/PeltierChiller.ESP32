#pragma once

#include "TemperatureSensorTarget.cpp"
#include "TemperatureSensorType.cpp"
#include "BaseJsonModel.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace Models::TemperatureSensors
{

	class BaseSensor :
		public Models::Abstractions::BaseJsonModel
	{
	private:
		uint16_t _payloadSize;

		Models::Enums::TemperatureSensorTarget _sensorTarget;
		Models::Enums::TemperatureSensorType _sensorType;

	protected:

		float _temperature = 0.0;
		uint32_t _sensorRequestTimer = 0;

	public:

		BaseSensor(Models::Enums::TemperatureSensorTarget _sensorTarget, Models::Enums::TemperatureSensorType _sensorType,
			uint16_t payloadSize);
		void init();
		float getTemperature();
		virtual void sensorRequest() = 0;
		virtual DynamicJsonDocument createPayload() override;
		Models::Enums::TemperatureSensorTarget getSensorTarget();
		Models::Enums::TemperatureSensorType getSensorType();
	};
}