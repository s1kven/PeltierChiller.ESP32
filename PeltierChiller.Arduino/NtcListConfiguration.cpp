#include "NtcListConfiguration.h"

void Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration::init()
{
}

Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration::~NtcListConfiguration()
{
	delete [] _items;
}

void Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration::init(uint8_t adcResolution, 
	LinkedList<NtcConfiguration*>* items)
{
	_adcResolution = adcResolution;
	_items = items;
}

LinkedList<Communication::Models::Configurations::TemperatureSensors::NtcConfiguration*>*
Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration::getItems()
{
	return _items;
}

uint8_t Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration::getAdcResolution()
{
	return _adcResolution;
}
