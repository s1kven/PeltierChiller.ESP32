#include "BaseSerializableObject.h"

Communication::Abstractions::BaseSerializableObject::BaseSerializableObject(uint16_t jsonSize)
{
    _jsonSize = jsonSize;
}

uint16_t Communication::Abstractions::BaseSerializableObject::GetJsonSize()
{
    return _jsonSize;
}
