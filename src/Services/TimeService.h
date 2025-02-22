#pragma once
#ifndef _TimeService_
#define _TimeService_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "time.h"
#include "esp_timer.h"
#include "Abstractions/BaseTimerCallback.h"

extern uint64_t _initMillis;

namespace Services
{
    class TimeService
    {
    private:
        String _ntpServer;
        bool _isTimeConfigured = false;
        struct tm _timeInfo;

        struct tm getEmptyTime();
		static void IRAM_ATTR onTimer(void* arg);
        time_t timeToSeconds(uint64_t hours, uint64_t minutes, uint64_t seconds);

    public:
        TimeService(String ntpServer);

        void configTime();
        struct tm getCurrentTime();
        struct tm getStartupTime();
        bool isValidTime(tm time);
        void convertToTmStruct(uint16_t date, uint16_t time, struct tm &tm);
        struct tm subtractMilliseconds(const tm &timeStruct, uint64_t milliseconds);
        void startTimer(Abstractions::BaseTimerCallback* instance, struct tm &time);
    };
}
#endif