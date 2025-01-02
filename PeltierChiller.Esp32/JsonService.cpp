#pragma once

#include "JsonService.h"

String Services::JsonService::serializeObject(Communication::Abstractions::BaseSerializableObject* response)
{
	String serializedObject = response->createPayload().as<String>();

	return serializedObject;
}

String Services::JsonService::serializeRequest(Communication::Abstractions::BaseSerializableObject* request,
	Communication::Enums::RequestType requestType)
{
	DynamicJsonDocument requestJson(request->getJsonSize() + JSON_OBJECT_SIZE(2));

	requestJson["RequestType"] = static_cast<uint16_t>(requestType);
	JsonObject dataJson = requestJson.createNestedObject("Data");
	dataJson.set(request->createPayload().as<JsonObjectConst>());

	String serializedRequest;
	serializeJson(requestJson, serializedRequest);

	requestJson.clear();
	return serializedRequest;
}

Communication::Models::Requests::BaseRequest* Services::JsonService::deserializeRequest(String content)
{
	Communication::Models::Requests::BaseRequest* request = nullptr;
	DynamicJsonDocument document(65536);
	DeserializationError error = deserializeJson(document, content);
	if (error)
	{
		request = new Communication::Models::Requests::ErrorRequest(Communication::Enums::RequestType::errorDeserialization, 
			content.c_str(), error.c_str());
	}
	else 
	{
		uint8_t type = document["RequestType"];
		JsonObject data = document["Data"];
		
		request = deserializeRequestByType(static_cast<Communication::Enums::RequestType>(type), data, content);
	}
	document.clear();
	return request;
}

Communication::Models::Requests::BaseRequest* Services::JsonService::deserializeRequestByType(
	Communication::Enums::RequestType _requestType, JsonObject data, String request)
{
	switch (_requestType)
	{
	case Communication::Enums::RequestType::configuration:
		return deserializeConfigurationRequest(data);
	case Communication::Enums::RequestType::softReset:
		return deserializeSoftResetCommand();
	case Communication::Enums::RequestType::updateConfiguration:
		return deserializeUpdateConfigurationCommand(data);
	case Communication::Enums::RequestType::updateTempConfiguration:
		return deserializeUpdateTempConfigurationCommand(data);
	case Communication::Enums::RequestType::resetTempConfiguration:
		return deserializeResetTempConfigurationCommand();
	case Communication::Enums::RequestType::unknown:
	default:
		String emptyString = String();
		return new Communication::Models::Requests::ErrorRequest(_requestType,
			request.c_str(), emptyString.c_str());
	}
}

#pragma region Deserializers

#pragma region Configurations

Communication::Models::Requests::ConfigurationRequest* Services::JsonService::deserializeConfigurationRequest(JsonObject data)
{
	return new Communication::Models::Requests::ConfigurationRequest(deserializeConfiguration(data));
}

