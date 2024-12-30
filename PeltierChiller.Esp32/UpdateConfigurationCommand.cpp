#include "JsonService.h"
#include "ConfigurationService.h"
#include "UpdateConfigurationCommand.h"

Commands::UpdateConfigurationCommand::UpdateConfigurationCommand(Communication::Models::Configurations::Configuration* configuration)
	: Models::Abstractions::BaseCommand(Communication::Enums::RequestType::updateConfiguration)
{
	_configuration = new Communication::Models::Configurations::Configuration(*configuration);
	delete configuration;
}

void Commands::UpdateConfigurationCommand::init()
{
}

void Commands::UpdateConfigurationCommand::invoke()
{
	const char* configPath = _configurationService->getConfigPath();

	String configurationJson = _jsonService->serializeRequest(_configuration, Communication::Enums::RequestType::configuration);

	uint32_t bytesWrite = _fileService->writeFile(configPath, configurationJson);

	Communication::Models::Responses::Response* updateConfigResponse;
	if (configurationJson.length() + 2 == bytesWrite) // Success config updated (2 is perhaps /n)
	{
		updateConfigResponse = new Communication::Models::Responses::Response(
			Communication::Enums::ResponseType::updateConfiguration, true, nullptr, "");
	}
	else
	{
		updateConfigResponse = new Communication::Models::Responses::Response(
			Communication::Enums::ResponseType::updateConfiguration, false,
			nullptr, _failedWriteConfigMessage + configurationJson);
	}

	_communicationService->sendResponse(updateConfigResponse);
}

void Commands::UpdateConfigurationCommand::clear()
{
	_configuration->clear();
	delete _configuration;
}
