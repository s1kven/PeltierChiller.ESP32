#include "BaseSerializableObject.h"

Communication::Abstractions::BaseSerializableObject::BaseSerializableObject()
{

}

void Communication::Abstractions::BaseSerializableObject::setJsonSize(uint16_t jsonSize)
{
    _jsonSize = jsonSize;
}

uint16_t Communication::Abstractions::BaseSerializableObject::GetJsonSize()
{
    return _jsonSize;
}
