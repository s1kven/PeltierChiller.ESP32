#pragma once

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

namespace Models::Abstractions
{
	class BaseJsonModel
	{
	public:
		virtual DynamicJsonDocument createPayload() = 0;
	};
}
 