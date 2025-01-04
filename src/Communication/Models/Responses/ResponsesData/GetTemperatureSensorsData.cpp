#include "GetTemperatureSensorsData.h"

Communication::Models::Responses::ResponsesData::GetTemperatureSensorsData::GetTemperatureSensorsData(
	LinkedList<::Models::TemperatureSensors::BaseSensor*>* temperatureSensors)
{
	_temperatureSensors = temperatureSensors;

	uint16_t resultedPayloadSize = _payloadSize
		+ JSON_ARRAY_SIZE(_temperatureSensors->size());
	for (int i = 0; i < _temperatureSensors->size(); i++)
	{
		resultedPayloadSize += _temperatureSensors->get(i)->getJsonSize();
	}
	Communication::Abstractions::BaseSerializableObject::setJsonSize(resultedPayloadSize);
}

DynamicJsonDocument Communication::Models::Responses::ResponsesData::GetTemperatureSensorsData::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());
	JsonArray temperatureSensors = document.createNestedArray("TemperatureSensors");
	for (int i = 0; i < _temperatureSensors->size(); i++)
	{
		temperatureSensors.add((*_temperatureSensors->get(i)).createPayload());
	}

	return document;
}

void Communication::Models::Responses::ResponsesData::GetTemperatureSensorsData::clear()
{

}
