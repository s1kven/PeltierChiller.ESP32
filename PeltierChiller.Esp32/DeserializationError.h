#pragma once
#ifndef _DeserializationError_
#define _DeserializationError_ 
#include "BaseError.h"

namespace Communication
{
	namespace Models
	{
		namespace Errors
		{
			class DeserializationError :
				public Communication::Abstractions::BaseError
			{
			protected:
				void init() override;
				virtual DynamicJsonDocument createPayload() override;

			public:
				DeserializationError(Communication::Enums::ErrorCode code, const char* errorMessage, String invalidPayload);
			};
		}
	}
}
#endif