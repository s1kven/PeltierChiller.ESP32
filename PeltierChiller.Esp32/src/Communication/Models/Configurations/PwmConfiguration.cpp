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
	uint16_t itemsJsonSize = JSON_ARRAY_SIZE(_pwmValues->size());
	for (int i = 0; i < _pwmValues->size(); i++)
	{
		itemsJsonSize += _pwmValues->get(i)->getJsonSize();
	}
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize 
		+ itemsJsonSize + JSON_STRING_SIZE(_name.length()));
}

void Communication::Models::Configurations::PwmConfiguration::clear()
{
	for (int i = 0; i < _pwmValues->size(); i++)
	{
		delete _pwmValues->get(i);
	}
	_pwmValues->clear();
	delete _pwmValues;
	_name.clear();
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

DynamicJsonDocument Communication::Models::Configurations::PwmConfiguration::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());

	document["Tacho"] = _tachoPin;
	document["PWM"] = _pwmPin;
	document["Name"] = _name;
	document["SetToMaxWhenChillerLoad"] = _setToMaxWhenChillerLoad;

	JsonArray valueConfiguration = document.createNestedArray("Value");
	for (int i = 0; i < _pwmValues->size(); i++)
	{
		valueConfiguration.add((_pwmValues->get(i))->createPayload());
	}

	document["ControlType"] = static_cast<uint16_t>(_type);

	return document;
}
