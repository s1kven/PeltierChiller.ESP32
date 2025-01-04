#pragma once
#ifndef _Ds18b20Configuration_
#define _Ds18b20Configuration_ 
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
				class Ds18b20Configuration :
					public Abstractions::BaseDeserializableObject,
					public Abstractions::BaseSerializableObject
				{
				private:
					const uint16_t _payloadSize = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(8);
					uint8_t _address[8];
					String _name;
					TemperatureSensorTarget _target;

				protected:
					void init() override;

				public:
					~Ds18b20Configuration();
					void init(uint8_t* address, String name, TemperatureSensorTarget target);

					uint8_t* getAddress();
					String getName();
					TemperatureSensorTarget getTarget();

					DynamicJsonDocument createPayload() override;
				};
			}
		}
	}
}
#endif