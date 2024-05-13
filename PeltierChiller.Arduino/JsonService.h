#pragma once

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include "ChillerConfiguration.h"
#include "BaseSerializableObject.h"
#include "KeyValuePair.h"
#include "ResponseType.cpp"
#include "RequestType.cpp"

namespace Services
{
	class JsonService
	{
	private:
		const uint16_t _baseResponseSize = 64; // DynamicJsonDocument(48) + responsePayload(8) + data(responsePayload)

		uint16_t calculateJsonDocumentSize(
			Models::Abstractions::KeyValuePair<Communication::Abstractions::BaseSerializableObject**, uint8_t> _models);

		void buildResponseBasedOnType(JsonObject& _data,
			Models::Abstractions::KeyValuePair<Communication::Abstractions::BaseSerializableObject**, uint8_t> _models,
			Communication::Enums::ResponseType _responseType);

		void buildTemperatureSensorsResponse(JsonObject& _data,
			Models::Abstractions::KeyValuePair<Communication::Abstractions::BaseSerializableObject**, uint8_t> _models);
		Communication::Abstractions::BaseDeserializableObject* deserializeRequestByType(
			Communication::Enums::RequestType _requestType, JsonObject data);
		Communication::Models::ChillerConfiguration* DeserializeChillerConfiguration(JsonObject data);

	public:

		String serializeObject(
			Models::Abstractions::KeyValuePair<Communication::Abstractions::BaseSerializableObject**, uint8_t> _models, 
			Communication::Enums::ResponseType _responseType);
		Communication::Abstractions::BaseDeserializableObject* deserializeRequest(String& content);
	};
}

