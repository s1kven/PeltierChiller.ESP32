#pragma once
#ifndef _RequestService_
#define _RequestService_ 

#include "BaseRequest.h"
#include "CommunicationService.h"

extern Communication::Services::CommunicationService* _communicationService;

namespace Communication
{
	namespace Enums {
		enum class ResponseType;
	}
}

namespace Services
{
	class RequestService
	{
	private:

	public:
		void handleRequest(Communication::Models::Requests::BaseRequest* request);
	};
}
#endif