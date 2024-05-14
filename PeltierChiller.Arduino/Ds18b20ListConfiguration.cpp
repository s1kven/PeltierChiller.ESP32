#include "Ds18b20ListConfiguration.h"

void Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration::init()
{
}

Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration::~Ds18b20ListConfiguration()
{
	delete [] _items;
}

void Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration::init(uint8_t pin,
	uint8_t temperaturePrecision, LinkedList<Ds18b20Configuration*>* items)
{
	_pin = pin;
	_temperaturePrecision = temperaturePrecision;
	_items = items;
}

LinkedList<Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration*>*
Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration::getItems()
{
	return _items;
}

uint8_t Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration::getPin()
{
	return _pin;
}

uint8_t Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration::getTemperaturePrecision()
{
	return _temperaturePrecision;
}
