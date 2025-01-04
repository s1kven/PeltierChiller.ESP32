#include "NtcListConfiguration.h"

void Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration::init()
{
}

void Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration::init(uint8_t adcResolution, 
	LinkedList<NtcConfiguration*>* items)
{
	_adcResolution = adcResolution;
	_items = items;
	uint16_t itemsJsonSize = JSON_ARRAY_SIZE(_items->size());
	for (int i = 0; i < _items->size(); i++)
	{
		itemsJsonSize += _items->get(i)->getJsonSize();
	}
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize + itemsJsonSize);
}

void Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration::clear()
{
	for (int i = 0; i < _items->size(); i++)
	{
		delete _items->get(i);
	}
	_items->clear();
	delete _items;
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

DynamicJsonDocument Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());

	document["AdcResolution"] = _adcResolution;

	JsonArray itemsConfiguration = document.createNestedArray("Items");
	for (int i = 0; i < _items->size(); i++)
	{
		itemsConfiguration.add((_items->get(i))->createPayload());
	}
	return document;
}
