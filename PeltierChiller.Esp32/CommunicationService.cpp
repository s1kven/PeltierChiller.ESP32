#pragma once
#include "CommunicationService.h"
#include "JsonService.h"
#include "CommunicationService.h"

Communication::Models::Requests::BaseRequest* Communication::Services::CommunicationService::readRequest(String payload)
{
	return _jsonService->deserializeRequest(payload);
}