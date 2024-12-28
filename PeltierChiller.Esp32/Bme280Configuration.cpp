#include "Bme280Configuration.h"

void Communication::Models::Configurations::TemperatureSensors::Bme280Configuration::init()
{
}

Communication::Models::Configurations::TemperatureSensors::Bme280Configuration::~Bme280Configuration()
{
	_name.clear();
}

void Communication::Models::Configurations::TemperatureSensors::Bme280Configuration::init(uint16_t address, String name, TemperatureSensorTarget target)
{
	_address = address;
	_name = name;
	_target = target;
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize + JSON_STRING_SIZE(_name.length()));
}

uint16_t Communication::Models::Configurations::TemperatureSensors::Bme280Configuration::getAddress()
{
	return _address;
}

TemperatureSensorTarget Communication::Models::Configurations::TemperatureSensors::Bme280Configuration::getTarget()
{
	return _target;
}

String Communication::Models::Configurations::TemperatureSensors::Bme280Configuration::getName()
{
	return _name;
}

DynamicJsonDocument Communication::Models::Configurations::TemperatureSensors::Bme280Configuration::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());
	JsonObject payload = document.to<JsonObject>();

	payload["Address"] = _address;
	payload["Name"] = _name;
	payload["Target"] = static_cast<uint16_t>(_target);

	return document;
}
