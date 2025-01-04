#include "PinsConfiguration.h"

void Communication::Models::Configurations::PinsConfiguration::init(uint8_t powerButton, uint8_t powerSignal,
	uint8_t chillerPsSignal, uint8_t chillerSignal, uint8_t pcVoltage)
{
	_powerButton = powerButton;
	_powerSignal = powerSignal;
	_chillerPsSignal = chillerPsSignal;
	_chillerSignal = chillerSignal;
	_pcVoltage = pcVoltage;
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize);
}

uint8_t Communication::Models::Configurations::PinsConfiguration::getPowerButton()
{
	return _powerButton;
}

uint8_t Communication::Models::Configurations::PinsConfiguration::getPowerSignal()
{
	return _powerSignal;
}

uint8_t Communication::Models::Configurations::PinsConfiguration::getChillerPsSignal()
{
	return _chillerPsSignal;
}

uint8_t Communication::Models::Configurations::PinsConfiguration::getChillerSignal()
{
	return _chillerSignal;
}

uint8_t Communication::Models::Configurations::PinsConfiguration::getPcVoltage()
{
	return _pcVoltage;
}

DynamicJsonDocument Communication::Models::Configurations::PinsConfiguration::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());

	document["PowerButton"] = _powerButton;
	document["PowerSignal"] = _powerSignal;
	document["ChillerPsSignal"] = _chillerPsSignal;
	document["ChillerSignal"] = _chillerSignal;
	document["PcVoltage"] = _pcVoltage;

	return document;
}

void Communication::Models::Configurations::PinsConfiguration::init()
{
}
