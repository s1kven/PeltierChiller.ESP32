#pragma once
#ifndef _ConfigurationError_
#define _ConfigurationError_ 
#include "BaseError.h"

namespace Communication
{
	namespace Models
	{
		namespace Errors
		{
			class ConfigurationError :
				public Communication::Abstractions::BaseError
			{
			protected:
				void init() override;
				virtual DynamicJsonDocument createPayload() override;
				const char* getErrorMessageByCode(Communication::Enums::ErrorCode code);

			public:
				ConfigurationError(Communication::Enums::ErrorCode code, const char* errorMessage, String invalidConfiguration);
			};
		}
	}
}
#endif