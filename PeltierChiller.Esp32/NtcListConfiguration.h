#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <LinkedList.h>
#include "BaseDeserializableObject.h"
#include "NtcConfiguration.h"

namespace Communication
{
	namespace Models
	{
		namespace Configurations
		{
			namespace TemperatureSensors
			{
				class NtcListConfiguration :
					public Abstractions::BaseDeserializableObject
				{
				private:
					uint8_t _adcResolution;
					LinkedList<NtcConfiguration*>* _items;

				protected:
					void init() override;

				public:
					~NtcListConfiguration();
					void init(uint8_t adcResolution, LinkedList<NtcConfiguration*>* items);

					LinkedList<NtcConfiguration*>* getItems();
					uint8_t getAdcResolution();
				};
			}
		}
	}
}

