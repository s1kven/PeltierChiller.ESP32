#include "SoftResetCommand.h"

void Communication::Models::Requests::Commands::SoftResetCommand::init()
{
}

Communication::Models::Requests::Commands::SoftResetCommand::SoftResetCommand()
	: Communication::Models::Requests::BaseRequest(Communication::Enums::RequestType::softReset)
{
}

void Communication::Models::Requests::Commands::SoftResetCommand::invoke()
{
	ESP.restart();
}

void Communication::Models::Requests::Commands::SoftResetCommand::clear()
{
}
