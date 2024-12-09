#include "BaseError.h"

Communication::Abstractions::BaseError::BaseError(Communication::Enums::ResponseType errorType, 
	Communication::Enums::ErrorCode code, const char* errorMessage, String invalidRequest)
{
	_code = code;
	_errorMessage = errorMessage;
	_invalidRequest = invalidRequest;
	_responseType = errorType;
	init();
}

void Communication::Abstractions::BaseError::init()
{
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize);
}

DynamicJsonDocument Communication::Abstractions::BaseError::createPayload()
{
	DynamicJsonDocument document(_payloadSize + JSON_STRING_SIZE(_invalidRequest.length()));
	JsonObject payload = document.to<JsonObject>();
	payload["ResponseType"] = static_cast<uint16_t>(_responseType);
	JsonObject data = payload.createNestedObject("Data");
	data["Code"] = static_cast<uint16_t>(getCode());
	data["Message"] = static_cast<const char*>(getErrorMessage());
	data["InvalidRequest"] = _invalidRequest;
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
