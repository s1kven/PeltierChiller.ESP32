#include "PwmItem.h"

Models::PwmItem::PwmItem(uint8_t tachoPin, uint8_t pwmPin, String name, int8_t setToMaxWhenChillerLoad,
	LinkedList<Models::PwmValue*>* values, Models::Enums::PwmType controlType, uint32_t updatePwmDelay)
{
	_tachoPin = tachoPin;
	_pwmPin = pwmPin;
	_name = name;
	_setToMaxWhenChillerLoad = setToMaxWhenChillerLoad;
	_values = values;
	_controlType = controlType;
	_updatePwmDelay = updatePwmDelay;
	init();
	Communication::Abstractions::BaseSerializableObject::setJsonSize(
		_payloadSize + JSON_STRING_SIZE(_name.length()));
}

uint8_t Models::PwmItem::getTachoPin()
{
	return _tachoPin;
}

uint8_t Models::PwmItem::getPwmPin()
{
	return _pwmPin;
}

String Models::PwmItem::getName()
{
	return _name;
}

int8_t Models::PwmItem::getSetToMaxWhenChillerLoad()
{
	return _setToMaxWhenChillerLoad;
}

LinkedList<Models::PwmValue*>* Models::PwmItem::getValues()
{
	return _values;
}

Models::Enums::PwmType Models::PwmItem::getControlType()
{
	return _controlType;
}

uint16_t Models::PwmItem::getRpm()
{
	float errorCorrection = ((float)_updatePwmDelay / (float)(millis() - _rpmTimer));
	float multiplier = 30.0 * (1000.0 / (float)_updatePwmDelay);
	_rpm = _count * multiplier * errorCorrection;
	_count = 0;
	_rpmTimer = millis();
	return _rpm;
}

DynamicJsonDocument Models::PwmItem::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());
	JsonObject payload = document.to<JsonObject>();
	payload["ControlType"] = static_cast<uint16_t>(getControlType());
	payload["Name"] = getName();
	payload["RPM"] = getRpm();
	return document;
}

void Models::PwmItem::clear()
{
	for (int i = 0; i < _values->size(); i++)
	{
		delete _values->get(i);
	}
	_values->clear();
	delete _values;
}

void Models::PwmItem::init()
{
	pinMode(_pwmPin, OUTPUT);
	analogWrite(_pwmPin, 0);
	pinMode(_tachoPin, INPUT);
	digitalWrite(_tachoPin, HIGH);
	attachInterruptArg(digitalPinToInterrupt(_tachoPin), Models::PwmItem::interruptHandler, this, FALLING);
}

void Models::PwmItem::interruptHandler(void* ptr)
{
	Models::PwmItem* item = (Models::PwmItem*)ptr;
	item->counter();
}

void Models::PwmItem::counter()
{
	_count++;
}
