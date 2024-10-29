#pragma once

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
			const uint16_t _payloadSize = JSON_OBJECT_SIZE(4);

			Communication::Enums::ErrorCode _code;
			const char* _errorMessage;
			Communication::Enums::ResponseType _responseType;

		protected:
			BaseError(Communication::Enums::ErrorCode code, const char* errorMessage);
			virtual void init() override;
			virtual DynamicJsonDocument createPayload() override;

		public:
			explicit operator bool() const;

			Communication::Enums::ErrorCode getCode();
			const char* getErrorMessage();
		};
	}
}