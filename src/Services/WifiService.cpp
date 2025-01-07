#include "WifiService.h"

Services::WifiService::WifiService(Communication::Models::Configurations::WifiConfiguration *wifiConfig)
{
    _ssid = wifiConfig->getSsid();
    _password = wifiConfig->getPassword();
    _reconnectionTimeout = wifiConfig->getReconnectionTimeout();
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
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);
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
