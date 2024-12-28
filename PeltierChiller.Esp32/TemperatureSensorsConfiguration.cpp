#include "TemperatureSensorsConfiguration.h"

void Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration::init()
{
}

void Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration::init(
	Bme280ListConfiguration* bmeListConfiguration, NtcListConfiguration* ntcListConfiguration, 
	Ds18b20ListConfiguration* ds18b20ListConfiguration)
{
	_bmeListConfiguration = bmeListConfiguration;
	_ntcListConfiguration = ntcListConfiguration;
	_ds18b20ListConfiguration = ds18b20ListConfiguration;
	uint16_t resultedPayloadSize = _payloadSize
		+ _bmeListConfiguration->getJsonSize()
		+ _ntcListConfiguration->getJsonSize()
		+ _ds18b20ListConfiguration->getJsonSize();
	Communication::Abstractions::BaseSerializableObject::setJsonSize(resultedPayloadSize);
}

void Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration::clear()
{
	_bmeListConfiguration->clear();
	delete _bmeListConfiguration;
	_ntcListConfiguration->clear();
	delete _ntcListConfiguration;
	_ds18b20ListConfiguration->clear();
	delete _ds18b20ListConfiguration;
}

Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration* 
Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration::getBmeListConfiguration()
{
	return _bmeListConfiguration;
}

Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration*
Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration::getNtcListConfiguration()
{
	return _ntcListConfiguration;
}

Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration*
Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration::getDs18b20ListConfiguration()
{
	return _ds18b20ListConfiguration;
}

DynamicJsonDocument Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());

	JsonObject bme280Configuration = document.createNestedObject("BME280");
	bme280Configuration.set(_bmeListConfiguration->createPayload().as<JsonObjectConst>());

	JsonObject ntcConfiguration = document.createNestedObject("NTC");
	ntcConfiguration.set(_ntcListConfiguration->createPayload().as<JsonObjectConst>());

	JsonObject ds18b20Configuration = document.createNestedObject("DS18B20");
	ds18b20Configuration.set(_ds18b20ListConfiguration->createPayload().as<JsonObjectConst>());

	return document;
}
