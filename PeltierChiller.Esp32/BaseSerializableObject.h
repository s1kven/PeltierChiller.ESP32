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

		protected:
			void setJsonSize(uint16_t jsonSize);

		public:
			BaseSerializableObject();

			virtual DynamicJsonDocument createPayload() = 0;
			uint16_t GetJsonSize();
		};
	}
}
 