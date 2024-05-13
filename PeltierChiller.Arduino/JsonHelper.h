#pragma once

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
		static KeyValuePair<BaseSerializableObject**, uint8_t> convertToBaseSerializableObjectArray(BaseSensor* _models[], uint8_t _arraySize);
	};
}

