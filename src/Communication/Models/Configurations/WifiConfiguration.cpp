#include "WifiConfiguration.h"

void Communication::Models::Configurations::WifiConfiguration::init()
{
}

void Communication::Models::Configurations::WifiConfiguration::init(String ssid, String password, uint32_t reconnectionTimeout, String ntpServer)
{
    _ssid = ssid;
    _password = password;
    _reconnectionTimeout = reconnectionTimeout;
    _ntpServer = ntpServer;
    uint16_t resultedPayloadSize = _payloadSize + JSON_STRING_SIZE(_ssid.length()) + JSON_STRING_SIZE(_password.length()) + JSON_STRING_SIZE(_ntpServer.length());
	Communication::Abstractions::BaseSerializableObject::setJsonSize(resultedPayloadSize);
}

String Communication::Models::Configurations::WifiConfiguration::getSsid()
{
    return _ssid;
}

String Communication::Models::Configurations::WifiConfiguration::getPassword()
{
    return _password;
}

uint32_t Communication::Models::Configurations::WifiConfiguration::getReconnectionTimeout()
{
    return _reconnectionTimeout;
}

String Communication::Models::Configurations::WifiConfiguration::getNtpServer()
{
    return _ntpServer;
}

DynamicJsonDocument Communication::Models::Configurations::WifiConfiguration::createPayload()
{
	DynamicJsonDocument document(Communication::Abstractions::BaseSerializableObject::getJsonSize());
    JsonObject payload = document.to<JsonObject>();

	payload["SSID"] = _ssid;
	payload["Password"] = _password;
	payload["ReconnectionTimeout"] = _reconnectionTimeout;
	payload["NtpServer"] = _ntpServer;
	return document;
}
