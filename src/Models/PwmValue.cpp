#include "PwmValue.h"

Models::PwmValue::PwmValue(float temperature, uint8_t load)
{
	_temperature = temperature;
	_load = load;
}

float Models::PwmValue::getTemperature()
{
	return _temperature;
}

uint8_t Models::PwmValue::getLoad()
{
	return _load;
}
