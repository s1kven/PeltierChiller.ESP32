#include "ConfigurationService.h"

Services::ConfigurationService::ConfigurationService(FileService* fileService, JsonService* jsonService)
{
	_fileService = fileService;
	_jsonService = jsonService;
}

Communication::Abstractions::BaseError* Services::ConfigurationService::readConfigurationFromSd()
{
	String content = _fileService->readFile(_configPath);

	Communication::Abstractions::BaseDeserializableObject* deserializedObject = _jsonService->deserializeRequest(content);
	Communication::Models::DeserializationError* error = dynamic_cast<Communication::Models::DeserializationError*>(deserializedObject);
	if (error)
	{
		return error;
	}
	else
	{
		currentConfiguration = static_cast<Communication::Models::Configurations::Configuration*>(deserializedObject);
		return new Communication::Models::DeserializationError(Communication::Enums::ErrorCode::ok, nullptr);
	}
}

Communication::Models::Configurations::Configuration* Services::ConfigurationService::getConfiguration()
{
	return currentConfiguration;
}
