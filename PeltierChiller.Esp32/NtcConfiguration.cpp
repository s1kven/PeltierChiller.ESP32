#include "NtcConfiguration.h"

void Communication::Models::Configurations::TemperatureSensors::NtcConfiguration::init()
{
}

void Communication::Models::Configurations::TemperatureSensors::NtcConfiguration::init(uint16_t address, TemperatureSensorTarget target,
	uint32_t resistance, uint32_t resistanceNtc, uint32_t bCoefficient, float baseTemperature, float supplyVoltage)
{
	_address = address;
	_target = target;
	_resistance = resistance;
	_resistanceNtc = resistanceNtc;
	_bCoefficient = bCoefficient;
	_baseTemperature = baseTemperature;
	_supplyVoltage = supplyVoltage;
}

uint16_t Communication::Models::Configurations::TemperatureSensors::NtcConfiguration::getAddress()
{
	return _address;
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
