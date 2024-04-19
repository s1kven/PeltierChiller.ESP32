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

		uint16_t responseDocumentSize = calculateJsonDocumentSize(_models);

		DynamicJsonDocument response(responseDocumentSize);
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

uint16_t Services::JsonService::calculateJsonDocumentSize(
	Models::Abstractions::KeyValuePair<Models::Abstractions::BaseJsonModel**, uint8_t> _models)
{
	uint16_t documentSize = 0;
	for (int i = 0; i < _models.value; i++)
	{
		documentSize += (*_models.key[i]).GetJsonSize();
	}
	documentSize += _baseResponseSize + 8; // 8 - size of JsonArray

	return documentSize;
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

#pragma region buildResponse

void Services::JsonService::buildTemperatureSensorsResponse(JsonObject& _data,
	Models::Abstractions::KeyValuePair<Models::Abstractions::BaseJsonModel**, uint8_t> _models)
{
	JsonArray temperatureSensors = _data.createNestedArray("TemperatureSensors");
	for (int i = 0; i < _models.value; i++)
	{
		temperatureSensors.add((*_models.key[i]).createPayload());
	}
}

#pragma endregion
