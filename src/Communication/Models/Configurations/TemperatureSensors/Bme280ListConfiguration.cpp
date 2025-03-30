#include "Bme280ListConfiguration.h"

void Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration::init()
{
}

void Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration::init(LinkedList<Bme280Configuration*>* items)
{
	_items = items;
	uint16_t itemsJsonSize = JSON_ARRAY_SIZE(_items->size());
	for (int i = 0; i < _items->size(); i++)
	{
		itemsJsonSize += _items->get(i)->getJsonSize();
	}
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize + itemsJsonSize);
}

void Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration::clear()
{
	for (int i = 0; i < _items->size(); i++)
	{
		delete _items->get(i);
	}
	_items->clear();
	delete _items;
}

LinkedList<Communication::Models::Configurations::TemperatureSensors::Bme280Configuration*>* 
Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration::getItems()
{
	return _items;
}

DynamicJsonDocument Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());

	JsonArray itemsConfiguration = document.createNestedArray("Items");
	for (int i = 0; i < _items->size(); i++)
	{
		itemsConfiguration.add((_items->get(i))->createPayload());
	}
	return document;
}
