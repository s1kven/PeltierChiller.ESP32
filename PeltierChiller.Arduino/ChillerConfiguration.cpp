#include "ChillerConfiguration.h"

void Communication::Models::Configurations::ChillerConfiguration::init()
{
}

void Communication::Models::Configurations::ChillerConfiguration::init(uint16_t potentiometerAddress,
	uint16_t maxPotentiometerValue, uint16_t minPotentiometerValue, float kp, float ki, float kd, float dt, float pidRatio, 
	float minIntegral, float maxIntegral, uint32_t computePidDelay)
{
	_potentiometerAddress = potentiometerAddress;
	_maxPotentiometerValue = maxPotentiometerValue;
	_minPotentiometerValue = minPotentiometerValue;
	_kp = kp;
	_ki = ki;
	_kd = kd;
	_dt = dt;
	_pidRatio = pidRatio;
	_minIntegral = minIntegral;
	_maxIntegral = maxIntegral;
	_computePidDelay = computePidDelay;
}

uint16_t Communication::Models::Configurations::ChillerConfiguration::getPotentiometerAddress()
{
	return _potentiometerAddress;
}

uint16_t Communication::Models::Configurations::ChillerConfiguration::getMaxPotentiometerValue()
{
	return _maxPotentiometerValue;
}

uint16_t Communication::Models::Configurations::ChillerConfiguration::getMinPotentiometerValue()
{
	return _minPotentiometerValue;
}

float Communication::Models::Configurations::ChillerConfiguration::getKp()
{
	return _kp;
}

float Communication::Models::Configurations::ChillerConfiguration::getKi()
{
	return _ki;
}

float Communication::Models::Configurations::ChillerConfiguration::getKd()
{
	return _kd;
}

float Communication::Models::Configurations::ChillerConfiguration::getDt()
{
	return _dt;
}

float Communication::Models::Configurations::ChillerConfiguration::getPidRatio()
{
	return _pidRatio;
}

float Communication::Models::Configurations::ChillerConfiguration::getMinIntegral()
{
	return _minIntegral;
}

float Communication::Models::Configurations::ChillerConfiguration::getMaxIntegral()
{
	return _maxIntegral;
}

uint32_t Communication::Models::Configurations::ChillerConfiguration::getComputePidDelay()
{
	return _computePidDelay;
}
