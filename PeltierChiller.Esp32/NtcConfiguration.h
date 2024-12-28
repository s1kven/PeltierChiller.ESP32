#pragma once
#ifndef _NtcConfiguration_
#define _NtcConfiguration_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "BaseDeserializableObject.h"
#include "BaseSerializableObject.h"
#include "TemperatureSensorTarget.cpp"
#include "JsonHelper.h"

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
					public Abstractions::BaseDeserializableObject,
					public Abstractions::BaseSerializableObject
				{

				private:
					const uint16_t _payloadSize = JSON_OBJECT_SIZE(8);
					uint16_t _address;
					String _name;
					TemperatureSensorTarget _target;
					uint32_t _resistance;
					uint32_t _resistanceNtc;
					uint32_t _bCoefficient;
					float _baseTemperature;
					float _supplyVoltage;

				protected:
					void init() override;

				public:
					~NtcConfiguration();
					void init(uint16_t address, String name, TemperatureSensorTarget target, uint32_t resistance, uint32_t resistanceNtc, 
						uint32_t bCoefficient, float baseTemperature, float supplyVoltage);

					uint16_t getAddress();
					String getName();
					TemperatureSensorTarget getTarget();
					uint32_t getResistance();
					uint32_t getResistanceNtc();
					uint32_t getBCoefficient();
					float getBaseTemperature();
					float getSupplyVoltage();

					DynamicJsonDocument createPayload() override;
				};
			}
		}
	}
}
#endif