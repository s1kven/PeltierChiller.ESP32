#pragma once
#ifndef _JsonHelper_
#define _JsonHelper_ 

#include <LinkedList.h>
#include "BaseSerializableObject.h"
#include "BaseSensor.h"
#include "KeyValuePair.h"
#include "PwmItem.h"

using namespace Communication::Abstractions;
using namespace Models::Abstractions;
using namespace Models::TemperatureSensors;

namespace Helpers
{
	class JsonHelper
	{
	public:

		static KeyValuePair<BaseSerializableObject**, uint8_t> convertToBaseSerializableObjectArray(
			LinkedList<Models::TemperatureSensors::BaseSensor*>* temperatureSensors);
		static KeyValuePair<BaseSerializableObject**, uint8_t> convertToBaseSerializableObjectArray(
			LinkedList<Models::PwmItem*>* pwmItems);
		static uint16_t getFloatJsonSizeWorkaround(uint16_t objectsCount);
	};
}
#endif

