#include "PwmItem.h"

Models::PwmItem::PwmItem(uint8_t tachoPin, uint8_t pwmPin, String name, LinkedList<Models::PwmValue*>* values, Models::Enums::PwmType controlType)
{
	_tachoPin = tachoPin;
	_pwmPin = pwmPin;
	_name = name;
	_values = values;
	_controlType = controlType;
	init();
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

LinkedList<Models::PwmValue*>* Models::PwmItem::getValues()
{
	return _values;
}

Models::Enums::PwmType Models::PwmItem::getControlType()
{
	return _controlType;
}

uint16_t Models::PwmItem::getRpm(uint16_t getRpmDelay)
{
	float errorCorrection = ((float)getRpmDelay / (float)(millis() - _rpmTimer));
	float multiplier = 30.0 * (1000.0 / (float)getRpmDelay);
	_rpm = _count * multiplier * errorCorrection;
	_count = 0;
	_rpmTimer = millis();
	return _rpm;
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
