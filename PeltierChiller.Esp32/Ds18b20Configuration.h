#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "BaseDeserializableObject.h"
#include "TemperatureSensorTarget.cpp"

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
					public Abstractions::BaseDeserializableObject
				{

				private:
					uint8_t _address[8];
					TemperatureSensorTarget _target;

				protected:
					void init() override;

				public:
					~Ds18b20Configuration();
					void init(uint8_t* address, TemperatureSensorTarget target);

					uint8_t* getAddress();
					TemperatureSensorTarget getTarget();
				};
			}
		}
	}
}