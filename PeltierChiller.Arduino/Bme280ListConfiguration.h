#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <LinkedList.h>
#include "BaseDeserializableObject.h"
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
					public Abstractions::BaseDeserializableObject
				{
				private:
					LinkedList<Bme280Configuration*>* _items;

				protected:
					void init() override;

				public:
					~Bme280ListConfiguration();
					void init(LinkedList<Bme280Configuration*>* items);

					LinkedList<Bme280Configuration*>* getItems();
				};
			}
		}
	}
}

