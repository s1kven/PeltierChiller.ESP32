#pragma once
#include "Configuration.h"
#include "FileService.h"
#include "JsonService.h"

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

		void readConfigurationFromSd();

		Communication::Models::Configurations::Configuration* getConfiguration();
	};
}