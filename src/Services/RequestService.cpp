#include "RequestService.h"

void Services::RequestService::handleRequest(Communication::Models::Requests::BaseRequest* request)
{
	if (request != nullptr)
	{
		if(_configurationService->isMainConfigInit() || request->isIndependentToConfig())
		{
			request->invoke();
			request->clear();
			delete request;
		}
		else if(!request->isIndependentToConfig())
		{
			Communication::Models::Responses::Response* commandResponse =
				new Communication::Models::Responses::Response(Communication::Enums::ResponseType::errorRequest, false,
					nullptr, "The request could not be applied due to a failure during initialization of the main configuration.");
			_communicationService->sendResponse(commandResponse);
		}
	}
	else
	{
		Communication::Models::Responses::Response* commandResponse =
			new Communication::Models::Responses::Response(Communication::Enums::ResponseType::errorRequest, false,
				nullptr, "Failed to parse request.");
		_communicationService->sendResponse(commandResponse);
	}
}
