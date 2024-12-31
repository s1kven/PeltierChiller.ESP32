#pragma once

#include "RequestService.h"
#include "ResponseType.cpp"

void Services::RequestService::handleRequest(Communication::Models::Requests::BaseRequest* request)
{
	if (request != nullptr)
	{
		request->invoke();
		request->clear();
		delete request;
	}
	else
	{
		Communication::Models::Responses::Response* commandResponse =
			new Communication::Models::Responses::Response(Communication::Enums::ResponseType::errorRequest, false,
				nullptr, "Failed to parse request.");
		_communicationService->sendResponse(commandResponse);
	}
}
