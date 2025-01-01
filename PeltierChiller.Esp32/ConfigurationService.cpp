#include "ConfigurationService.h"

Services::ConfigurationService::ConfigurationService()
{
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
		return new Communication::Models::Responses::Response(Communication::Enums::ResponseType::errorConfiguration, errorRequest->getMessage());
	}
	else if(configurationRequest != nullptr)
	{
		_currentConfiguration = static_cast<Communication::Models::Configurations::Configuration*>(configurationRequest->getConfiguration());
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
	if (_currentTempConfiguration != nullptr)
	{
		_currentTempConfiguration->clear();
		delete _currentTempConfiguration;
	}

	_currentTempConfiguration = newConfiguration;
	initConfiguration(_currentTempConfiguration);
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
	Services::TemperatureService* oldTemperatureService = _temperatureService;
	Services::PwmService* oldPwmService = _pwmService;
	Services::ChillerService* oldChillerService = _chillerService;

	_communicationDelay = configuration->getTimersConfiguration()->getCommunicationDelay();

	_temperatureService = new Services::TemperatureService(configuration->getTemperatureSensorsConfiguration());

	_pwmService = new Services::PwmService(
		configuration->getTimersConfiguration()->getUpdatePwmDelay(),
		configuration->getPwmsConfiguration());

	_chillerService = new Services::ChillerService(configuration);

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

	_isChangeConfiguration = false;
}
