#include "Ds18b20ListConfiguration.h"

void Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration::init()
{
}

void Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration::init(uint8_t pin,
	uint8_t temperaturePrecision, LinkedList<Ds18b20Configuration*>* items)
{
	_pin = pin;
	_temperaturePrecision = temperaturePrecision;
	_items = items;
	uint16_t itemsJsonSize = JSON_ARRAY_SIZE(_items->size());
	for (int i = 0; i < _items->size(); i++)
	{
		itemsJsonSize += _items->get(i)->getJsonSize();
	}
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize + itemsJsonSize);
}

void Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration::clear()
{
	for (int i = 0; i < _items->size(); i++)
	{
		delete _items->get(i);
	}
	_items->clear();
	delete _items;
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

DynamicJsonDocument Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());
	JsonObject payload = document.to<JsonObject>();

	payload["Pin"] = _pin;
	payload["TemperaturePrecision"] = _temperaturePrecision;

	JsonArray itemsConfiguration = payload.createNestedArray("Items");
	for (int i = 0; i < _items->size(); i++)
	{
		itemsConfiguration.add((_items->get(i))->createPayload());
	}
	return document;
}
