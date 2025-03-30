#pragma once
#ifndef _WifiService_
#define _WifiService_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <esp_task_wdt.h>
#include <WiFi.h>
#include "Communication/Models/Configurations/WifiConfiguration.h"
#include "TimeService.h"

extern Services::TimeService* _timeService;

namespace Services
{
    class WifiService
    {
    private:
        bool _isFirstConnection = true;
        String _ssid;
        String _password;
        uint32_t _reconnectionTimeout;

        bool isSuccessfullyConfigured();

    public:
        WifiService(Communication::Models::Configurations::WifiConfiguration* wifiConfig);
        ~WifiService();

        void tryConnect();
        bool isConnected();
        uint32_t getReconnectionTimeout();
    };
}
#endif