#pragma once
#ifndef _Ds18b20ListConfiguration_
#define _Ds18b20ListConfiguration_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <LinkedList.h>
#include "Communication/Abstractions/BaseDeserializableObject.h"
#include "Communication/Abstractions/BaseSerializableObject.h"
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
					public Abstractions::BaseDeserializableObject,
					public Abstractions::BaseSerializableObject
				{
				private:
					const uint16_t _payloadSize = JSON_OBJECT_SIZE(3);
					uint8_t _pin;
					uint8_t _temperaturePrecision;
					LinkedList<Ds18b20Configuration*>* _items;

				protected:
					void init() override;

				public:
					void init(uint8_t pin, uint8_t temperaturePrecision, LinkedList<Ds18b20Configuration*>* items);
					void clear();

					LinkedList<Ds18b20Configuration*>* getItems();
					uint8_t getPin();
					uint8_t getTemperaturePrecision();

					DynamicJsonDocument createPayload() override;
				};
			}
		}
	}
}
#endif

