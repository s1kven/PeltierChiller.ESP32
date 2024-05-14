#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "BaseDeserializableObject.h"

namespace Communication
{
	namespace Models
	{
		namespace Configurations 
		{
			class TimersConfiguration :
				public Abstractions::BaseDeserializableObject
			{
			private:
				uint32_t _buttonMinPressTime;
				uint32_t _communicationDelay;
				uint32_t _temperatureSensorsRequestDelay;

			protected:
				void init() override;

			public:
				void init(uint32_t buttonMinPressTime, uint32_t communicationDelay, uint32_t temperatureSensorsRequestDelay);

				uint32_t getButtonMinPressTime();
				uint32_t getCommunicationDelay();
				uint32_t getTemperatureSensorsRequestDelay();
			};
		}
	}
}