#include "SoftResetCommand.h"

void Communication::Models::SoftResetCommand::init()
{
}

void Communication::Models::SoftResetCommand::invoke()
{
	ESP.restart();
}