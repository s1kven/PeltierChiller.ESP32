#pragma once
#include "ChillerConfiguration.h"
#include "FileService.h"
#include "JsonService.h"

namespace Services
{
	class ConfigurationService
	{
	private:
		const char* _configPath = "/Settings.json";

		FileService* _fileService;
		JsonService* _jsonService;

	public:
		ConfigurationService(FileService* fileService, JsonService* jsonService);

		Communication::Models::ChillerConfiguration* getConfiguration();
	};
}