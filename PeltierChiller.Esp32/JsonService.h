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
#include "PwmValueConfiguration.h"
#include "ChillerType.cpp"
#include "ErrorRequest.h"
#include "ConfigurationRequest.h"
#include "SoftResetCommand.h"
#include "UpdateConfigurationCommand.h"

namespace Services
{
	class JsonService
	{
	private:
		Communication::Models::Requests::BaseRequest* deserializeRequestByType(
			Communication::Enums::RequestType _requestType, JsonObject data, String request);

		Communication::Models::Requests::ConfigurationRequest* deserializeConfigurationRequest(JsonObject data);
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

		Communication::Models::Requests::Commands::SoftResetCommand* deserializeSoftResetCommand();
		Communication::Models::Requests::Commands::UpdateConfigurationCommand* deserializeUpdateConfigurationCommand(JsonObject data);

	public:

		String serializeObject(Communication::Abstractions::BaseSerializableObject* response);
		String serializeRequest(Communication::Abstractions::BaseSerializableObject* request, Communication::Enums::RequestType requestType);
		Communication::Models::Requests::BaseRequest* deserializeRequest(String content);
	};
}
#endif

