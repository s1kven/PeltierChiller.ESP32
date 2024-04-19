#pragma once

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

namespace Models::Abstractions
{
	class BaseJsonModel
	{
	private:
		uint16_t _jsonSize;

	public:
		BaseJsonModel(uint16_t jsonSize);

		virtual DynamicJsonDocument createPayload() = 0;
		uint16_t GetJsonSize();
	};
}
 