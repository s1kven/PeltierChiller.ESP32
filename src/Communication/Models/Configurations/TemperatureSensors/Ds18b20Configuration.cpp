#include "Ds18b20Configuration.h"

void Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration::init()
{
}

Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration::~Ds18b20Configuration()
{
	_name.clear();
}

void Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration::init(uint8_t* address, String name,
	TemperatureSensorTarget target)
{
	_target = target;
	_name = name;
	for (int i = 0; i < 8; i++)
	{
		_address[i] = address[i];
	}
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize + JSON_STRING_SIZE(_name.length()));
}

uint8_t* Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration::getAddress()
{
	return _address;
}

String Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration::getName()
{
	return _name;
}

TemperatureSensorTarget Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration::getTarget()
{
	return _target;
}

DynamicJsonDocument Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());
	JsonObject payload = document.to<JsonObject>();

	JsonArray addressConfiguration = payload.createNestedArray("Address");
	for (int i = 0; i < sizeof(_address) / sizeof(_address[0]); i++)
	{
		addressConfiguration.add(_address[i]);
	}

	payload["Name"] = _name;
	payload["Target"] = static_cast<uint16_t>(_target);

	return document;
}
