#include "Bme280Configuration.h"

void Communication::Models::Configurations::TemperatureSensors::Bme280Configuration::init()
{
}

void Communication::Models::Configurations::TemperatureSensors::Bme280Configuration::init(uint16_t address, TemperatureSensorTarget target)
{
	_address = address;
	_target = target;
}

uint16_t Communication::Models::Configurations::TemperatureSensors::Bme280Configuration::getAddress()
{
	return _address;
}

TemperatureSensorTarget Communication::Models::Configurations::TemperatureSensors::Bme280Configuration::getTarget()
{
	return _target;
}
