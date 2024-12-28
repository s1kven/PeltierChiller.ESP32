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
	if (configurationJson.length() + 2 == bytesWrite) // Success config updated (2 is perhaps /n)
	{
		_communicationService->sendData(
			_jsonService->serializeObject(Communication::Enums::ResponseType::updateConfiguration, true));
	}
	else 
	{
		Communication::Models::Errors::SdError* sdError =
			new Communication::Models::Errors::SdError(Communication::Enums::ErrorCode::failedWriteFile,
				_failedWriteConfigMessage, configurationJson);
		String errorJson = _jsonService->serializeObject(sdError);
		_communicationService->sendData(errorJson);

		errorJson.clear();
		delete sdError;
	}
}

void Commands::UpdateConfigurationCommand::clear()
{
	_configuration->clear();
	delete _configuration;
}
