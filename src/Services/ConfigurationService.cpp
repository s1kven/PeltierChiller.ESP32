#include "ConfigurationService.h"

Services::ConfigurationService::ConfigurationService()
{
	_currentConfiguration = nullptr;
	_currentTempConfiguration = nullptr;
}

const char* Services::ConfigurationService::getConfigPath()
{
	return _configPath;
}

Communication::Models::Responses::Response* Services::ConfigurationService::readConfigurationFromSd()
{
	String content = _fileService->readFile(_configPath);

	Communication::Models::Requests::BaseRequest* request = _jsonService->deserializeRequest(content);
	Communication::Models::Requests::ErrorRequest* errorRequest = dynamic_cast<Communication::Models::Requests::ErrorRequest*>(request);
	Communication::Models::Requests::ConfigurationRequest* configurationRequest = dynamic_cast<Communication::Models::Requests::ConfigurationRequest*>(request);
	if (errorRequest != nullptr)
	{
		String errorMessage = errorRequest->getMessage();
		errorRequest->clear();
		delete errorRequest;
		return new Communication::Models::Responses::Response(Communication::Enums::ResponseType::errorConfiguration, errorMessage);
	}
	else if (configurationRequest != nullptr)
	{
		clearConfiguration(_currentConfiguration);
		_currentConfiguration = static_cast<Communication::Models::Configurations::Configuration*>(configurationRequest->getConfiguration());
		configurationRequest->clear();
		delete configurationRequest;
		return validateConfiguration(_currentConfiguration, content);
	}
	return new Communication::Models::Responses::Response(Communication::Enums::ResponseType::errorConfiguration, "");
}

Communication::Models::Configurations::Configuration* Services::ConfigurationService::getConfiguration()
{
	return _currentConfiguration;
}

void Services::ConfigurationService::changeConfiguration(Communication::Models::Configurations::Configuration* newConfiguration)
{
	_isChangeConfiguration = true;
	clearConfiguration(_currentTempConfiguration);

	_currentTempConfiguration = newConfiguration;
	initConfiguration(_currentTempConfiguration);
}

void Services::ConfigurationService::resetTempConfiguration()
{
	_isChangeConfiguration = true;
	initConfiguration();
}

void Services::ConfigurationService::initConfiguration()
{
	initConfiguration(_currentConfiguration);
}

bool Services::ConfigurationService::isChangeConfiguration()
{
	return _isChangeConfiguration;
}

Communication::Models::Responses::Response*
	Services::ConfigurationService::validateConfiguration(Communication::Models::Configurations::Configuration* configuration, String content)
{
	if (configuration->getTemperatureSensorsConfiguration()->getBmeListConfiguration()->getItems()->size() == 0
		&& configuration->getTemperatureSensorsConfiguration()->getDs18b20ListConfiguration()->getItems()->size() == 0
		&& configuration->getTemperatureSensorsConfiguration()->getNtcListConfiguration()->getItems()->size() == 0)
	{
		return new Communication::Models::Responses::Response(Communication::Enums::ResponseType::errorConfiguration, _emptySensorsListError);
	}
	if (!isSensorsAvailable(configuration, Models::Enums::TemperatureSensorTarget::coldCircuit))
	{
		return new Communication::Models::Responses::Response(Communication::Enums::ResponseType::errorConfiguration, _noColdCircuitSensorsError);
	}
	if (configuration->getChillerType() == Models::Enums::ChillerType::deltaTemperature)
	{
		if (!isSensorsAvailable(configuration, Models::Enums::TemperatureSensorTarget::room))
		{
			return new Communication::Models::Responses::Response(Communication::Enums::ResponseType::errorConfiguration, _noRoomSensorsError);
		}
	}
	if (configuration->getChillerType() == Models::Enums::ChillerType::dewPointTemperature 
		&& (configuration->getTemperatureSensorsConfiguration()->getBmeListConfiguration()->getItems()->size() == 0 
		   || !anyBmeTargetToRoom(configuration->getTemperatureSensorsConfiguration()->getBmeListConfiguration())))
	{
		return new Communication::Models::Responses::Response(Communication::Enums::ResponseType::errorConfiguration, _noBmeSensorsError);
	}

	return new Communication::Models::Responses::Response(Communication::Enums::ResponseType::errorConfiguration, true, nullptr, "");
}

