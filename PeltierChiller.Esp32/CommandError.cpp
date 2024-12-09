#include "CommandError.h"

Communication::Models::Errors::CommandError::CommandError(String invalidCommand) :
Communication::Abstractions::BaseError(Communication::Enums::ResponseType::errorCommand, 
	Communication::Enums::ErrorCode::invalidCommand, "Failed to parse command.", invalidCommand)
{
}

void Communication::Models::Errors::CommandError::init()
{
}

DynamicJsonDocument Communication::Models::Errors::CommandError::createPayload()
{
	return Communication::Abstractions::BaseError::createPayload();
}
