#include "PinsConfiguration.h"

void Communication::Models::Configurations::PinsConfiguration::init(uint8_t powerButton, uint8_t powerSignal,
	uint8_t chillerPsSignal, uint8_t chillerSignal, uint8_t pcVoltage)
{
	_powerButton = powerButton;
	_powerSignal = powerSignal;
	_chillerPsSignal = chillerPsSignal;
	_chillerSignal = chillerSignal;
	_pcVoltage = pcVoltage;
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

void Communication::Models::Configurations::PinsConfiguration::init()
{
}
