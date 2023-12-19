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

void Models::Rele::setReleState(Models::Enums::ReleState _releState)
{
	_state = _releState;
}