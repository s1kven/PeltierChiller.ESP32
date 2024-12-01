#include "BaseError.h"

Communication::Abstractions::BaseError::BaseError(Communication::Enums::ErrorCode code, const char* errorMessage)
{
	_code = code;
	_errorMessage = errorMessage;
	_responseType = Communication::Enums::ResponseType::errorConfiguration;
	init();
}

void Communication::Abstractions::BaseError::init()
{
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize);
}

DynamicJsonDocument Communication::Abstractions::BaseError::createPayload()
{
	DynamicJsonDocument document(_payloadSize);
	JsonObject payload = document.to<JsonObject>();
	payload["ResponseType"] = static_cast<uint16_t>(_responseType);
	JsonObject data = payload.createNestedObject("Data");
	data["Code"] = static_cast<uint16_t>(getCode());
	data["Message"] = static_cast<const char*>(getErrorMessage());
	return document;
}

Communication::Abstractions::BaseError::operator bool() const
{
	return _code != Communication::Enums::ErrorCode::ok;
}

Communication::Enums::ErrorCode Communication::Abstractions::BaseError::getCode()
{
	return _code;
}

const char* Communication::Abstractions::BaseError::getErrorMessage()
{
	return _errorMessage;
}
