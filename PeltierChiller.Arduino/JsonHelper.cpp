#pragma once

#include "JsonHelper.h"

KeyValuePair<BaseJsonModel**, uint8_t> Helpers::JsonHelper::convertToBaseJsonModelArray(BaseSensor* _models[], uint8_t _arraySize)
{
	BaseJsonModel** _resultArrayKey = new BaseJsonModel* [_arraySize];

	for (int i = 0; i < _arraySize; i++)
	{
		_resultArrayKey[i] = _models[i];
	}
	KeyValuePair<BaseJsonModel**, uint8_t> _result =
	{
		_resultArrayKey,
		_arraySize
	};

	return _result;
}
