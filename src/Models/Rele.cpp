#pragma once

#include "Rele.h"

Models::Rele::Rele()
{
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize);
}

Models::Rele::Rele(int8_t _relePin)
{
	_signalPin = _relePin;
	_state = Models::Enums::ReleState::off;
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize);
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
	DynamicJsonDocument document(_payloadSize);
	JsonObject payload = document.to<JsonObject>();
	payload["Pin"] = _signalPin;
	payload["State"] = static_cast<uint16_t>(getReleState());
	return document;
}

void Models::Rele::setReleState(Models::Enums::ReleState _releState)
{
	_state = _releState;
}