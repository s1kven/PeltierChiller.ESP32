#include "Configuration.h"

ChillerType Communication::Models::Configurations::Configuration::getChillerType()
{
	return _chillerType;
}

float Communication::Models::Configurations::Configuration::getTargetCircuitTemperature()
{
	return _targetCircuitTemperature;
}

float Communication::Models::Configurations::Configuration::getVoltmeterThreshold()
{
	return _voltmeterThreshold;
}

uint32_t Communication::Models::Configurations::Configuration::getVoltmeterR1()
{
	return _voltmeterR1;
}

uint32_t Communication::Models::Configurations::Configuration::getVoltmeterR2()
{
	return _voltmeterR2;
}

bool Communication::Models::Configurations::Configuration::getIsDelayEnablingPc()
{
	return _isDelayEnablingPc;
}

Communication::Models::Configurations::PinsConfiguration* Communication::Models::Configurations::Configuration::getPinsConfiguration()
{
	return _pinsConfiguration;
}

Communication::Models::Configurations::TimersConfiguration* Communication::Models::Configurations::Configuration::getTimersConfiguration()
{
	return _timersConfiguration;
}

Communication::Models::Configurations::ChillerConfiguration* Communication::Models::Configurations::Configuration::getChillerConfiguration()
{
	return _chillerConfiguration;
}

Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* 
Communication::Models::Configurations::Configuration::getTemperatureSensorsConfiguration()
{
	return _temperatureSensorsConfiguration;
}

Communication::Models::Configurations::PwmsConfiguration* 
Communication::Models::Configurations::Configuration::getPwmsConfiguration()
{
	return _pwmsConfiguration;
}

Communication::Models::Configurations::WifiConfiguration *Communication::Models::Configurations::Configuration::getWifiConfiguration()
{
    return _wifiConfiguration;
}

DynamicJsonDocument Communication::Models::Configurations::Configuration::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());
	JsonObject payload = document.to<JsonObject>();

	JsonObject pinsConfiguration = payload.createNestedObject("Pins");
	pinsConfiguration.set(_pinsConfiguration->createPayload().as<JsonObjectConst>());

	payload["ChillerType"] = static_cast<uint16_t>(_chillerType);
	payload["TargetTemperature"] = serialized(String(_targetCircuitTemperature, 1));
	payload["VoltmeterThreshold"] = serialized(String(_voltmeterThreshold, 1));
	payload["VoltmeterR1"] = _voltmeterR1;
	payload["VoltmeterR2"] = _voltmeterR2;
	payload["StartupPcAfterFeatTargetTemperature"] = _isDelayEnablingPc;

	JsonObject chillerSettingsConfiguration = payload.createNestedObject("ChillerSettings");
	chillerSettingsConfiguration.set(_chillerConfiguration->createPayload().as<JsonObjectConst>());

	JsonObject timersConfiguration = payload.createNestedObject("Timers");
	timersConfiguration.set(_timersConfiguration->createPayload().as<JsonObjectConst>());

	JsonObject temperatureSensorsConfiguration = payload.createNestedObject("TemperatureSensors");
	temperatureSensorsConfiguration.set(_temperatureSensorsConfiguration->createPayload().as<JsonObjectConst>());

	JsonArray pwmsConfiguration = payload.createNestedArray("PWMs");
	pwmsConfiguration.set(_pwmsConfiguration->createPayload().as<JsonArrayConst>());

	JsonObject wifiConfiguration = payload.createNestedObject("Wifi");
	wifiConfiguration.set(_wifiConfiguration->createPayload().as<JsonObjectConst>());

	return document;
}

void Communication::Models::Configurations::Configuration::init()
{
}

void Communication::Models::Configurations::Configuration::init(ChillerType chillerType, float targetCircuitTemperature,
	float voltmeterThreshold, uint32_t voltmeterR1, uint32_t voltmeterR2,
	bool isDelayEnablingPc, Communication::Models::Configurations::PinsConfiguration* pinsConfiguration,
	Communication::Models::Configurations::TimersConfiguration* timersConfiguration,
	Communication::Models::Configurations::ChillerConfiguration* chillerConfiguration,
	Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration* temperatureSensorsConfiguration,
	Communication::Models::Configurations::PwmsConfiguration* pwmsConfiguration,
	Communication::Models::Configurations::WifiConfiguration* wifiConfiguration)
{
	_chillerType = chillerType;
	_targetCircuitTemperature = targetCircuitTemperature;
	_voltmeterThreshold = voltmeterThreshold;
	_voltmeterR1 = voltmeterR1;
	_voltmeterR2 = voltmeterR2;
	_isDelayEnablingPc = isDelayEnablingPc;
	_pinsConfiguration = new Communication::Models::Configurations::PinsConfiguration(*pinsConfiguration);
	_timersConfiguration = new Communication::Models::Configurations::TimersConfiguration(*timersConfiguration);
	_chillerConfiguration = new Communication::Models::Configurations::ChillerConfiguration(*chillerConfiguration);
	_temperatureSensorsConfiguration 
		= new Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration(*temperatureSensorsConfiguration);
	_pwmsConfiguration = new Communication::Models::Configurations::PwmsConfiguration(*pwmsConfiguration);
	_wifiConfiguration = new Communication::Models::Configurations::WifiConfiguration(*wifiConfiguration);

	uint16_t resultedPayloadSize = _payloadSize
		+ _pinsConfiguration->getJsonSize()
		+ _chillerConfiguration->getJsonSize()
		+ _timersConfiguration->getJsonSize()
		+ _temperatureSensorsConfiguration->getJsonSize()
		+ _pwmsConfiguration->getJsonSize()
		+ _wifiConfiguration->getJsonSize()
		+ Helpers::JsonHelper::getFloatJsonSizeWorkaround(2);
	Communication::Abstractions::BaseSerializableObject::setJsonSize(resultedPayloadSize);

	delete pinsConfiguration;
	delete timersConfiguration;
	delete chillerConfiguration;
	delete temperatureSensorsConfiguration;
	delete pwmsConfiguration;
	delete wifiConfiguration;
}

void Communication::Models::Configurations::Configuration::clear()
{
	delete _pinsConfiguration;
	delete _timersConfiguration;
	delete _chillerConfiguration;
	_temperatureSensorsConfiguration->clear();
	delete _temperatureSensorsConfiguration;
	_pwmsConfiguration->clear();
	delete _pwmsConfiguration;
	delete _wifiConfiguration;
}
