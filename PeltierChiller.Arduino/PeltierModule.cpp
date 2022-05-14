#include "PeltierModule.h"


Models::PeltierModule::PeltierModule(int8_t _peltierPin) :Rele(_peltierPin)
{
	Models::Rele::setReleState(Models::Enums::ReleState::ReleState::none);
}

void Models::PeltierModule::switchingOn()
{
	digitalWrite(getSignalPin(), HIGH);
	Models::Rele::setReleState(Models::Enums::ReleState::ReleState::on);
}

void Models::PeltierModule::switchingOff()
{
	digitalWrite(getSignalPin(), LOW);
	Models::Rele::setReleState(Models::Enums::ReleState::ReleState::off);
}
