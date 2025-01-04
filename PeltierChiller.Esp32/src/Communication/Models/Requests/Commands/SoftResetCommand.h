#pragma once
#ifndef _SoftResetCommand_
#define _SoftResetCommand_ 

#include "Esp.h"
#include "../BaseRequest.h"

namespace Communication
{
	namespace Models
	{
		namespace Requests
		{
			namespace Commands
			{
				class SoftResetCommand :
					public Communication::Models::Requests::BaseRequest
				{
				protected:
					void init() override;

				public:
					SoftResetCommand();
					void invoke() override;
					void clear() override;
				};
			}
		}
	}
}
#endif