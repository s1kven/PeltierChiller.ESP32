#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "BaseDeserializableObject.h"
#include "Bme280ListConfiguration.h"
#include "NtcListConfiguration.h"
#include "Ds18b20ListConfiguration.h"

namespace Communication
{
	namespace Models
	{
		namespace Configurations
		{
			namespace TemperatureSensors
			{
				class TemperatureSensorsConfiguration :
					public Abstractions::BaseDeserializableObject
				{
				private:
					Bme280ListConfiguration* _bmeListConfiguration;
					NtcListConfiguration* _ntcListConfiguration;
					Ds18b20ListConfiguration* _ds18b20ListConfiguration;

				protected:
					void init() override;

				public:
					~TemperatureSensorsConfiguration();
					void init(Bme280ListConfiguration* bmeListConfiguration, 
						NtcListConfiguration* ntcListConfiguration, Ds18b20ListConfiguration* ds18b20ListConfiguration);

					Bme280ListConfiguration* getBmeListConfiguration();
					NtcListConfiguration* getNtcListConfiguration();
					Ds18b20ListConfiguration* getDs18b20ListConfiguration();
				};
			}
		}
	}
}

