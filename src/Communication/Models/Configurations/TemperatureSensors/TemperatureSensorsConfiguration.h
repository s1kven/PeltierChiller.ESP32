#pragma once
#ifndef _TemperatureSensorsConfiguration_
#define _TemperatureSensorsConfiguration_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "Communication/Abstractions/BaseDeserializableObject.h"
#include "Communication/Abstractions/BaseSerializableObject.h"
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
					public Abstractions::BaseDeserializableObject,
					public Abstractions::BaseSerializableObject
				{
				private:
					const uint16_t _payloadSize = JSON_OBJECT_SIZE(3);
					Bme280ListConfiguration* _bmeListConfiguration;
					NtcListConfiguration* _ntcListConfiguration;
					Ds18b20ListConfiguration* _ds18b20ListConfiguration;

				protected:
					void init() override;

				public:
					void init(Bme280ListConfiguration* bmeListConfiguration, 
						NtcListConfiguration* ntcListConfiguration, Ds18b20ListConfiguration* ds18b20ListConfiguration);
					void clear();

					Bme280ListConfiguration* getBmeListConfiguration();
					NtcListConfiguration* getNtcListConfiguration();
					Ds18b20ListConfiguration* getDs18b20ListConfiguration();

					DynamicJsonDocument createPayload() override;
				};
			}
		}
	}
}
#endif

