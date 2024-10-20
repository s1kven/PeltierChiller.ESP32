#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "BaseDeserializableObject.h"
#include "PinsConfiguration.h"
#include "TimersConfiguration.h"
#include "ChillerConfiguration.h"
#include "TemperatureSensorsConfiguration.h"
#include "PwmsConfiguration.h"

namespace Communication
{
	namespace Models
	{
		namespace Configurations
		{
			class Configuration :
				public Abstractions::BaseDeserializableObject
			{
			private:
				float _targetCircuitTemperature;
				float _pcVoltageThreshold;

				Communication::Models::Configurations::PinsConfiguration* _pinsConfiguration;
				Communication::Models::Configurations::TimersConfiguration* _timersConfiguration;
				Communication::Models::Configurations::ChillerConfiguration* _chillerConfiguration;
				Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* _temperatureSensorsConfiguration;
				Communication::Models::Configurations::PwmsConfiguration* _pwmsConfiguration;

			protected:
				void init() override;

			public:
				~Configuration();
				void init(float targetCircuitTemperature, float pcVoltageThreshold,
					Communication::Models::Configurations::PinsConfiguration* pinsConfiguration,
					Communication::Models::Configurations::TimersConfiguration* timersConfiguration,
					Communication::Models::Configurations::ChillerConfiguration* chillerConfiguration,
					Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* temperatureSensorsConfiguration,
					Communication::Models::Configurations::PwmsConfiguration* pwmsConfiguration);

				float getTargetCircuitTemperature();
				float getPcVoltageThreshold();
				Communication::Models::Configurations::PinsConfiguration* getPinsConfiguration();
				Communication::Models::Configurations::TimersConfiguration* getTimersConfiguration();
				Communication::Models::Configurations::ChillerConfiguration* getChillerConfiguration();
				Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* getTemperatureSensorsConfiguration();
				Communication::Models::Configurations::PwmsConfiguration* getPwmsConfiguration();
			};
		}
	}
}