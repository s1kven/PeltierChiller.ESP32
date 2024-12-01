#pragma once

#include "JsonHelper.h"

KeyValuePair<BaseSerializableObject**, uint8_t> Helpers::JsonHelper::convertToBaseSerializableObjectArray(
	LinkedList<Models::TemperatureSensors::BaseSensor*>* temperatureSensors)
{
	uint16_t listSize = temperatureSensors->size();

	BaseSerializableObject** _resultArrayKey = new BaseSerializableObject * [listSize];

	for (int i = 0; i < listSize; i++)
	{
		_resultArrayKey[i] = temperatureSensors->get(i);
	}
	KeyValuePair<BaseSerializableObject**, uint8_t> _result =
	{
		_resultArrayKey,
		listSize
	};

	return _result;
}



KeyValuePair<BaseSerializableObject**, uint8_t> Helpers::JsonHelper::convertToBaseSerializableObjectArray(
	LinkedList<Models::PwmItem*>* pwmItems)
{
	uint16_t listSize = pwmItems->size();

	BaseSerializableObject** _resultArrayKey = new BaseSerializableObject * [listSize];

	for (int i = 0; i < listSize; i++)
	{
		_resultArrayKey[i] = pwmItems->get(i);
	}
	KeyValuePair<BaseSerializableObject**, uint8_t> _result =
	{
		_resultArrayKey,
		listSize
	};

	return _result;
}
