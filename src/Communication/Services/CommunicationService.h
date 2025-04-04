#pragma once
#ifndef _CommunicationService_
#define _CommunicationService_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "../Models/Responses/Response.h"
#include "../Models/Requests/BaseRequest.h"

namespace Services {
	class JsonService;
	class LogService;
}

extern Services::JsonService* _jsonService;
extern Services::LogService* _logService;

namespace Communication
{
	namespace Services 
	{
		class CommunicationService
		{
		protected:
			virtual Communication::Models::Requests::BaseRequest* readRequest(String payload);

		public:

			virtual String readData() = 0;
			virtual void init() = 0;

			virtual void sendData(String data);
			virtual void sendResponse(Communication::Models::Responses::Response* response) = 0;
			virtual Communication::Models::Requests::BaseRequest* readRequest() = 0;
			virtual uint32_t availableToRead() = 0;
		};
	}
}
#endif