#pragma once
#ifndef _PwmValueConfiguration_
#define _PwmValueConfiguration_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "Communication/Abstractions/BaseDeserializableObject.h"
#include "Communication/Abstractions/BaseSerializableObject.h"
#include "Helpers/JsonHelper.h"

namespace Communication
{
	namespace Models
	{
		namespace Configurations
		{
			class PwmValueConfiguration :
				public Abstractions::BaseDeserializableObject,
				public Abstractions::BaseSerializableObject
			{
			private:
				const uint16_t _payloadSize = JSON_OBJECT_SIZE(2);
				uint8_t _load; // 0-100%
				float _temperature;

			protected:
				void init() override;

			public:

				PwmValueConfiguration(float temperature, uint8_t load);
				float getTemperature();
				uint8_t getLoad();

				DynamicJsonDocument createPayload() override;
			};
		}
	}
}
#endif