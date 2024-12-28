#pragma once
#ifndef _SdError_
#define _SdError_ 
#include "BaseError.h"

namespace Communication
{
	namespace Models
	{
		namespace Errors
		{
			class SdError :
				public Communication::Abstractions::BaseError
			{
			protected:
				void init() override;
				virtual DynamicJsonDocument createPayload() override;

			public:
				SdError(Communication::Enums::ErrorCode code, const char* errorMessage, String invalidPayload);
			};
		}
	}
}
#endif
