#pragma once

#include "Rele.h"

Models::Rele::Rele()
{
}

Models::Rele::Rele(int8_t _relePin)
{
	_signalPin = _relePin;
	_state = Models::Enums::ReleState::off;
}

int8_t Models::Rele::getSignalPin()
{
	return _signalPin;
}

Models::Enums::ReleState Models::Rele::getReleState()
{
	return _state;
}

DynamicJsonDocument Models::Rele::createPayload()
{
	DynamicJsonDocument document(JSON_OBJECT_SIZE(2));
	JsonObject payload = document.to<JsonObject>();
	payload["Pin"] = _signalPin;
	payload["State"] = getReleState();
	return document;
}

void Models::Rele::setReleState(Models::Enums::ReleState _releState)
{
	_state = _releState;
}