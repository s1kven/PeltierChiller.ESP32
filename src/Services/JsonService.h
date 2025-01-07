#pragma once
#ifndef _JsonService_
#define _JsonService_ 

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <DallasTemperature.h>
#include "Communication/Models/Configurations/PinsConfiguration.h"
#include "Communication/Models/Configurations/TimersConfiguration.h"
#include "Communication/Models/Configurations/ChillerConfiguration.h"
#include "Communication/Models/Configurations/TemperatureSensors/TemperatureSensorsConfiguration.h"
#include "Communication/Models/Configurations/PwmsConfiguration.h"
#include "Communication/Models/Configurations/WifiConfiguration.h"
#include "Communication/Models/Configurations/Configuration.h"
#include "Communication/Abstractions/BaseSerializableObject.h"
#include "Communication/Models/Configurations/PwmValueConfiguration.h"
#include "Models/Enums/ChillerType.cpp"
#include "Communication/Models/Requests/ErrorRequest.h"
#include "Communication/Models/Requests/ConfigurationRequest.h"
#include "Communication/Models/Requests/Commands/SoftResetCommand.h"
#include "Communication/Models/Requests/Commands/UpdateConfigurationCommand.h"
#include "Communication/Models/Requests/Commands/UpdateTempConfigurationCommand.h"
#include "Communication/Models/Requests/Commands/ResetTempConfigurationCommand.h"

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
		Communication::Models::Configurations::WifiConfiguration* deserializeWifiConfiguration(JsonObject data);

		Communication::Models::Requests::Commands::SoftResetCommand* deserializeSoftResetCommand();
		Communication::Models::Requests::Commands::UpdateConfigurationCommand* deserializeUpdateConfigurationCommand(JsonObject data, Communication::Enums::RequestType requestType);
		Communication::Models::Requests::Commands::UpdateTempConfigurationCommand* deserializeUpdateTempConfigurationCommand(JsonObject data);
		Communication::Models::Requests::Commands::ResetTempConfigurationCommand* deserializeResetTempConfigurationCommand();

	public:

		String serializeObject(Communication::Abstractions::BaseSerializableObject* response);
		String serializeRequest(Communication::Abstractions::BaseSerializableObject* request, Communication::Enums::RequestType requestType);
		Communication::Models::Requests::BaseRequest* deserializeRequest(String content);
	};
}
#endif

