#include "Ds18b20Configuration.h"

void Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration::init()
{
}

Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration::~Ds18b20Configuration()
{
	delete [] _address;
}

void Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration::init(uint8_t* address,
	TemperatureSensorTarget target)
{
	_target = target;
	for (int i = 0; i < 8; i++)
	{
		_address[i] = address[i];
	}
}

uint8_t* Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration::getAddress()
{
	return _address;
}

TemperatureSensorTarget Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration::getTarget()
{
	return _target;
}
