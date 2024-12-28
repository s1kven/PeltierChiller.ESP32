#pragma once
#ifndef _ConfigurationService_
#define _ConfigurationService_ 

#include "Configuration.h"
#include "FileService.h"
#include "JsonService.h"
#include "BaseError.h"
#include "DeserializationError.h"
#include "ConfigurationError.h"
#include "ChillerType.cpp"
#include "TemperatureSensorTarget.cpp"
#include "Bme280Configuration.h"
#include "Ds18b20Configuration.h"
#include "NtcConfiguration.h"

namespace Services
{
	class JsonService;
}

extern Services::JsonService* _jsonService;
extern Services::FileService* _fileService;

namespace Services
{
	class ConfigurationService
	{
	private:
		const char* _configPath = "/Configuration.json";

		Communication::Models::Configurations::Configuration* _currentConfiguration;

		Communication::Abstractions::BaseError* validateConfiguration(Communication::Models::Configurations::Configuration* configuration, String content);
		bool isSensorsAvailable(Communication::Models::Configurations::Configuration* configuration, Models::Enums::TemperatureSensorTarget target);
		bool anyBmeTargetToRoom(Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration* bme280ListConfiguration);
	public:

		const char* getConfigPath();

		Communication::Abstractions::BaseError* readConfigurationFromSd();

		Communication::Models::Configurations::Configuration* getConfiguration();

		void changeConfiguration(Communication::Models::Configurations::Configuration* newConfiguration);

		String getJsonFromConfiguration(Communication::Models::Configurations::Configuration* configuration);
	};
}
#endif