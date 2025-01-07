#pragma once
#ifndef _TimeService_
#define _TimeService_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "time.h"

namespace Services
{
    class TimeService
    {
    private:
        String _ntpServer;
        bool _isTimeConfigured = false;
        struct tm _timeInfo;

        tm getEmptyTime();

    public:
        TimeService(String ntpServer);

        void configTime();
        tm getCurrentTime();
        bool isValidTime(tm time);
    };
}
#endif