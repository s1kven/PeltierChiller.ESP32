#pragma once

#include "BaseJsonModel.h"
#include "BaseSensor.h"
#include "KeyValuePair.h"

using namespace Models::Abstractions;
using namespace Models::TemperatureSensors;

namespace Helpers
{
	class JsonHelper
	{
	public:
		static KeyValuePair<BaseJsonModel**, uint8_t> convertToBaseJsonModelArray(BaseSensor* _models[], uint8_t _arraySize);
	};
}

