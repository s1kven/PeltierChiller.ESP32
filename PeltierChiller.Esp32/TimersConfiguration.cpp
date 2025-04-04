#include "TimersConfiguration.h"

void Communication::Models::Configurations::TimersConfiguration::init()
{
}

void Communication::Models::Configurations::TimersConfiguration::init(uint32_t buttonMinPressTime, uint32_t communicationDelay,
	uint32_t temperatureSensorsRequestDelay, uint32_t updatePwmDelay)
{
	_buttonMinPressTime = buttonMinPressTime;
	_communicationDelay = communicationDelay;
	_temperatureSensorsRequestDelay = temperatureSensorsRequestDelay;
	_updatePwmDelay = updatePwmDelay;
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize);
}

uint32_t Communication::Models::Configurations::TimersConfiguration::getButtonMinPressTime()
{
	return _buttonMinPressTime;
}

uint32_t Communication::Models::Configurations::TimersConfiguration::getCommunicationDelay()
{
	return _communicationDelay;
}

uint32_t Communication::Models::Configurations::TimersConfiguration::getTemperatureSensorsRequestDelay()
{
	return _temperatureSensorsRequestDelay;
}

uint32_t Communication::Models::Configurations::TimersConfiguration::getUpdatePwmDelay()
{
	return _updatePwmDelay;
}

DynamicJsonDocument Communication::Models::Configurations::TimersConfiguration::createPayload()
{
	DynamicJsonDocument document(_payloadSize);

	document["ButtonMinPressTime"] = _buttonMinPressTime;
	document["CommunicationDelay"] = _communicationDelay;
	document["TemperatureSensorsRequestDelay"] = _temperatureSensorsRequestDelay;
	document["UpdatePwmDelay"] = _updatePwmDelay;

	return document;
}