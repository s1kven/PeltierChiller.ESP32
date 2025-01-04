#pragma once
#ifndef _RequestService_
#define _RequestService_ 

#include "Communication/Models/Requests/BaseRequest.h"
#include "Communication/Services/CommunicationService.h"

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