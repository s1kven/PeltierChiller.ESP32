#include "ConfigurationService.h"

Services::ConfigurationService::ConfigurationService(FileService* fileService, JsonService* jsonService)
{
	_fileService = fileService;
	_jsonService = jsonService;
}

void Services::ConfigurationService::readConfigurationFromSd()
{
	String content = _fileService->readFile(_configPath);

	Communication::Abstractions::BaseDeserializableObject* deserializedObject = _jsonService->deserializeRequest(content);
	currentConfiguration = static_cast<Communication::Models::Configurations::Configuration*>(deserializedObject);
}

Communication::Models::Configurations::Configuration* Services::ConfigurationService::getConfiguration()
{
	return currentConfiguration;
}
