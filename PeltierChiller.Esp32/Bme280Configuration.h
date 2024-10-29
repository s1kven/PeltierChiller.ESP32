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
				class Bme280Configuration :
					public Abstractions::BaseDeserializableObject
				{

				private:
					uint16_t _address;
					TemperatureSensorTarget _target;

				protected:
					void init() override;

				public:
					void init(uint16_t address, TemperatureSensorTarget target);

					uint16_t getAddress();
					TemperatureSensorTarget getTarget();
				};
			}
		}
	}
}