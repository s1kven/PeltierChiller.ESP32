#include "BaseJsonModel.h"

Models::Abstractions::BaseJsonModel::BaseJsonModel(uint16_t jsonSize)
{
    _jsonSize = jsonSize;
}

uint16_t Models::Abstractions::BaseJsonModel::GetJsonSize()
{
    return _jsonSize;
}
