#include "PwmValueConfiguration.h"

void Communication::Models::Configurations::PwmValueConfiguration::init()
{
}

Communication::Models::Configurations::PwmValueConfiguration::PwmValueConfiguration(float temperature, uint8_t load)
{
	_temperature = temperature;
	_load = load;
}

float Communication::Models::Configurations::PwmValueConfiguration::getTemperature()
{
	return _temperature;
}

uint8_t Communication::Models::Configurations::PwmValueConfiguration::getLoad()
{
	return _load;
}
