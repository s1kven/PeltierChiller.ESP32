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
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize +
		+ Helpers::JsonHelper::getFloatJsonSizeWorkaround(7));
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

DynamicJsonDocument Communication::Models::Configurations::ChillerConfiguration::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());

	document["PotentiometerAddress"] = _potentiometerAddress;
	document["MaxPotentiometerValue"] = _maxPotentiometerValue;
	document["MinPotentiometerValue"] = _minPotentiometerValue;
	document["Kp"] = serialized(String(_kp, 1));
	document["Ki"] = serialized(String(_ki, 1));
	document["Kd"] = serialized(String(_kd, 1));
	document["Dt"] = serialized(String(_dt, 1));
	document["PidRatio"] = serialized(String(_pidRatio, 1));
	document["MinIntegral"] = serialized(String(_minIntegral, 1));
	document["MaxIntegral"] = serialized(String(_maxIntegral, 1));
	document["ComputePidDelay"] = _computePidDelay;

	return document;
}
