#include "BaseCommand.h"

Models::Abstractions::BaseCommand::BaseCommand(Communication::Enums::RequestType commandType)
{
	_commandType = commandType;
}

void Models::Abstractions::BaseCommand::init()
{
}

void Models::Abstractions::BaseCommand::test()
{
}

Communication::Enums::RequestType Models::Abstractions::BaseCommand::getCommandType()
{
	return _commandType;
}
