#include "ConfigurationData.h"

Communication::Models::Responses::ResponsesData::ConfigurationData::ConfigurationData(
    Communication::Models::Configurations::Configuration *configuration)
{
    _configuration = configuration;

	uint16_t resultedPayloadSize = _payloadSize + _configuration->getJsonSize();
	Communication::Abstractions::BaseSerializableObject::setJsonSize(resultedPayloadSize);
}

DynamicJsonDocument Communication::Models::Responses::ResponsesData::ConfigurationData::createPayload()
{
    DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());
    document = _configuration->createPayload();
	return document;
}

void Communication::Models::Responses::ResponsesData::ConfigurationData::clear()
{
    _configuration->clear();
    delete _configuration;
}
