#pragma once
#ifndef _TimersConfiguration_
#define _TimersConfiguration_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "Communication/Abstractions/BaseDeserializableObject.h"
#include "Communication/Abstractions/BaseSerializableObject.h"

namespace Communication
{
	namespace Models
	{
		namespace Configurations 
		{
			class TimersConfiguration :
				public Abstractions::BaseDeserializableObject,
				public Abstractions::BaseSerializableObject
			{
			private:
				const uint16_t _payloadSize = JSON_OBJECT_SIZE(4);
				uint32_t _buttonMinPressTime;
				uint32_t _communicationDelay;
				uint32_t _temperatureSensorsRequestDelay;
				uint32_t _updatePwmDelay;

			protected:
				void init() override;

			public:
				void init(uint32_t buttonMinPressTime, uint32_t communicationDelay, 
					uint32_t temperatureSensorsRequestDelay, uint32_t updatePwmDelay);

				uint32_t getButtonMinPressTime();
				uint32_t getCommunicationDelay();
				uint32_t getTemperatureSensorsRequestDelay();
				uint32_t getUpdatePwmDelay();

				DynamicJsonDocument createPayload() override;
			};
		}
	}
}
#endif