#pragma once
#ifndef _NtcListConfiguration_
#define _NtcListConfiguration_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <LinkedList.h>
#include "Communication/Abstractions/BaseDeserializableObject.h"
#include "Communication/Abstractions/BaseSerializableObject.h"
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
					public Abstractions::BaseDeserializableObject,
					public Abstractions::BaseSerializableObject
				{
				private:
					const uint16_t _payloadSize = JSON_OBJECT_SIZE(2);
					uint8_t _adcResolution;
					LinkedList<NtcConfiguration*>* _items;

				protected:
					void init() override;

				public:
					void init(uint8_t adcResolution, LinkedList<NtcConfiguration*>* items);
					void clear();

					LinkedList<NtcConfiguration*>* getItems();
					uint8_t getAdcResolution();

					DynamicJsonDocument createPayload() override;
				};
			}
		}
	}
}
#endif

