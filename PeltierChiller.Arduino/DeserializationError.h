#pragma once
#include "BaseError.h"

namespace Communication
{
	namespace Models
	{
		class DeserializationError :
			public Communication::Abstractions::BaseError
		{
		protected:
			void init() override;
			virtual DynamicJsonDocument createPayload() override;
			
		public:
			DeserializationError(Communication::Enums::ErrorCode code, const char* errorMessage);
		};
	}
}