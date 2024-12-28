#include "PwmValueConfiguration.h"

void Communication::Models::Configurations::PwmValueConfiguration::init()
{
}

Communication::Models::Configurations::PwmValueConfiguration::PwmValueConfiguration(float temperature, uint8_t load)
{
	_temperature = temperature;
	_load = load;
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize
		+ Helpers::JsonHelper::getFloatJsonSizeWorkaround(1));
}

float Communication::Models::Configurations::PwmValueConfiguration::getTemperature()
{
	return _temperature;
}

uint8_t Communication::Models::Configurations::PwmValueConfiguration::getLoad()
{
	return _load;
}

DynamicJsonDocument Communication::Models::Configurations::PwmValueConfiguration::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());

	document["Temperature"] = serialized(String(_temperature, 1));
	document["Load"] = _load;

	return document;
}
