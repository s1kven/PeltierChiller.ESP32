#include "DeserializationError.h"

Communication::Models::DeserializationError::DeserializationError(Communication::Enums::ErrorCode code, const char* errorMessage) :
	Communication::Abstractions::BaseError(code, errorMessage)
{

}

void Communication::Models::DeserializationError::init()
{
}

DynamicJsonDocument Communication::Models::DeserializationError::createPayload()
{
	return Communication::Abstractions::BaseError::createPayload();
}
