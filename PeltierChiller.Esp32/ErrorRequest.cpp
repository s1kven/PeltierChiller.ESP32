#include "ErrorRequest.h"
#include "ResponseType.cpp"

Communication::Models::Requests::ErrorRequest::ErrorRequest(Communication::Enums::RequestType requestType, const char* payload, const char* errorMessage)
	: Communication::Models::Requests::BaseRequest(requestType)
{
	_payload = payload;
	_errorMessage = errorMessage;
}

void Communication::Models::Requests::ErrorRequest::init()
{
}

void Communication::Models::Requests::ErrorRequest::invoke()
{
	Communication::Models::Responses::Response* errorResponse = new Communication::Models::Responses::Response(
		Communication::Enums::ResponseType::errorRequest, getMessage());
	_communicationService->sendResponse(errorResponse);
}

void Communication::Models::Requests::ErrorRequest::clear()
{
}

String Communication::Models::Requests::ErrorRequest::getMessage()
{
	return String(_errorMessage) + String(_payload);
}
