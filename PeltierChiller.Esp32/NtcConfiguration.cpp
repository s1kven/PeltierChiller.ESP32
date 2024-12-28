#include "NtcConfiguration.h"

void Communication::Models::Configurations::TemperatureSensors::NtcConfiguration::init()
{
}

Communication::Models::Configurations::TemperatureSensors::NtcConfiguration::~NtcConfiguration()
{
	_name.clear();
}

void Communication::Models::Configurations::TemperatureSensors::NtcConfiguration::init(uint16_t address, String name, TemperatureSensorTarget target,
	uint32_t resistance, uint32_t resistanceNtc, uint32_t bCoefficient, float baseTemperature, float supplyVoltage)
{
	_address = address;
	_name = name;
	_target = target;
	_resistance = resistance;
	_resistanceNtc = resistanceNtc;
	_bCoefficient = bCoefficient;
	_baseTemperature = baseTemperature;
	_supplyVoltage = supplyVoltage;
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize + JSON_STRING_SIZE(_name.length())
		+ Helpers::JsonHelper::getFloatJsonSizeWorkaround(1));
}

uint16_t Communication::Models::Configurations::TemperatureSensors::NtcConfiguration::getAddress()
{
	return _address;
}

String Communication::Models::Configurations::TemperatureSensors::NtcConfiguration::getName()
{
	return _name;
}

TemperatureSensorTarget Communication::Models::Configurations::TemperatureSensors::NtcConfiguration::getTarget()
{
	return _target;
}

uint32_t Communication::Models::Configurations::TemperatureSensors::NtcConfiguration::getResistance()
{
	return _resistance;
}

uint32_t Communication::Models::Configurations::TemperatureSensors::NtcConfiguration::getResistanceNtc()
{
	return _resistanceNtc;
}

uint32_t Communication::Models::Configurations::TemperatureSensors::NtcConfiguration::getBCoefficient()
{
	return _bCoefficient;
}

float Communication::Models::Configurations::TemperatureSensors::NtcConfiguration::getBaseTemperature()
{
	return _baseTemperature;
}

float Communication::Models::Configurations::TemperatureSensors::NtcConfiguration::getSupplyVoltage()
{
	return _supplyVoltage;
}

DynamicJsonDocument Communication::Models::Configurations::TemperatureSensors::NtcConfiguration::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());
	JsonObject payload = document.to<JsonObject>();

	payload["Address"] = _address;
	payload["Name"] = _name;
	payload["Target"] = static_cast<uint16_t>(_target);
	payload["Resistance"] = _resistance;
	payload["ResistanceNTC"] = _resistanceNtc;
	payload["BCoefficient"] = _bCoefficient;
	payload["BaseTemperature"] = _baseTemperature;
	payload["SupplyVoltage"] = serialized(String(_supplyVoltage, 1));


	return document;
}
