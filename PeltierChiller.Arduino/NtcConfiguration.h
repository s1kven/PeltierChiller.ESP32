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
				class NtcConfiguration :
					public Abstractions::BaseDeserializableObject
				{

				private:
					uint16_t _address;
					TemperatureSensorTarget _target;
					uint32_t _resistance;
					uint32_t _resistanceNtc;
					uint32_t _bCoefficient;
					float _baseTemperature;
					float _supplyVoltage;

				protected:
					void init() override;

				public:
					void init(uint16_t address, TemperatureSensorTarget target, uint32_t resistance, uint32_t resistanceNtc, 
						uint32_t bCoefficient, float baseTemperature, float supplyVoltage);

					uint16_t getAddress();
					TemperatureSensorTarget getTarget();
					uint32_t getResistance();
					uint32_t getResistanceNtc();
					uint32_t getBCoefficient();
					float getBaseTemperature();
					float getSupplyVoltage();
				};
			}
		}
	}
}