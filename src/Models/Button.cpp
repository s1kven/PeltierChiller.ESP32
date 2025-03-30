#pragma once

#include "Button.h"

Models::Button::Button(uint8_t signalPin, String name)
{
	_signalPin = signalPin;
	_name = name;
	pinMode(_signalPin, INPUT_PULLUP);
}

uint8_t Models::Button::getSignalPin()
{
	return _signalPin;
}

String Models::Button::getName()
{
	return _name;
}

void Models::Button::setLastPressTime(uint32_t lastPressTime)
{
	_lastPressTime = lastPressTime;
}

uint32_t Models::Button::getLastPressTime()
{
	return _lastPressTime;
}

void Models::Button::setLastPressMillis(uint32_t lastPressMillis)
{
	_lastPressMillis = lastPressMillis;
}

uint32_t Models::Button::getLastPressMillis()
{
	return _lastPressMillis;
}

void Models::Button::setLastTime(uint32_t lastTime)
{
	_lastTime = lastTime;
}

uint32_t Models::Button::getLastTime()
{
	return _lastTime;
}

bool Models::Button::getState()
{
	return _pressed;
}

void Models::Button::setState(bool pressed)
{
	_pressed = pressed;
}

bool Models::Button::getFlag()
{
	return _flag;
}

void Models::Button::setFlag(bool flag)
{
	_flag = flag;
}
