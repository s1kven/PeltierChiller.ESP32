#pragma once

namespace Communication
{
	namespace Abstractions
	{
		class BaseDeserializableObject
		{
		protected:
			virtual void init() = 0;
		};
	}
}
