#pragma once
#ifndef _ErrorRequest_
#define _ErrorRequest_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "BaseRequest.h"
#include "../../Services/CommunicationService.h"

extern Communication::Services::CommunicationService* _communicationService;

namespace Communication
{
	namespace Enums {
		enum class ResponseType;
	}
}

namespace Communication
{
	namespace Models
	{
		namespace Requests
		{
			class ErrorRequest :
				public Communication::Models::Requests::BaseRequest
			{
			private:
				const char* _payload;
				const char* _errorMessage;

			protected:
				void init() override;

			public:
				ErrorRequest(Communication::Enums::RequestType requestType, const char* payload, const char* errorMessage);
				void invoke() override;
				void clear() override;
				String getMessage();
			};
		}
	}
}
#endif