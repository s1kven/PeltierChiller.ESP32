#include "JsonService.h"
#include "ConfigurationService.h"
#include "UpdateConfigurationCommand.h"

Communication::Models::Requests::Commands::UpdateConfigurationCommand::UpdateConfigurationCommand(
	Communication::Models::Configurations::Configuration* configuration, Communication::Enums::RequestType request)
	: Communication::Models::Requests::BaseRequest(request)
{
	_configuration = new Communication::Models::Configurations::Configuration(*configuration);
	delete configuration;
}

void Communication::Models::Requests::Commands::UpdateConfigurationCommand::init()
{
}

void Communication::Models::Requests::Commands::UpdateConfigurationCommand::invoke()
{
	const char* configPath = _configurationService->getConfigPath();

	String configurationJson = _jsonService->serializeRequest(_configuration, Communication::Enums::RequestType::configuration);

	uint32_t bytesWrite = _fileService->writeFile(configPath, configurationJson);

	Communication::Models::Responses::Response* updateConfigResponse;
	Communication::Enums::ResponseType responseType = 
		static_cast<Communication::Enums::ResponseType>(Communication::Models::Requests::BaseRequest::getCommandType());
	if (configurationJson.length() + 2 == bytesWrite) // Success config updated (2 is perhaps /n)
	{
		if (Communication::Models::Requests::BaseRequest::getCommandType() == Communication::Enums::RequestType::updateAndApplyConfiguration)
		{
			Communication::Models::Responses::Response* readConfigResponse = _configurationService->readConfigurationFromSd();
			if (!readConfigResponse->getSuccess())
			{
				updateConfigResponse = new Communication::Models::Responses::Response(
					responseType, false,
					nullptr, _failedResetConfigFromSdMessage + configurationJson);
				_communicationService->sendResponse(updateConfigResponse);
				readConfigResponse->clear();
				delete readConfigResponse;
				return;
			}
			readConfigResponse->clear();
			delete readConfigResponse;
			_configurationService->resetTempConfiguration();
		}

		updateConfigResponse = new Communication::Models::Responses::Response(
			responseType, true, nullptr, "");
	}
	else
	{
		updateConfigResponse = new Communication::Models::Responses::Response(
			responseType, false,
			nullptr, _failedWriteConfigMessage + configurationJson);
	}

	_communicationService->sendResponse(updateConfigResponse);
}

void Communication::Models::Requests::Commands::UpdateConfigurationCommand::clear()
{
	_configuration->clear();
	delete _configuration;
}
