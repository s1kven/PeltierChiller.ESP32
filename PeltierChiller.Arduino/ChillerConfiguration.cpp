#include "ChillerConfiguration.h"

Communication::Models::ChillerConfiguration::ChillerConfiguration(float targetCircuitTemperature)
{
	_targetCircuitTemperature = targetCircuitTemperature;
}

float Communication::Models::ChillerConfiguration::getTargetCircuitTemperature()
{
	return _targetCircuitTemperature;
}

void Communication::Models::ChillerConfiguration::init()
{
}
