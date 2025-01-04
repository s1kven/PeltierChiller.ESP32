#include "Services/ConfigurationService.h"
#include "ResetTempConfigurationCommand.h"

Communication::Models::Requests::Commands::ResetTempConfigurationCommand::ResetTempConfigurationCommand()
	: Communication::Models::Requests::BaseRequest(Communication::Enums::RequestType::resetTempConfiguration)
{
}

void Communication::Models::Requests::Commands::ResetTempConfigurationCommand::init()
{
}

void Communication::Models::Requests::Commands::ResetTempConfigurationCommand::invoke()
{
	_configurationService->resetTempConfiguration();
	Communication::Models::Responses::Response* resetTempConfigResponse = new Communication::Models::Responses::Response(
		Communication::Enums::ResponseType::resetTempConfiguration, true, nullptr, "");

	_communicationService->sendResponse(resetTempConfigResponse);
}

void Communication::Models::Requests::Commands::ResetTempConfigurationCommand::clear()
{
}
