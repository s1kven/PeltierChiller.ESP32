#pragma once
#ifndef _TimeService_
#define _TimeService_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "esp_task_wdt.h"

#include "time.h"
#include "driver/timer.h"
#include "Helpers/TimerInfo.h"

extern uint64_t _initMillis;

namespace Services
{
	class LogService;
}

namespace Services
{
    class TimeService
    {
    private:
        static const Helpers::TimerInfo _logTimerInfo;
		static bool _isLogTimerInit;

        String _ntpServer;
        bool _isTimeConfigured = false;
        struct tm _timeInfo;

        struct tm getEmptyTime();
		static void IRAM_ATTR onLogTimer(void* arg);
        time_t timeToSeconds(uint64_t hours, uint64_t minutes, uint64_t seconds);
        static void releaseLogTimer();

    public:
        TimeService(String ntpServer);
        ~TimeService();

        void configTime();
        struct tm getCurrentTime();
        struct tm getStartupTime();
        bool isValidTime(tm time);
        void convertToTmStruct(uint16_t date, uint16_t time, struct tm &tm);
        struct tm subtractMilliseconds(const tm &timeStruct, uint64_t milliseconds);
        bool startLogTimer(const struct tm &time);
    };
}
#endif