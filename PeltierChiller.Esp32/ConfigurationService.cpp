#include "ConfigurationService.h"

const char* Services::ConfigurationService::getConfigPath()
{
	return _configPath;
}

Communication::Abstractions::BaseError* Services::ConfigurationService::readConfigurationFromSd()
{
	String content = _fileService->readFile(_configPath);

	Communication::Abstractions::BaseDeserializableObject* deserializedObject = _jsonService->deserializeRequest(content);
	Communication::Models::Errors::DeserializationError* error = dynamic_cast<Communication::Models::Errors::DeserializationError*>(deserializedObject);
	if (error)
	{
		return error;
	}
	else
	{
		_currentConfiguration = static_cast<Communication::Models::Configurations::Configuration*>(deserializedObject);
		return validateConfiguration(_currentConfiguration, content);
	}
}

Communication::Models::Configurations::Configuration* Services::ConfigurationService::getConfiguration()
{
	return _currentConfiguration;
}

void Services::ConfigurationService::changeConfiguration(Communication::Models::Configurations::Configuration* newConfiguration)
{
	_currentConfiguration->clear();
	delete _currentConfiguration;
}

String Services::ConfigurationService::getJsonFromConfiguration(Communication::Models::Configurations::Configuration* configuration)
{
	return String();
}

Communication::Abstractions::BaseError* 
	Services::ConfigurationService::validateConfiguration(Communication::Models::Configurations::Configuration* configuration, String content)
{
	if (configuration->getTemperatureSensorsConfiguration()->getBmeListConfiguration()->getItems()->size() == 0
		&& configuration->getTemperatureSensorsConfiguration()->getDs18b20ListConfiguration()->getItems()->size() == 0
		&& configuration->getTemperatureSensorsConfiguration()->getNtcListConfiguration()->getItems()->size() == 0)
	{
		return new Communication::Models::Errors::ConfigurationError(Communication::Enums::ErrorCode::emptySensors, nullptr, content);
	}
	if (!isSensorsAvailable(configuration, Models::Enums::TemperatureSensorTarget::coldCircuit))
	{
		return new Communication::Models::Errors::ConfigurationError(Communication::Enums::ErrorCode::emptyColdCircuitSensors, nullptr, content);
	}
	if (configuration->getChillerType() == Models::Enums::ChillerType::deltaTemperature)
	{
		if (!isSensorsAvailable(configuration, Models::Enums::TemperatureSensorTarget::room))
		{
			return new Communication::Models::Errors::ConfigurationError(Communication::Enums::ErrorCode::emptyRoomSensors, nullptr, content);
		}
	}
	if (configuration->getChillerType() == Models::Enums::ChillerType::dewPointTemperature 
		&& (configuration->getTemperatureSensorsConfiguration()->getBmeListConfiguration()->getItems()->size() == 0 
		   || !anyBmeTargetToRoom(configuration->getTemperatureSensorsConfiguration()->getBmeListConfiguration())))
	{
		return new Communication::Models::Errors::ConfigurationError(Communication::Enums::ErrorCode::invalidBmeConfiguration, nullptr, content);
	}

	return new Communication::Models::Errors::DeserializationError(Communication::Enums::ErrorCode::ok, nullptr, content);
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
