#pragma once
#ifndef _ConfigurationService_
#define _ConfigurationService_ 

#include "Configuration.h"
#include "FileService.h"
#include "JsonService.h"
#include "ChillerType.cpp"
#include "TemperatureSensorTarget.cpp"
#include "Bme280Configuration.h"
#include "Ds18b20Configuration.h"
#include "NtcConfiguration.h"
#include "Response.h"
#include "ResponseType.cpp"
#include "ChillerService.h"
#include "PwmService.h"
#include "TemperatureService.h"

namespace Services
{
	class JsonService;
}

extern Services::JsonService* _jsonService;
extern Services::FileService* _fileService;
extern uint16_t _communicationDelay;
extern Services::TemperatureService* _temperatureService;
extern Services::ChillerService* _chillerService;
extern Services::PwmService* _pwmService;

namespace Services
{
	class ConfigurationService
	{
	private:
		const char* _configPath = "/Configuration.json";

		bool _isChangeConfiguration = false;

		String _emptySensorsListError = "Failed to configure chiller. Empty temperature sensors list! Add at least one sensor to cold circuit.";
		String _noColdCircuitSensorsError = "Failed to configure chiller. No temperature sensor was found for the cold circuit.";
		String _noRoomSensorsError = "Failed to configure chiller. No temperature sensor was found for the room. To use delta temperature chiller type (ChillerType: 2), you need to connect at least one room sensor.";
		String _noBmeSensorsError = "Failed to configure chiller. No BME280 temperature sensor was found for the room. To use dew point chiller type (ChillerType: 3), you need to connect at least one room BME280 sensor.";

		Communication::Models::Configurations::Configuration* _currentConfiguration;
		Communication::Models::Configurations::Configuration* _currentTempConfiguration;

		Communication::Models::Responses::Response* validateConfiguration(Communication::Models::Configurations::Configuration* configuration, String content);
		bool isSensorsAvailable(Communication::Models::Configurations::Configuration* configuration, Models::Enums::TemperatureSensorTarget target);
		bool anyBmeTargetToRoom(Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration* bme280ListConfiguration);
		void initConfiguration(Communication::Models::Configurations::Configuration* configuration);
	public:
		ConfigurationService();

		const char* getConfigPath();

		Communication::Models::Responses::Response* readConfigurationFromSd();

		Communication::Models::Configurations::Configuration* getConfiguration();

		void changeConfiguration(Communication::Models::Configurations::Configuration* newConfiguration);
		void resetTempConfiguration();

		void initConfiguration();

		bool isChangeConfiguration();
	};
}
#endif