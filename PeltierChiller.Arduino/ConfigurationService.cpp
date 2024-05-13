#include "ConfigurationService.h"

Services::ConfigurationService::ConfigurationService(FileService* fileService, JsonService* jsonService)
{
	_fileService = fileService;
	_jsonService = jsonService;
}

Communication::Models::ChillerConfiguration* Services::ConfigurationService::getConfiguration()
{
	String content = _fileService->readFile(_configPath);

	Communication::Abstractions::BaseDeserializableObject* deserializedObject = _jsonService->deserializeRequest(content);
	Communication::Models::ChillerConfiguration* chillerConfiguration =
		static_cast<Communication::Models::ChillerConfiguration*>(deserializedObject);

	return chillerConfiguration;
}
