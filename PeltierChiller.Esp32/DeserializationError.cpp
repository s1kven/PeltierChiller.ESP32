#include "DeserializationError.h"

Communication::Models::Errors::DeserializationError::DeserializationError(Communication::Enums::ErrorCode code, 
	const char* errorMessage, String invalidPayload) :
	Communication::Abstractions::BaseError(Communication::Enums::ResponseType::errorDeserialization, 
		code, errorMessage, invalidPayload)
{
}

void Communication::Models::Errors::DeserializationError::init()
{
}

DynamicJsonDocument Communication::Models::Errors::DeserializationError::createPayload()
{
	return Communication::Abstractions::BaseError::createPayload();
}
