#pragma once

#include "JsonService.h"

String Services::JsonService::serializeObject(Communication::Abstractions::BaseSerializableObject* response)
{
	String serializedObject = response->createPayload().as<String>();

	return serializedObject;
}

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
	uint32_t documentSize = getDeserializedJsonSize(content);
	DynamicJsonDocument document(documentSize);
	JsonObject payload = document.to<JsonObject>();
	DeserializationError error = deserializeJson(document, content);

	if (error)
	{
		Communication::Abstractions::BaseDeserializableObject* errorObject = buildError(error);
		return errorObject;
	}
	else 
	{
		uint8_t type = document["RequestType"];
		JsonObject data = document["Data"];

		Communication::Abstractions::BaseDeserializableObject* deserializedObject = deserializeRequestByType(
			static_cast<Communication::Enums::RequestType>(type), data);
		return deserializedObject;
	}
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


uint32_t Services::JsonService::getDeserializedJsonSize(String& content)
{
	uint16_t objectsCount = 0;
	uint16_t arraysCount = 0;

	for (int i = 0; i < content.length(); i++)
	{
		if (content[i] == '{')
		{
			objectsCount++;
		}
		else if (content[i] == '[')
		{
			arraysCount++;
		}
	}

	uint32_t documentSize = objectsCount * sizeof(JsonObject) + arraysCount * sizeof(JsonArray) + content.length();

	return documentSize;
}

Communication::Models::DeserializationError* Services::JsonService::buildError(DeserializationError error)
{
	return new Communication::Models::DeserializationError(errorCodeConverter(error), error.c_str());
}

