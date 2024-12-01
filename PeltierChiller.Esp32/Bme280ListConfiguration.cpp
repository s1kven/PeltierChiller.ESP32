#include "Bme280ListConfiguration.h"

void Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration::init()
{
}

Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration::~Bme280ListConfiguration()
{
	delete [] _items;
}

void Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration::init(LinkedList<Bme280Configuration*>* items)
{
	_items = items;
}

LinkedList<Communication::Models::Configurations::TemperatureSensors::Bme280Configuration*>* 
Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration::getItems()
{
	return _items;
}
