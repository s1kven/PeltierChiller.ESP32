#pragma once
#ifndef _BaseDeserializableObject_
#define _BaseDeserializableObject_ 

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
#endif
