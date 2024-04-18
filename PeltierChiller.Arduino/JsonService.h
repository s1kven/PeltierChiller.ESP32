#pragma once

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include "BaseJsonModel.h"
#include "KeyValuePair.h"
#include "ResponseType.cpp"

namespace Services
{
	class JsonService
	{
	private:
		uint32_t _serialWriteTimer = 0;
		uint32_t _serialWriteDelay = 0;

		void buildResponseBasedOnType(JsonObject& _data,
			Models::Abstractions::KeyValuePair<Models::Abstractions::BaseJsonModel**, uint8_t> _models, 
			Models::Enums::ResponseType _responseType);

		void buildTemperatureSensorsResponse(JsonObject& _data,
			Models::Abstractions::KeyValuePair<Models::Abstractions::BaseJsonModel**, uint8_t> _models);

	public:
		JsonService(uint32_t _serialWriteDelay);

		void serializeAndSendToSerialPort(
			Models::Abstractions::KeyValuePair<Models::Abstractions::BaseJsonModel**, uint8_t> _models, Models::Enums::ResponseType _responseType);

	};
}

