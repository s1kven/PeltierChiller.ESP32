#pragma once

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