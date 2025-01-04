#include "PwmsConfiguration.h"

void Communication::Models::Configurations::PwmsConfiguration::init()
{
}

void Communication::Models::Configurations::PwmsConfiguration::init(LinkedList<PwmConfiguration*>* pwmsListConfiguration)
{
	_pwmsListConfiguration = pwmsListConfiguration;
	uint16_t itemsJsonSize = JSON_ARRAY_SIZE(_pwmsListConfiguration->size());
	for (int i = 0; i < _pwmsListConfiguration->size(); i++)
	{
		itemsJsonSize += _pwmsListConfiguration->get(i)->getJsonSize();
	}
	Communication::Abstractions::BaseSerializableObject::setJsonSize(_payloadSize + itemsJsonSize);
}

void Communication::Models::Configurations::PwmsConfiguration::clear()
{
	for (int i = 0; i < _pwmsListConfiguration->size(); i++)
	{
		_pwmsListConfiguration->get(i)->clear();
		delete _pwmsListConfiguration->get(i);
	}
	_pwmsListConfiguration->clear();
	delete _pwmsListConfiguration;
}

LinkedList<Communication::Models::Configurations::PwmConfiguration*>*
Communication::Models::Configurations::PwmsConfiguration::getPwmsListConfiguration()
{
	return _pwmsListConfiguration;
}

DynamicJsonDocument Communication::Models::Configurations::PwmsConfiguration::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());
	JsonArray payload = document.to<JsonArray>();

	for (int i = 0; i < _pwmsListConfiguration->size(); i++)
	{
		payload.add((_pwmsListConfiguration->get(i))->createPayload());
	}
	return document;
}
