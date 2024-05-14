#pragma once

#include <LinkedList.h>
#include "BaseSerializableObject.h"
#include "BaseSensor.h"
#include "KeyValuePair.h"

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
	};
}

