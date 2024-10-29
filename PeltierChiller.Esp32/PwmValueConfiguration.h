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
			class PwmValueConfiguration :
				public Abstractions::BaseDeserializableObject
			{
			private:

				uint8_t _load; // 0-100%
				float _temperature;

			protected:
				void init() override;

			public:

				PwmValueConfiguration(float temperature, uint8_t load);
				float getTemperature();
				uint8_t getLoad();
			};
		}
	}
}