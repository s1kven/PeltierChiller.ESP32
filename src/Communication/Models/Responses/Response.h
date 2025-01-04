#pragma once
#ifndef _Response_
#define _Response_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "ResponsesData/BaseResponseData.h"


namespace Communication
{
	namespace Enums {
		enum class ResponseType;
	}

	namespace Models
	{
		namespace Responses
		{
			class Response 
				: public Communication::Abstractions::BaseSerializableObject
			{
			private:
				const uint16_t _payloadSize = JSON_OBJECT_SIZE(4);

				Communication::Enums::ResponseType _responseType;
				bool _success;
				String _errorMessage;
				ResponsesData::BaseResponseData* _data;

			public:
				Response(Communication::Enums::ResponseType responseType, bool success, 
					ResponsesData::BaseResponseData* data, String errorMessage);
				Response(Communication::Enums::ResponseType responseType,
					ResponsesData::BaseResponseData* data);
				Response(Communication::Enums::ResponseType responseType, 
					String errorMessage);
				DynamicJsonDocument createPayload() override;

				void clear();
				bool getSuccess();
			};
		}
	}
}
#endif
