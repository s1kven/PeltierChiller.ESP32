#pragma once

#include "Configuration.h"
#include "FileService.h"
#include "JsonService.h"
#include "BaseError.h"
#include "DeserializationError.h"

namespace Services
{
	class ConfigurationService
	{
	private:
		const char* _configPath = "/Configuration.json";

		FileService* _fileService;
		JsonService* _jsonService;

		Communication::Models::Configurations::Configuration* currentConfiguration;

	public:
		ConfigurationService(FileService* fileService, JsonService* jsonService);

		Communication::Abstractions::BaseError* readConfigurationFromSd();

		Communication::Models::Configurations::Configuration* getConfiguration();
	};
}