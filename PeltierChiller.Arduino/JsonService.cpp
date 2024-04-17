#pragma once

#include "JsonService.h"

Services::JsonService::JsonService(uint32_t serialWriteDelay)
{
	_serialWriteDelay = serialWriteDelay;
}

void Services::JsonService::serializeAndSendToSerialPort(
	Models::Abstractions::KeyValuePair<Models::Abstractions::BaseJsonModel**, uint8_t> _models)
{
	if (millis() - _serialWriteTimer >= _serialWriteDelay)
	{
		_serialWriteTimer = millis();

		StaticJsonDocument<242> document;
		JsonObject payload = document.to<JsonObject>();

		StaticJsonDocument<222> nestedDocument;
		JsonArray nestedDocumentPayload = nestedDocument.to<JsonArray>();

		for (int i = 0; i < _models.value; i++)
		{
			nestedDocumentPayload.add((*_models.key[i]).createPayload());
		}
		payload["TemperatureSensors"] = nestedDocumentPayload;
		serializeJson(document, Serial);
		Serial.println();

		nestedDocument.clear();
		document.clear();
	}

	delete _models.key;
}
