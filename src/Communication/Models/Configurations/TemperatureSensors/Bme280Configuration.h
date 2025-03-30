#pragma once
#ifndef _Bme280Configuration_
#define _Bme280Configuration_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "Communication/Abstractions/BaseDeserializableObject.h"
#include "Communication/Abstractions/BaseSerializableObject.h"
#include "Models/Enums/TemperatureSensorTarget.cpp"

using namespace Models::Enums;

namespace Communication
{
	namespace Models
	{
		namespace Configurations
		{
			namespace TemperatureSensors
			{
				class Bme280Configuration :
					public Abstractions::BaseDeserializableObject,
					public Abstractions::BaseSerializableObject
				{

				private:
					const uint16_t _payloadSize = JSON_OBJECT_SIZE(3);
					uint16_t _address;
					String _name;
					TemperatureSensorTarget _target;

				protected:
					void init() override;

				public:
					~Bme280Configuration();
					void init(uint16_t address, String name, TemperatureSensorTarget target);

					uint16_t getAddress();
					TemperatureSensorTarget getTarget();
					String getName();

					DynamicJsonDocument createPayload() override;
				};
			}
		}
	}
}
#endif