#pragma once
#ifndef _Configuration_
#define _Configuration_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "Communication/Abstractions/BaseDeserializableObject.h"
#include "Communication/Abstractions/BaseSerializableObject.h"
#include "PinsConfiguration.h"
#include "TimersConfiguration.h"
#include "ChillerConfiguration.h"
#include "TemperatureSensors/TemperatureSensorsConfiguration.h"
#include "PwmsConfiguration.h"
#include "Models/Enums/ChillerType.cpp"
#include "Helpers/JsonHelper.h"

namespace Communication
{
	namespace Models
	{
		namespace Configurations
		{
			class Configuration :
				public Abstractions::BaseDeserializableObject,
				public Abstractions::BaseSerializableObject
			{
			private:
				const uint16_t _payloadSize = JSON_OBJECT_SIZE(10) + JSON_ARRAY_SIZE(1);

				ChillerType _chillerType;
				float _targetCircuitTemperature;
				float _voltmeterThreshold;
				uint32_t _voltmeterR1;
				uint32_t _voltmeterR2;
				bool _isDelayEnablingPc;

				Communication::Models::Configurations::PinsConfiguration* _pinsConfiguration;
				Communication::Models::Configurations::TimersConfiguration* _timersConfiguration;
				Communication::Models::Configurations::ChillerConfiguration* _chillerConfiguration;
				Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* _temperatureSensorsConfiguration;
				Communication::Models::Configurations::PwmsConfiguration* _pwmsConfiguration;

			protected:
				void init() override;

			public:
				void init(ChillerType chillerType, float targetCircuitTemperature, float voltmeterThreshold,
					uint32_t voltmeterR1, uint32_t voltmeterR2, bool isDelayEnablingPc,
					Communication::Models::Configurations::PinsConfiguration* pinsConfiguration,
					Communication::Models::Configurations::TimersConfiguration* timersConfiguration,
					Communication::Models::Configurations::ChillerConfiguration* chillerConfiguration,
					Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* temperatureSensorsConfiguration,
					Communication::Models::Configurations::PwmsConfiguration* pwmsConfiguration);
				void clear();

				ChillerType getChillerType();
				float getTargetCircuitTemperature();
				float getVoltmeterThreshold();
				uint32_t getVoltmeterR1();
				uint32_t getVoltmeterR2();
				bool getIsDelayEnablingPc();
				Communication::Models::Configurations::PinsConfiguration* getPinsConfiguration();
				Communication::Models::Configurations::TimersConfiguration* getTimersConfiguration();
				Communication::Models::Configurations::ChillerConfiguration* getChillerConfiguration();
				Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* getTemperatureSensorsConfiguration();
				Communication::Models::Configurations::PwmsConfiguration* getPwmsConfiguration();

				DynamicJsonDocument createPayload() override;
			};
		}
	}
}
#endif