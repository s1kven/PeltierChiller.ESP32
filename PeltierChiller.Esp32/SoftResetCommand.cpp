#include "SoftResetCommand.h"

void Commands::SoftResetCommand::init()
{
}

Commands::SoftResetCommand::SoftResetCommand() 
	: Models::Abstractions::BaseCommand(Communication::Enums::RequestType::softReset)
{
}

void Commands::SoftResetCommand::invoke()
{
	ESP.restart();
}

void Commands::SoftResetCommand::clear()
{
}
