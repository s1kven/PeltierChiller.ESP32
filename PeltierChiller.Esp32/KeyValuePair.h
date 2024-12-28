#pragma once
#ifndef _KeyValuePair_
#define _KeyValuePair_ 

namespace Models
{
	namespace Abstractions
	{
		template<class KeyType, class ValueType>
		struct KeyValuePair
		{
		public:
			KeyType key;
			ValueType value;
		};
	}
}
#endif