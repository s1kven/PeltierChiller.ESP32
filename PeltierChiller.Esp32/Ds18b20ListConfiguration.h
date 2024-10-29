#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <LinkedList.h>
#include "BaseDeserializableObject.h"
#include "Ds18b20Configuration.h"

namespace Communication
{
	namespace Models
	{
		namespace Configurations
		{
			namespace TemperatureSensors
			{
				class Ds18b20ListConfiguration :
					public Abstractions::BaseDeserializableObject
				{
				private:
					uint8_t _pin;
					uint8_t _temperaturePrecision;
					LinkedList<Ds18b20Configuration*>* _items;

				protected:
					void init() override;

				public:
					~Ds18b20ListConfiguration();
					void init(uint8_t pin, uint8_t temperaturePrecision, LinkedList<Ds18b20Configuration*>* items);

					LinkedList<Ds18b20Configuration*>* getItems();
					uint8_t getPin();
					uint8_t getTemperaturePrecision();

				};
			}
		}
	}
}

