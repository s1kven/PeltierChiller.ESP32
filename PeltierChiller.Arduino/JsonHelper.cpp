#pragma once

#include "JsonHelper.h"

KeyValuePair<BaseSerializableObject**, uint8_t> Helpers::JsonHelper::convertToBaseSerializableObjectArray(BaseSensor* _models[], uint8_t _arraySize)
{
	BaseSerializableObject** _resultArrayKey = new BaseSerializableObject * [_arraySize];

	for (int i = 0; i < _arraySize; i++)
	{
		_resultArrayKey[i] = _models[i];
	}
	KeyValuePair<BaseSerializableObject**, uint8_t> _result =
	{
		_resultArrayKey,
		_arraySize
	};

	return _result;
}