bool Services::ConfigurationService::isSensorsAvailable(Communication::Models::Configurations::Configuration* configuration,
	Models::Enums::TemperatureSensorTarget target)
{
	LinkedList<Communication::Models::Configurations::TemperatureSensors::Bme280Configuration*>* bmeList =
		(configuration->getTemperatureSensorsConfiguration()->getBmeListConfiguration()->getItems());
	LinkedList<Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration*>* ds18b20List =
		(configuration->getTemperatureSensorsConfiguration()->getDs18b20ListConfiguration()->getItems());
	LinkedList<Communication::Models::Configurations::TemperatureSensors::NtcConfiguration*>* ntcList =
		(configuration->getTemperatureSensorsConfiguration()->getNtcListConfiguration()->getItems());

	for (int i = 0; i < bmeList->size(); i++)
	{
		if (bmeList->get(i)->getTarget() == target)
		{
			return true;
		}
	}

	for (int i = 0; i < ds18b20List->size(); i++)
	{
		if (ds18b20List->get(i)->getTarget() == target)
		{
			return true;
		}
	}

	for (int i = 0; i < ntcList->size(); i++)
	{
		if (ntcList->get(i)->getTarget() == target)
		{
			return true;
		}
	}

	return false;
}

bool Services::ConfigurationService::anyBmeTargetToRoom(Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration* bme280ListConfiguration)
{
	LinkedList<Communication::Models::Configurations::TemperatureSensors::Bme280Configuration*>* bmeList =
		(bme280ListConfiguration->getItems());

	for (int i = 0; i < bmeList->size(); i++)
	{
		if (bmeList->get(i)->getTarget() == Models::Enums::TemperatureSensorTarget::room)
		{
			return true;
		}
	}

	return false;
}

void Services::ConfigurationService::initConfiguration(Communication::Models::Configurations::Configuration* configuration)
{
	if(_logService != nullptr)
	{
		_logService->pushContentToLog();
	}

	Services::TemperatureService* oldTemperatureService = _temperatureService;
	Services::PwmService* oldPwmService = _pwmService;
	Services::ChillerService* oldChillerService = _chillerService;
	Services::WifiService* oldWifiService = _wifiService;
	Services::TimeService* oldTimeService = _timeService;
	Services::LogService* oldLogService = _logService;

	_communicationDelay = configuration->getTimersConfiguration()->getCommunicationDelay();

	_temperatureService = new Services::TemperatureService(configuration->getTemperatureSensorsConfiguration());

	_pwmService = new Services::PwmService(
		configuration->getTimersConfiguration()->getUpdatePwmDelay(),
		configuration->getPwmsConfiguration());

	_chillerService = new Services::ChillerService(configuration);

	_wifiService = new Services::WifiService(configuration->getWifiConfiguration());

	_timeService = new Services::TimeService(configuration->getWifiConfiguration()->getNtpServer());

	_logService = new Services::LogService(configuration->getLogConfiguration(), configuration->getWifiConfiguration()->getReconnectionTimeout());

	if(_logService->getIsEnabled())
	{
		_logService->addToLogQueue(_jsonService->serializeObject(configuration));
	}

	if (oldTemperatureService != nullptr)
	{
		oldTemperatureService->clear();
		delete oldTemperatureService;
	}
	if (oldPwmService != nullptr)
	{
		oldPwmService->clear();
		delete oldPwmService;
	}
	if (oldChillerService != nullptr)
	{
		oldChillerService->clear();
		delete oldChillerService;
	}
	if (oldWifiService != nullptr)
	{
		delete oldWifiService;
	}
	if (oldTimeService != nullptr)
	{
		delete oldTimeService;
	}
	if (oldLogService != nullptr)
	{
		delete oldLogService;
	}

	_isChangeConfiguration = false;
	_initMillis = millis();
}

void Services::ConfigurationService::clearConfiguration(Communication::Models::Configurations::Configuration* configuration)
{
	if (configuration != nullptr)
	{
		configuration->clear();
		delete configuration;
	}
}
