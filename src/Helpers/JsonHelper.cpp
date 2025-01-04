#pragma once
#include "JsonHelper.h"

uint16_t Helpers::JsonHelper::getFloatJsonSizeWorkaround(uint16_t objectsCount)
{
	return JSON_STRING_SIZE(5) * objectsCount;
}
