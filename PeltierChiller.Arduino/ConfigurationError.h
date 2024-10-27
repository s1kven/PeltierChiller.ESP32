#pragma once
#include "BaseError.h"

namespace Communication
{
	namespace Models
	{
		class ConfigurationError :
			public Communication::Abstractions::BaseError
		{
		protected:
			void init() override;
			virtual DynamicJsonDocument createPayload() override;
			const char* getErrorMessageByCode(Communication::Enums::ErrorCode code);

		public:
			ConfigurationError(Communication::Enums::ErrorCode code, const char* errorMessage);
		};
	}
}