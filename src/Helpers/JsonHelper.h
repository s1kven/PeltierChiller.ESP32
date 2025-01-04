#pragma once
#ifndef _JsonHelper_
#define _JsonHelper_ 

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

namespace Helpers
{
	class JsonHelper
	{
	public:

		static uint16_t getFloatJsonSizeWorkaround(uint16_t objectsCount);
	};
}
#endif

