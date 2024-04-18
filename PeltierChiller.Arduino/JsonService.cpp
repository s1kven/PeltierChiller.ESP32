#pragma once

#include "JsonService.h"

Services::JsonService::JsonService(uint32_t serialWriteDelay)
{
	_serialWriteDelay = serialWriteDelay;
}

void Services::JsonService::serializeAndSendToSerialPort(
	Models::Abstractions::KeyValuePair<Models::Abstractions::BaseJsonModel**, uint8_t> _models, Models::Enums::ResponseType _responseType)
{
	if (millis() - _serialWriteTimer >= _serialWriteDelay)
	{
		_serialWriteTimer = millis();

		StaticJsonDocument<256> response;
		JsonObject responsePayload = response.to<JsonObject>();

		responsePayload["ResponseType"] = _responseType;

		JsonObject data = responsePayload.createNestedObject("Data");

		buildResponseBasedOnType(data, _models, _responseType);

		serializeJson(response, Serial);
		Serial.println();

		response.clear();
	}

	delete _models.key;
}



void Services::JsonService::buildResponseBasedOnType(JsonObject& _data, 
	Models::Abstractions::KeyValuePair<Models::Abstractions::BaseJsonModel**, uint8_t> _models, 
	Models::Enums::ResponseType _responseType)
{
	switch (_responseType)
	{
	case Models::Enums::unknown:
		break;
	case Models::Enums::temperatureSensors:
		buildTemperatureSensorsResponse(_data, _models);

		break;
	default:
		break;
	}
}

void Services::JsonService::buildTemperatureSensorsResponse(JsonObject& _data,
	Models::Abstractions::KeyValuePair<Models::Abstractions::BaseJsonModel**, uint8_t> _models)
{
	JsonArray temperatureSensors = _data.createNestedArray("TemperatureSensors");
	for (int i = 0; i < _models.value; i++)
	{
		temperatureSensors.add((*_models.key[i]).createPayload());
	}
}
