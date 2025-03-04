#include "PwmConfiguration.h"

void Communication::Models::Configurations::PwmConfiguration::init()
{
}

void Communication::Models::Configurations::PwmConfiguration::init(uint8_t tachoPin, uint8_t pwmPin, String name,
	int8_t setToMaxWhenChillerLoad, LinkedList<Communication::Models::Configurations::PwmValueConfiguration*>* pwmValues, PwmType type)
{
	_tachoPin = tachoPin;
	_pwmPin = pwmPin;
	_name = name;
	_setToMaxWhenChillerLoad = setToMaxWhenChillerLoad;
	_pwmValues = pwmValues;
	_type = type;
}

uint8_t Communication::Models::Configurations::PwmConfiguration::getTachoPin()
{
	return _tachoPin;
}

uint8_t Communication::Models::Configurations::PwmConfiguration::getPwmPin()
{
	return _pwmPin;
}

String Communication::Models::Configurations::PwmConfiguration::getName()
{
	return _name;
}

int8_t Communication::Models::Configurations::PwmConfiguration::getSetToMaxWhenChillerLoad()
{
	return _setToMaxWhenChillerLoad;
}

LinkedList<Communication::Models::Configurations::PwmValueConfiguration*>* Communication::Models::Configurations::PwmConfiguration::getPwmValues()
{
	return _pwmValues;
}

PwmType Communication::Models::Configurations::PwmConfiguration::getPwmType()
{
	return _type;
}
