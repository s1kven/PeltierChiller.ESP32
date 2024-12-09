#pragma once

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <DallasTemperature.h>
#include "PinsConfiguration.h"
#include "TimersConfiguration.h"
#include "ChillerConfiguration.h"
#include "TemperatureSensorsConfiguration.h"
#include "PwmsConfiguration.h"
#include "Configuration.h"
#include "BaseSerializableObject.h"
#include "KeyValuePair.h"
#include "RequestType.cpp"
#include "DeserializationError.h"
#include "PwmValueConfiguration.h"
#include "ChillerType.cpp"
#include "SoftResetCommand.h"

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
		void buildPwmsResponse(JsonObject& _data,
			Models::Abstractions::KeyValuePair<Communication::Abstractions::BaseSerializableObject**, uint8_t> _models);

		uint32_t getDeserializedJsonSize(String& content);

		Communication::Models::Errors::DeserializationError* buildError(DeserializationError error, String content);
		Communication::Enums::ErrorCode errorCodeConverter(DeserializationError error);

		Communication::Abstractions::BaseDeserializableObject* deserializeRequestByType(
			Communication::Enums::RequestType _requestType, JsonObject data, String request);

		Communication::Models::Configurations::Configuration* deserializeConfiguration(JsonObject data);
		Communication::Models::Configurations::PinsConfiguration* deserializePinsConfiguration(JsonObject data);
		Communication::Models::Configurations::TimersConfiguration* deserializeTimersConfiguration(JsonObject data);
		Communication::Models::Configurations::ChillerConfiguration* deserializeChillerConfiguration(JsonObject data);
		Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* 
			deserializeTemperatureSensorsConfiguration(JsonObject data);
		Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration*
			deserializeBme280ListConfiguration(JsonObject data);
		Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration*
			deserializeNtcListConfiguration(JsonObject data);
		Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration*
			deserializeDs18b20ListConfiguration(JsonObject data);
		Communication::Models::Configurations::PwmsConfiguration* deserializePwmsConfiguration(JsonArray items);

		Communication::Models::SoftResetCommand* deserializeSoftReset();

	public:

		String serializeObject(Communication::Abstractions::BaseSerializableObject* response);
		String serializeObject(
			Models::Abstractions::KeyValuePair<Communication::Abstractions::BaseSerializableObject**, uint8_t> _models, 
			Communication::Enums::ResponseType _responseType);
		Communication::Abstractions::BaseDeserializableObject* deserializeRequest(String& content);
		Models::Abstractions::BaseCommand* deserializeCommand(String& content);
	};
}

