#include "JsonService.h"
#include "ConfigurationService.h"
#include "UpdateTempConfigurationCommand.h"

Communication::Models::Requests::Commands::UpdateTempConfigurationCommand::UpdateTempConfigurationCommand(
	Communication::Models::Configurations::Configuration* configuration)
	: Communication::Models::Requests::BaseRequest(Communication::Enums::RequestType::updateTempConfiguration)
{
	_configuration = new Communication::Models::Configurations::Configuration(*configuration);
	delete configuration;
}

void Communication::Models::Requests::Commands::UpdateTempConfigurationCommand::init()
{
}

void Communication::Models::Requests::Commands::UpdateTempConfigurationCommand::invoke()
{
	_configurationService->changeConfiguration(_configuration);
	Communication::Models::Responses::Response* updateTempConfigResponse = new Communication::Models::Responses::Response(
		Communication::Enums::ResponseType::updateTempConfiguration, true, nullptr, "");

	_communicationService->sendResponse(updateTempConfigResponse);
}

void Communication::Models::Requests::Commands::UpdateTempConfigurationCommand::clear()
{

}