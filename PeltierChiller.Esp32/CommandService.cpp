#pragma once

#include "CommandService.h"

void Services::CommandService::handleCommand(Models::Abstractions::BaseCommand* command)
{
	command->invoke();
}
