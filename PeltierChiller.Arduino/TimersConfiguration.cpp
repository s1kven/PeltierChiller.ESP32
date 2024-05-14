#include "TimersConfiguration.h"

void Communication::Models::Configurations::TimersConfiguration::init()
{
}

void Communication::Models::Configurations::TimersConfiguration::init(uint32_t buttonMinPressTime, uint32_t communicationDelay,
	uint32_t temperatureSensorsRequestDelay)
{
	_buttonMinPressTime = buttonMinPressTime;
	_communicationDelay = communicationDelay;
	_temperatureSensorsRequestDelay = temperatureSensorsRequestDelay;
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
