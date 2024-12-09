#pragma once

#include "BaseError.h"

namespace Communication
{
	namespace Models
	{
		namespace Errors
		{
			class CommandError :
				public Communication::Abstractions::BaseError
			{
			private:

			protected:
				void init() override;
				virtual DynamicJsonDocument createPayload() override;

			public:
				CommandError(String invalidCommand);
			};
		}
	}
}