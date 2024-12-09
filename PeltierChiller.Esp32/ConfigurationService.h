#pragma once

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
	class ConfigurationService
	{
	private:
		const char* _configPath = "/Configuration.json";

		FileService* _fileService;
		JsonService* _jsonService;

		Communication::Models::Configurations::Configuration* currentConfiguration;

		Communication::Abstractions::BaseError* validateConfiguration(Communication::Models::Configurations::Configuration* configuration, String content);
		bool isSensorsAvailable(Communication::Models::Configurations::Configuration* configuration, Models::Enums::TemperatureSensorTarget target);
		bool anyBmeTargetToRoom(Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration* bme280ListConfiguration);
	public:
		ConfigurationService(FileService* fileService, JsonService* jsonService);

		Communication::Abstractions::BaseError* readConfigurationFromSd();

		Communication::Models::Configurations::Configuration* getConfiguration();
	};
}