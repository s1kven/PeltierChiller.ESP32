#pragma once
#ifndef _Bme280ListConfiguration_
#define _Bme280ListConfiguration_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <LinkedList.h>
#include "BaseDeserializableObject.h"
#include "BaseSerializableObject.h"
#include "Bme280Configuration.h"

namespace Communication
{
	namespace Models
	{
		namespace Configurations
		{
			namespace TemperatureSensors
			{
				class Bme280ListConfiguration :
					public Abstractions::BaseDeserializableObject,
					public Abstractions::BaseSerializableObject
				{
				private:
					const uint16_t _payloadSize = JSON_OBJECT_SIZE(1);
					LinkedList<Bme280Configuration*>* _items;

				protected:
					void init() override;

				public:
					void init(LinkedList<Bme280Configuration*>* items);
					void clear();

					LinkedList<Bme280Configuration*>* getItems();

					DynamicJsonDocument createPayload() override;
				};
			}
		}
	}
}
#endif

