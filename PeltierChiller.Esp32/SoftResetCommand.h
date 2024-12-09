#pragma once

#include "Esp.h"
#include "BaseDeserializableObject.h"
#include "BaseCommand.h"

namespace Communication
{
	namespace Models
	{
		class SoftResetCommand :
			public ::Models::Abstractions::BaseCommand
		{
		protected:
			void init() override;

		public:
			void invoke() override;
		};
	}
}