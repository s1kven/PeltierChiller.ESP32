#pragma once
#ifndef _CommunicationService_
#define _CommunicationService_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "Response.h"

namespace Services {
	class JsonService;
}

extern Services::JsonService* _jsonService;

namespace Communication
{
	namespace Services 
	{
		class CommunicationService
		{
		public:

			virtual void init() = 0;

			virtual void sendData(String data) = 0;
			virtual void sendResponse(Communication::Models::Responses::Response* response) = 0;
			virtual String readData() = 0;
		};
	}
}
#endif