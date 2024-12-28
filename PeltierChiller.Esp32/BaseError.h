#pragma once
#ifndef _BaseError_
#define _BaseError_ 

#include "BaseDeserializableObject.h"
#include "BaseSerializableObject.h"
#include "ErrorCode.cpp"
#include "ResponseType.cpp"

namespace Communication
{
	namespace Abstractions
	{
		class BaseError : 
			public Communication::Abstractions::BaseDeserializableObject, public Communication::Abstractions::BaseSerializableObject
		{
		private:
			const uint16_t _payloadSize = JSON_OBJECT_SIZE(5);

			Communication::Enums::ErrorCode _code;
			const char* _errorMessage;
			Communication::Enums::ResponseType _responseType;
			String _invalidRequest;

		protected:
			BaseError(Communication::Enums::ResponseType errorType, Communication::Enums::ErrorCode code, 
				const char* errorMessage, String invalidRequest);
			virtual void init() override;
			virtual DynamicJsonDocument createPayload() override;

		public:
			explicit operator bool() const;

			Communication::Enums::ErrorCode getCode();
			const char* getErrorMessage();
		};
	}
}
#endif