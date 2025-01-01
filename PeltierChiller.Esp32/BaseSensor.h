#pragma once
#ifndef _BaseSensor_
#define _BaseSensor_ 

#include "TemperatureSensorTarget.cpp"
#include "TemperatureSensorType.cpp"
#include "BaseSerializableObject.h"

namespace Models
{
	namespace TemperatureSensors
	{
		class BaseSensor :
			public Communication::Abstractions::BaseSerializableObject
		{
		private:
			const uint16_t _commonPayloadSize = JSON_OBJECT_SIZE(4);

			Models::Enums::TemperatureSensorTarget _sensorTarget;
			Models::Enums::TemperatureSensorType _sensorType;
			String _name;

		protected:
			float _temperature = 0.0;
			uint32_t _sensorRequestTimer = 0;

			const uint16_t getCommonPayloadSize();

		public:

			BaseSensor(Models::Enums::TemperatureSensorTarget _sensorTarget, 
				Models::Enums::TemperatureSensorType _sensorType, String name);
			void init();
			float getTemperature();
			virtual void sensorRequest() = 0;
			virtual void clear() = 0;
			virtual DynamicJsonDocument createPayload() override;
			Models::Enums::TemperatureSensorTarget getSensorTarget();
			Models::Enums::TemperatureSensorType getSensorType();
			String getName();
		};
	}
}
#endif