#include "WifiService.h"

Services::WifiService::WifiService(Communication::Models::Configurations::WifiConfiguration *wifiConfig)
{
    _ssid = wifiConfig->getSsid();
    _password = wifiConfig->getPassword();
    _reconnectionTimeout = wifiConfig->getReconnectionTimeout();
}

Services::WifiService::~WifiService()
{
    WiFi.disconnect();
}

void Services::WifiService::tryConnect()
{
    if(!isSuccessfullyConfigured())
    {
        return;
    }

    if(isConnected())
    {
        _timeService->configTime();
        return;
    }
    if(_isFirstConnection)
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(_ssid, _password);
        _isFirstConnection = false;
    }
    if(WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(_ssid, _password);
    }
    esp_task_wdt_reset();
}

uint32_t Services::WifiService::getReconnectionTimeout()
{
    return _reconnectionTimeout;
}

bool Services::WifiService::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

bool Services::WifiService::isSuccessfullyConfigured()
{
    if(_ssid.length() > 0 && _reconnectionTimeout > 100)
    {
        return true;
    }
    return false;
}
