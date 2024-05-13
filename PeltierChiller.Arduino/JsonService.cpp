#pragma once

#include "JsonService.h"

String Services::JsonService::serializeObject(
	Models::Abstractions::KeyValuePair<Communication::Abstractions::BaseSerializableObject**, uint8_t> _models,
	Communication::Enums::ResponseType _responseType)
{
	uint16_t responseDocumentSize = calculateJsonDocumentSize(_models);

	DynamicJsonDocument response(responseDocumentSize);
	JsonObject responsePayload = response.to<JsonObject>();

	responsePayload["ResponseType"] = static_cast<uint16_t>(_responseType);

	JsonObject data = responsePayload.createNestedObject("Data");

	buildResponseBasedOnType(data, _models, _responseType);

	delete _models.key;

	String serializedObject = response.as<String>();

	response.clear();

	return serializedObject;
}

Communication::Abstractions::BaseDeserializableObject* Services::JsonService::deserializeRequest(String& content)
{
	DynamicJsonDocument document(_baseResponseSize + content.length());
	JsonObject payload = document.to<JsonObject>();
	DeserializationError err = deserializeJson(document, content);

	uint8_t type = document["RequestType"];
	JsonObject data = document["Data"];

	Communication::Abstractions::BaseDeserializableObject* deserializedObject = deserializeRequestByType(
		static_cast<Communication::Enums::RequestType>(type), data);
	return deserializedObject;
}

uint16_t Services::JsonService::calculateJsonDocumentSize(
	Models::Abstractions::KeyValuePair<Communication::Abstractions::BaseSerializableObject**, uint8_t> _models)
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
	Models::Abstractions::KeyValuePair<Communication::Abstractions::BaseSerializableObject**, uint8_t> _models,
	Communication::Enums::ResponseType _responseType)
{
	switch (_responseType)
	{
	case Communication::Enums::ResponseType::unknown:
		break;
	case Communication::Enums::ResponseType::temperatureSensors:
		buildTemperatureSensorsResponse(_data, _models);

		break;
	default:
		break;
	}
}

Communication::Abstractions::BaseDeserializableObject* Services::JsonService::deserializeRequestByType(
	Communication::Enums::RequestType _requestType, JsonObject data)
{
	Communication::Abstractions::BaseDeserializableObject* content;
	switch (_requestType)
	{
	case Communication::Enums::RequestType::unknown :
		break;
	case Communication::Enums::RequestType::chillerSettings:
		content = DeserializeChillerConfiguration(data);
		break;
	default:
		break;
	}
	return content;
}

#pragma region buildResponse

void Services::JsonService::buildTemperatureSensorsResponse(JsonObject& _data,
	Models::Abstractions::KeyValuePair<Communication::Abstractions::BaseSerializableObject**, uint8_t> _models)
{
	JsonArray temperatureSensors = _data.createNestedArray("TemperatureSensors");
	for (int i = 0; i < _models.value; i++)
	{
		temperatureSensors.add((*_models.key[i]).createPayload());
	}
}

#pragma endregion

#pragma region Deserializers

Communication::Models::ChillerConfiguration* Services::JsonService::DeserializeChillerConfiguration(JsonObject data)
{
	float temp = data["TargetTemperature"];

	return new Communication::Models::ChillerConfiguration(temp);
}

#pragma endregion
