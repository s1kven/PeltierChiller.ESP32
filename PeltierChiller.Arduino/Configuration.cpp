#include "Configuration.h"

ChillerType Communication::Models::Configurations::Configuration::getChillerType()
{
	return _chillerType;
}

float Communication::Models::Configurations::Configuration::getTargetCircuitTemperature()
{
	return _targetCircuitTemperature;
}

float Communication::Models::Configurations::Configuration::getPcVoltageThreshold()
{
	return _pcVoltageThreshold;
}

bool Communication::Models::Configurations::Configuration::getIsDelayEnablingPc()
{
	return _isDelayEnablingPc;
}

Communication::Models::Configurations::PinsConfiguration* Communication::Models::Configurations::Configuration::getPinsConfiguration()
{
	return _pinsConfiguration;
}

Communication::Models::Configurations::TimersConfiguration* Communication::Models::Configurations::Configuration::getTimersConfiguration()
{
	return _timersConfiguration;
}

Communication::Models::Configurations::ChillerConfiguration* Communication::Models::Configurations::Configuration::getChillerConfiguration()
{
	return _chillerConfiguration;
}

Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* 
Communication::Models::Configurations::Configuration::getTemperatureSensorsConfiguration()
{
	return _temperatureSensorsConfiguration;
}

Communication::Models::Configurations::PwmsConfiguration* 
Communication::Models::Configurations::Configuration::getPwmsConfiguration()
{
	return _pwmsConfiguration;
}

void Communication::Models::Configurations::Configuration::init()
{
}

Communication::Models::Configurations::Configuration::~Configuration()
{
	delete _pinsConfiguration;
	delete _timersConfiguration;
	delete _chillerConfiguration;
	delete _temperatureSensorsConfiguration;
	delete _pwmsConfiguration;
}

void Communication::Models::Configurations::Configuration::init(ChillerType chillerType, float targetCircuitTemperature,
	float pcVoltageThreshold, bool isDelayEnablingPc, Communication::Models::Configurations::PinsConfiguration* pinsConfiguration,
	Communication::Models::Configurations::TimersConfiguration* timersConfiguration,
	Communication::Models::Configurations::ChillerConfiguration* chillerConfiguration,
	Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* temperatureSensorsConfiguration,
	Communication::Models::Configurations::PwmsConfiguration* pwmsConfiguration)
{
	_chillerType = chillerType;
	_targetCircuitTemperature = targetCircuitTemperature;
	_pcVoltageThreshold = pcVoltageThreshold;
	_isDelayEnablingPc = isDelayEnablingPc;
	_pinsConfiguration = pinsConfiguration;
	_timersConfiguration = timersConfiguration;
	_chillerConfiguration = chillerConfiguration;
	_temperatureSensorsConfiguration = temperatureSensorsConfiguration;
	_pwmsConfiguration = pwmsConfiguration;
}
