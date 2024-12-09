#pragma once

#include "BaseDeserializableObject.h"

namespace Models
{
	namespace Abstractions
	{
		class BaseCommand : public Communication::Abstractions::BaseDeserializableObject
		{
		protected:
			void init() override;
		public:
			virtual void invoke() = 0;
		};
	}
}