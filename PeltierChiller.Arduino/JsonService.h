#pragma once

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include "ChillerConfiguration.h"
#include "BaseJsonModel.h"
#include "KeyValuePair.h"
#include "ResponseType.cpp"
#include "RequestType.cpp"

namespace Services
{
	class JsonService
	{
	private:
		const uint16_t _baseResponseSize = 64; // DynamicJsonDocument(48) + responsePayload(8) + data(responsePayload)

		uint32_t _serialWriteTimer = 0;
		uint32_t _serialWriteDelay = 0;

		uint16_t calculateJsonDocumentSize(Models::Abstractions::KeyValuePair<Models::Abstractions::BaseJsonModel**, uint8_t> _models);

		void buildResponseBasedOnType(JsonObject& _data,
			Models::Abstractions::KeyValuePair<Models::Abstractions::BaseJsonModel**, uint8_t> _models, 
			Communication::Enums::ResponseType _responseType);

		void buildTemperatureSensorsResponse(JsonObject& _data,
			Models::Abstractions::KeyValuePair<Models::Abstractions::BaseJsonModel**, uint8_t> _models);
		Communication::Abstractions::BaseDeserializableObject* deserializeRequestByType(
			Communication::Enums::RequestType _requestType, JsonObject data);
		Communication::Models::ChillerConfiguration* DeserializeChillerConfiguration(JsonObject data);

	public:
		JsonService(uint32_t _serialWriteDelay);

		void serializeAndSendToSerialPort(
			Models::Abstractions::KeyValuePair<Models::Abstractions::BaseJsonModel**, uint8_t> _models, Communication::Enums::ResponseType _responseType);
		Communication::Abstractions::BaseDeserializableObject* deserializeRequest(String& content);
	};
}

