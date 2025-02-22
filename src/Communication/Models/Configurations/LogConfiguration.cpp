#include "LogConfiguration.h"

void Communication::Models::Configurations::LogConfiguration::init()
{
}

void Communication::Models::Configurations::LogConfiguration::init(bool isEnabled, uint32_t logDelay, uint16_t logExpiration, String createNewLogTime)
{
    _isEnabled = isEnabled;
    _logDelay = logDelay;
    _logExpiration = logExpiration;
    _createNewLogTime = createNewLogTime;
    uint16_t resultedPayloadSize = _payloadSize + JSON_STRING_SIZE(_createNewLogTime.length());
	Communication::Abstractions::BaseSerializableObject::setJsonSize(resultedPayloadSize);
}

bool Communication::Models::Configurations::LogConfiguration::getIsEnabled()
{
    return _isEnabled;
}

uint32_t Communication::Models::Configurations::LogConfiguration::getLogDelay()
{
    return _logDelay;
}

uint16_t Communication::Models::Configurations::LogConfiguration::getLogExpiration()
{
    return _logExpiration;
}

String Communication::Models::Configurations::LogConfiguration::getCreateNewLogTime()
{
    return _createNewLogTime;
}

DynamicJsonDocument Communication::Models::Configurations::LogConfiguration::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());
    JsonObject payload = document.to<JsonObject>();

    payload["IsEnabled"] = _isEnabled;
	payload["Delay"] = _logDelay;
	payload["CreateNewLogTime"] = _createNewLogTime;
	payload["LogExpiration"] = _logExpiration;
	return document;
}
