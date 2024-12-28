#pragma once
#ifndef _CommandError_
#define _CommandError_ 

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
#endif