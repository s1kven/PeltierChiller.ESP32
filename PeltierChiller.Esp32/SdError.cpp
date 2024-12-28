#include "SdError.h"

Communication::Models::Errors::SdError::SdError(Communication::Enums::ErrorCode code, 
	const char* errorMessage, String invalidPayload) 
	: Communication::Abstractions::BaseError(Communication::Enums::ResponseType::errorSd,
		code, errorMessage, invalidPayload)
{
}

void Communication::Models::Errors::SdError::init()
{
}

DynamicJsonDocument Communication::Models::Errors::SdError::createPayload()
{
	return Communication::Abstractions::BaseError::createPayload();
}
