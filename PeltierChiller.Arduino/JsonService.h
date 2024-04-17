#pragma once

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include "BaseJsonModel.h"
#include "KeyValuePair.h"

namespace Services
{
	class JsonService
	{
	private:
		uint32_t _serialWriteTimer = 0;
		uint32_t _serialWriteDelay = 0;

	public:
		JsonService(uint32_t _serialWriteDelay);

		void serializeAndSendToSerialPort(Models::Abstractions::KeyValuePair<Models::Abstractions::BaseJsonModel**, uint8_t> _models);

	};
}