Communication::Enums::ErrorCode Services::JsonService::errorCodeConverter(DeserializationError error)
{
	switch (error.code())
	{
	case error.Ok:
		return Communication::Enums::ErrorCode::ok;
	case error.EmptyInput:
		return Communication::Enums::ErrorCode::emptyInput;
	case error.IncompleteInput:
		return Communication::Enums::ErrorCode::incompleteInput;
	case error.InvalidInput:
		return Communication::Enums::ErrorCode::invalidInput;
	case error.NoMemory:
		return Communication::Enums::ErrorCode::noMemory;
	case error.TooDeep:
		return Communication::Enums::ErrorCode::tooDeep;
	}
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
	case Communication::Enums::ResponseType::pwms:
		buildPwmsResponse(_data, _models);
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
	case Communication::Enums::RequestType::configuration:
		content = deserializeConfiguration(data);
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

void Services::JsonService::buildPwmsResponse(JsonObject& _data,
	Models::Abstractions::KeyValuePair<Communication::Abstractions::BaseSerializableObject**, uint8_t> _models)
{
	JsonArray temperatureSensors = _data.createNestedArray("Pwms");
	for (int i = 0; i < _models.value; i++)
	{
		temperatureSensors.add((*_models.key[i]).createPayload());
	}
}

#pragma endregion buildResponse

#pragma region Deserializers

#pragma region Configurations

Communication::Models::Configurations::Configuration* Services::JsonService::deserializeConfiguration(JsonObject data)
{
	Communication::Models::Configurations::Configuration* configuration = new Communication::Models::Configurations::Configuration();

	uint8_t chillerTypeDigit = data["ChillerType"];
	ChillerType chillerType = static_cast<ChillerType>(chillerTypeDigit);
	float targetTemperature = data["TargetTemperature"];
	float pcVoltageThreshold = data["PcVoltageThreshold"];

	JsonObject pinsConfigurationJson = data["Pins"]; 
	Communication::Models::Configurations::PinsConfiguration* pinsConfiguration = deserializePinsConfiguration(pinsConfigurationJson);

	JsonObject timersConfigurationJson = data["Timers"];
	Communication::Models::Configurations::TimersConfiguration* timersConfiguration = deserializeTimersConfiguration(timersConfigurationJson);

	JsonObject chillerConfigurationJson = data["ChillerSettings"];
	Communication::Models::Configurations::ChillerConfiguration* chillerConfiguration = deserializeChillerConfiguration(chillerConfigurationJson);
	
	JsonObject temperatureSensorsJson = data["TemperatureSensors"];
	Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* temperatureSensorsConfiguration =
		deserializeTemperatureSensorsConfiguration(temperatureSensorsJson);

	JsonArray pwmsJson = data["PWMs"];
	Communication::Models::Configurations::PwmsConfiguration* pwmsConfiguration =
		deserializePwmsConfiguration(pwmsJson);

	configuration->init(chillerType, targetTemperature, pcVoltageThreshold, 
		pinsConfiguration, timersConfiguration, chillerConfiguration, 
		temperatureSensorsConfiguration, pwmsConfiguration);

	return configuration;
}

Communication::Models::Configurations::PinsConfiguration* Services::JsonService::deserializePinsConfiguration(JsonObject data)
{
	Communication::Models::Configurations::PinsConfiguration* pinsConfiguration = new Communication::Models::Configurations::PinsConfiguration();
	uint8_t powerButtonPin = data["PowerButton"];
	uint8_t powerSignalPin = data["PowerSignal"];
	uint8_t chillerPsSignalPin = data["ChillerPsSignal"];
	uint8_t chillerSignal = data["ChillerSignal"];
	uint8_t pcVoltage = data["PcVoltage"];
	pinsConfiguration->init(powerButtonPin, powerSignalPin, chillerPsSignalPin, chillerSignal, pcVoltage);

	return pinsConfiguration;
}

Communication::Models::Configurations::TimersConfiguration* Services::JsonService::deserializeTimersConfiguration(JsonObject data)
{
	Communication::Models::Configurations::TimersConfiguration* timersConfiguration = new Communication::Models::Configurations::TimersConfiguration();
	uint32_t buttonMinPressTime = data["ButtonMinPressTime"];
	uint32_t communicationDelay = data["CommunicationDelay"];
	uint32_t temperatureSensorsRequestDelay = data["TemperatureSensorsRequestDelay"];
	uint32_t updatePwmDelay = data["UpdatePwmDelay"];
	timersConfiguration->init(buttonMinPressTime, communicationDelay, temperatureSensorsRequestDelay, updatePwmDelay);

	return timersConfiguration;
}

Communication::Models::Configurations::ChillerConfiguration* Services::JsonService::deserializeChillerConfiguration(JsonObject data)
{
	Communication::Models::Configurations::ChillerConfiguration* chillerConfiguration = new Communication::Models::Configurations::ChillerConfiguration();
	uint16_t potentiometerAddress = data["PotentiometerAddress"];
	uint16_t maxPotentiometerValue = data["MaxPotentiometerValue"];
	uint16_t minPotentiometerValue = data["MinPotentiometerValue"];
	float kp = data["Kp"];
	float ki = data["Ki"];
	float kd = data["Kd"];
	float dt = data["Dt"];
	float pidRatio = data["PidRatio"];
	float minIntegral = data["MinIntegral"];
	float maxIntegral = data["MaxIntegral"];
	uint32_t computePidDelay = data["ComputePidDelay"];
	chillerConfiguration->init(potentiometerAddress, maxPotentiometerValue, minPotentiometerValue, kp, ki, kd, dt, pidRatio,
		minIntegral, maxIntegral, computePidDelay);

	return chillerConfiguration;
}

Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* 
Services::JsonService::deserializeTemperatureSensorsConfiguration(JsonObject data)
{
	Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* _temperatureSensorsConfiguration =
		new Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration();

	JsonObject bme280ListConfigurationJson = data["BME280"];
	Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration* bme280ListConfiguration = 
		deserializeBme280ListConfiguration(bme280ListConfigurationJson);

	JsonObject ntcListConfigurationJson = data["NTC"];
	Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration* ntcListConfiguration =
		deserializeNtcListConfiguration(ntcListConfigurationJson);

	JsonObject ds18b20ListConfigurationJson = data["DS18B20"];
	Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration* ds18b20ListConfiguration =
		deserializeDs18b20ListConfiguration(ds18b20ListConfigurationJson);

	_temperatureSensorsConfiguration->init(bme280ListConfiguration, ntcListConfiguration, ds18b20ListConfiguration);

	return _temperatureSensorsConfiguration;
}

Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration* 
Services::JsonService::deserializeBme280ListConfiguration(JsonObject data)
{
	Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration* _bme280ListConfiguration =
		new Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration();
	JsonArray items = data["Items"];
	if (items.size() > 0)
	{
		LinkedList<Communication::Models::Configurations::TemperatureSensors::Bme280Configuration*>* _bme280Items =
			new LinkedList<Communication::Models::Configurations::TemperatureSensors::Bme280Configuration*>();
		for (int i = 0; i < items.size(); i++)
		{
			Communication::Models::Configurations::TemperatureSensors::Bme280Configuration* _bme280Configuration =
				new Communication::Models::Configurations::TemperatureSensors::Bme280Configuration();
			uint16_t address = items[i]["Address"];
			uint8_t digitTarget = items[i]["Target"];
			TemperatureSensorTarget target = static_cast<TemperatureSensorTarget>(digitTarget);

			_bme280Configuration->init(address, target);
			_bme280Items->add(_bme280Configuration);
		}
		_bme280ListConfiguration->init(_bme280Items);
	}

	return _bme280ListConfiguration;
}

Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration* 
Services::JsonService::deserializeNtcListConfiguration(JsonObject data)
{
	Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration* _ntcListConfiguration =
		new Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration();
	uint8_t adcResolution = data["AdcResolution"];
	JsonArray items = data["Items"];
	if (items.size() > 0)
	{
		LinkedList<Communication::Models::Configurations::TemperatureSensors::NtcConfiguration*>* _ntcItems =
			new LinkedList<Communication::Models::Configurations::TemperatureSensors::NtcConfiguration*>();
		for (int i = 0; i < items.size(); i++)
		{
			Communication::Models::Configurations::TemperatureSensors::NtcConfiguration* _ntcConfiguration =
				new Communication::Models::Configurations::TemperatureSensors::NtcConfiguration();
			uint16_t address = items[i]["Address"];
			uint8_t digitTarget = items[i]["Target"];
			TemperatureSensorTarget target = static_cast<TemperatureSensorTarget>(digitTarget);
			uint32_t resistance = items[i]["Resistance"];
			uint32_t resistanceNtc = items[i]["ResistanceNTC"];
			uint32_t bCoefficient = items[i]["BCoefficient"];
			float baseTemperature = items[i]["BaseTemperature"];
			float supplyVoltage = items[i]["SupplyVoltage"];

			_ntcConfiguration->init(address, target, resistance, resistanceNtc,
				bCoefficient, baseTemperature, supplyVoltage);
			_ntcItems->add(_ntcConfiguration);
		}
		_ntcListConfiguration->init(adcResolution, _ntcItems);
	}

	return _ntcListConfiguration;
}

Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration* 
Services::JsonService::deserializeDs18b20ListConfiguration(JsonObject data)
{
	Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration* _ds18b20ListConfiguration =
		new Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration();

	uint8_t pin = data["Pin"];
	uint8_t temperaturePrecision = data["TemperaturePrecision"];
	JsonArray items = data["Items"];
	if (items.size() > 0)
	{
		LinkedList<Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration*>* _ds18b20Items =
			new LinkedList<Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration*>();
		for (int i = 0; i < items.size(); i++)
		{
			Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration* _ds18b20Configuration =
				new Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration();
			JsonArray addressJson = items[i]["Address"];
			DeviceAddress address;
			for (int j = 0; j < addressJson.size(); j++)
			{
				address[j] = addressJson[j];
			}

			uint8_t digitTarget = items[i]["Target"];
			TemperatureSensorTarget target = static_cast<TemperatureSensorTarget>(digitTarget);

			_ds18b20Configuration->init(address, target);
			_ds18b20Items->add(_ds18b20Configuration);
		}
		_ds18b20ListConfiguration->init(pin, temperaturePrecision, _ds18b20Items);
	}

	return _ds18b20ListConfiguration;
}

Communication::Models::Configurations::PwmsConfiguration* Services::JsonService::deserializePwmsConfiguration(JsonArray items)
{
	Communication::Models::Configurations::PwmsConfiguration* pwmsConfiguration =
		new Communication::Models::Configurations::PwmsConfiguration();
	if (items.size() > 0)
	{
		LinkedList<Communication::Models::Configurations::PwmConfiguration*>* pwmItems =
			new LinkedList<Communication::Models::Configurations::PwmConfiguration*>();
		for (int i = 0; i < items.size(); i++)
		{
			Communication::Models::Configurations::PwmConfiguration* pwmConfiguration =
				new Communication::Models::Configurations::PwmConfiguration();
			uint8_t tachoPin = items[i]["Tacho"];
			uint8_t pwmPin = items[i]["PWM"];
			String name = items[i]["Name"];
			JsonArray value = items[i]["Value"];
			LinkedList<Communication::Models::Configurations::PwmValueConfiguration*>* pwmValues = 
				new LinkedList<Communication::Models::Configurations::PwmValueConfiguration*>();
			for (int j = 0; j < value.size(); j++)
			{
				pwmValues->add(
					new Communication::Models::Configurations::PwmValueConfiguration(value[j]["Temperature"], value[j]["Load"]));
			}
			uint8_t type = items[i]["ControlType"];
			Models::Enums::PwmType controlType = static_cast<Models::Enums::PwmType>(type);
			pwmConfiguration->init(tachoPin, pwmPin, name, pwmValues, controlType);
			pwmItems->add(pwmConfiguration);
		}
		pwmsConfiguration->init(pwmItems);
	}

	return pwmsConfiguration;
}

#pragma endregion Configurations

#pragma endregion Deserializers
