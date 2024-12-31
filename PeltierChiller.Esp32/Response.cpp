#include "Response.h"

Communication::Models::Responses::Response::Response(Communication::Enums::ResponseType responseType, 
	bool success, ResponsesData::BaseResponseData* data, String errorMessage)
{
	_responseType = responseType;
	_success = success;
	_data = data;
	_errorMessage = errorMessage;

	uint16_t resultedPayloadSize = _payloadSize
		+ JSON_STRING_SIZE(errorMessage.length());
	if (_data != nullptr)
	{
		resultedPayloadSize += _data->getJsonSize();
	}
	Communication::Abstractions::BaseSerializableObject::setJsonSize(resultedPayloadSize);
}

Communication::Models::Responses::Response::Response(Communication::Enums::ResponseType responseType, 
	ResponsesData::BaseResponseData* data) : Response(responseType, true, data, "")
{
}

Communication::Models::Responses::Response::Response(Communication::Enums::ResponseType responseType, 
	String errorMessage) : Response(responseType, false, nullptr, errorMessage)
{
}

DynamicJsonDocument Communication::Models::Responses::Response::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());
	JsonObject payload = document.to<JsonObject>();

	payload["ResponseType"] = static_cast<uint16_t>(_responseType);
	payload["Success"] = _success;

	if (_data != nullptr)
	{
		JsonObject responseData = payload.createNestedObject("Data");
		responseData.set(_data->createPayload().as<JsonObjectConst>());
	}
	else 
	{
		payload["Data"] = (char*)NULL;
	}

	payload["ErrorMessage"] = _errorMessage.length() ? _errorMessage : (char*)NULL;

	return document;
}

void Communication::Models::Responses::Response::clear()
{
	if (_data != nullptr)
	{
		_data->clear();
		delete _data;
	}
}

bool Communication::Models::Responses::Response::getSuccess()
{
	return _success;
}
