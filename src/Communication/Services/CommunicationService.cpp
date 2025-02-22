#pragma once
#include "CommunicationService.h"
#include "Services/JsonService.h"
#include "Services/LogService.h"
#include "CommunicationService.h"

Communication::Models::Requests::BaseRequest* Communication::Services::CommunicationService::readRequest(String payload)
{
	return _jsonService->deserializeRequest(payload);
}

void Communication::Services::CommunicationService::sendData(String data)
{
	if(_logService != nullptr)
	{
		_logService->addToLogQueue(data);
	}
}
