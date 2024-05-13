#pragma once

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

namespace Communication
{
	namespace Abstractions
	{
		class BaseSerializableObject
		{
		private:
			uint16_t _jsonSize;

		public:
			BaseSerializableObject(uint16_t jsonSize);

			virtual DynamicJsonDocument createPayload() = 0;
			uint16_t GetJsonSize();
		};
	}
}
 