Communication::Models::Configurations::Configuration* Services::JsonService::deserializeConfiguration(JsonObject data)
{
	Communication::Models::Configurations::Configuration* configuration = new Communication::Models::Configurations::Configuration();

	uint8_t chillerTypeDigit = data["ChillerType"];
	ChillerType chillerType = static_cast<ChillerType>(chillerTypeDigit);
	float targetTemperature = data["TargetTemperature"];
	float voltmeterThreshold = data["VoltmeterThreshold"];
	uint32_t voltmeterR1 = data["VoltmeterR1"];
	uint32_t voltmeterR2 = data["VoltmeterR2"];
	bool isDelayEnablingPc = data["StartupPcAfterFeatTargetTemperature"];

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

	configuration->init(chillerType, targetTemperature, voltmeterThreshold, voltmeterR1, voltmeterR2,
		isDelayEnablingPc, pinsConfiguration, timersConfiguration, chillerConfiguration, 
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
	LinkedList<Communication::Models::Configurations::TemperatureSensors::Bme280Configuration*>* _bme280Items =
		new LinkedList<Communication::Models::Configurations::TemperatureSensors::Bme280Configuration*>();
	if (items.size() > 0)
	{
		for (int i = 0; i < items.size(); i++)
		{
			Communication::Models::Configurations::TemperatureSensors::Bme280Configuration* _bme280Configuration =
				new Communication::Models::Configurations::TemperatureSensors::Bme280Configuration();
			uint16_t address = items[i]["Address"];
			String name = items[i]["Name"];
			uint8_t digitTarget = items[i]["Target"];
			TemperatureSensorTarget target = static_cast<TemperatureSensorTarget>(digitTarget);

			_bme280Configuration->init(address, name, target);
			_bme280Items->add(_bme280Configuration);
		}
	}
	_bme280ListConfiguration->init(_bme280Items);

	return _bme280ListConfiguration;
}

Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration* 
Services::JsonService::deserializeNtcListConfiguration(JsonObject data)
{
	Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration* _ntcListConfiguration =
		new Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration();
	uint8_t adcResolution = data["AdcResolution"];
	JsonArray items = data["Items"];
	LinkedList<Communication::Models::Configurations::TemperatureSensors::NtcConfiguration*>* _ntcItems =
		new LinkedList<Communication::Models::Configurations::TemperatureSensors::NtcConfiguration*>();
	if (items.size() > 0)
	{
		for (int i = 0; i < items.size(); i++)
		{
			Communication::Models::Configurations::TemperatureSensors::NtcConfiguration* _ntcConfiguration =
				new Communication::Models::Configurations::TemperatureSensors::NtcConfiguration();
			uint16_t address = items[i]["Address"];
			String name = items[i]["Name"];
			uint8_t digitTarget = items[i]["Target"];
			TemperatureSensorTarget target = static_cast<TemperatureSensorTarget>(digitTarget);
			uint32_t resistance = items[i]["Resistance"];
			uint32_t resistanceNtc = items[i]["ResistanceNTC"];
			uint32_t bCoefficient = items[i]["BCoefficient"];
			float baseTemperature = items[i]["BaseTemperature"];
			float supplyVoltage = items[i]["SupplyVoltage"];

			_ntcConfiguration->init(address, name, target, resistance, resistanceNtc,
				bCoefficient, baseTemperature, supplyVoltage);
			_ntcItems->add(_ntcConfiguration);
		}
	}
	_ntcListConfiguration->init(adcResolution, _ntcItems);

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
	LinkedList<Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration*>* _ds18b20Items =
		new LinkedList<Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration*>();
	if (items.size() > 0)
	{
		for (int i = 0; i < items.size(); i++)
		{
			Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration* _ds18b20Configuration =
				new Communication::Models::Configurations::TemperatureSensors::Ds18b20Configuration();
			JsonArray addressJson = items[i]["Address"];
			String name = items[i]["Name"];
			DeviceAddress address;
			for (int j = 0; j < addressJson.size(); j++)
			{
				address[j] = addressJson[j];
			}

			uint8_t digitTarget = items[i]["Target"];
			TemperatureSensorTarget target = static_cast<TemperatureSensorTarget>(digitTarget);

			_ds18b20Configuration->init(address, name, target);
			_ds18b20Items->add(_ds18b20Configuration);
		}
	}
	_ds18b20ListConfiguration->init(pin, temperaturePrecision, _ds18b20Items);

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
			int8_t setToMaxWhenChillerLoad = items[i]["SetToMaxWhenChillerLoad"];
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
			pwmConfiguration->init(tachoPin, pwmPin, name, setToMaxWhenChillerLoad, pwmValues, controlType);
			pwmItems->add(pwmConfiguration);
		}
		pwmsConfiguration->init(pwmItems);
	}

	return pwmsConfiguration;
}

#pragma endregion Configurations

#pragma region SoftReset

Communication::Models::Requests::Commands::SoftResetCommand* Services::JsonService::deserializeSoftResetCommand()
{
	return new Communication::Models::Requests::Commands::SoftResetCommand();
}

#pragma endregion SoftReset

#pragma region UpdateConfigurationCommand

Communication::Models::Requests::Commands::UpdateConfigurationCommand* Services::JsonService::deserializeUpdateConfigurationCommand(
	JsonObject data)
{
	return new Communication::Models::Requests::Commands::UpdateConfigurationCommand(deserializeConfiguration(data));
}

#pragma endregion UpdateConfigurationCommand

#pragma region UpdateTempConfigurationCommand

Communication::Models::Requests::Commands::UpdateTempConfigurationCommand* Services::JsonService::deserializeUpdateTempConfigurationCommand(JsonObject data)
{
	return new Communication::Models::Requests::Commands::UpdateTempConfigurationCommand(deserializeConfiguration(data));
}

#pragma endregion UpdateTempConfigurationCommand

#pragma region ResetTempConfigurationCommand

Communication::Models::Requests::Commands::ResetTempConfigurationCommand* Services::JsonService::deserializeResetTempConfigurationCommand()
{
	return new Communication::Models::Requests::Commands::ResetTempConfigurationCommand();
}

#pragma endregion ResetTempConfigurationCommand

#pragma endregion Deserializers
