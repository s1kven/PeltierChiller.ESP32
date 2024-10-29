#pragma once

namespace Models::Abstractions
{
	template<class KeyType, class ValueType>
	struct KeyValuePair
	{
	public:
		KeyType key;
		ValueType value;
	};
}