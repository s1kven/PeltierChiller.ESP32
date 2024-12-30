#include "GetPwmItemsData.h"

Communication::Models::Responses::ResponsesData::GetPwmItemsData::GetPwmItemsData(
	LinkedList<::Models::PwmItem*>* pwmItems)
{
	_pwmItems = pwmItems;

	uint16_t resultedPayloadSize = _payloadSize
		+ JSON_ARRAY_SIZE(_pwmItems->size());
	for (int i = 0; i < _pwmItems->size(); i++)
	{
		resultedPayloadSize += _pwmItems->get(i)->getJsonSize();
	}
	Communication::Abstractions::BaseSerializableObject::setJsonSize(resultedPayloadSize);
}

DynamicJsonDocument Communication::Models::Responses::ResponsesData::GetPwmItemsData::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());
	JsonArray pwmItems = document.createNestedArray("Pwms");
	for (int i = 0; i < _pwmItems->size(); i++)
	{
		pwmItems.add((*_pwmItems->get(i)).createPayload());
	}

	return document;
}

void Communication::Models::Responses::ResponsesData::GetPwmItemsData::clear()
{
}
