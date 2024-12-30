#pragma once
#ifndef _JsonService_
#define _JsonService_ 

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
#include "DeserializationError.h"
#include "PwmValueConfiguration.h"
#include "ChillerType.cpp"
#include "SoftResetCommand.h"
#include "UpdateConfigurationCommand.h"

namespace Services
{
	class JsonService
	{
	private:
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

		Commands::SoftResetCommand* deserializeSoftResetCommand();
		Commands::UpdateConfigurationCommand* deserializeUpdateConfigurationCommand(JsonObject data);

	public:

		String serializeObject(Communication::Enums::ResponseType _responseType, bool _success);
		String serializeObject(Communication::Abstractions::BaseSerializableObject* response);
		String serializeRequest(Communication::Abstractions::BaseSerializableObject* request, Communication::Enums::RequestType requestType);
		Communication::Abstractions::BaseDeserializableObject* deserializeRequest(String& content);
		Models::Abstractions::BaseCommand* deserializeCommand(String& content);
	};
}
#endif

