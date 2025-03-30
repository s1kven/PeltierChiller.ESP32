#include "TimeService.h"
#include "Services/LogService.h"

#define FAT_YEAR(date) (((date) >> 9) + 1980)
#define FAT_MONTH(date) (((date) >> 5) & 0x0F)
#define FAT_DAY(date) ((date) & 0x1F)
#define FAT_HOUR(time) ((time) >> 11)
#define FAT_MINUTE(time) (((time) >> 5) & 0x3F)
#define FAT_SECOND(time) (((time) & 0x1F) * 2)

Services::TimeService::TimeService(String ntpServer)
{
    _ntpServer = ntpServer;
}

Services::TimeService::~TimeService()
{
    releaseLogTimer();
}

void Services::TimeService::configTime()
{
    if (!_isTimeConfigured)
    {
        ::configTime(0, 0, _ntpServer.c_str());
        _isTimeConfigured = true;
    }
}

struct tm Services::TimeService::getCurrentTime()
{
    if (!_isTimeConfigured || !getLocalTime(&_timeInfo))
    {
        return getEmptyTime();
    }
    return _timeInfo;
}

struct tm Services::TimeService::getStartupTime()
{
    struct tm currentTime = getCurrentTime();
    if (isValidTime(currentTime))
    {
        return subtractMilliseconds(currentTime, millis() - _initMillis);
    }
    return getEmptyTime();
}

bool Services::TimeService::isValidTime(tm time)
{
    struct tm emptyTime = getEmptyTime();
    if (time.tm_hour == emptyTime.tm_hour && time.tm_isdst == emptyTime.tm_isdst 
        && time.tm_mday == emptyTime.tm_mday && time.tm_min == emptyTime.tm_min 
        && time.tm_mon == emptyTime.tm_mon && time.tm_sec == emptyTime.tm_sec 
        && time.tm_wday == emptyTime.tm_wday && time.tm_yday == emptyTime.tm_yday && time.tm_year == emptyTime.tm_year)
    {
        return false;
    }
    return true;
}

void Services::TimeService::convertToTmStruct(uint16_t date, uint16_t time, struct tm &tm)
{
    tm.tm_year = FAT_YEAR(date) - 1900;
    tm.tm_mon = FAT_MONTH(date) - 1;
    tm.tm_mday = FAT_DAY(date);
    tm.tm_hour = FAT_HOUR(time);
    tm.tm_min = FAT_MINUTE(time);
    tm.tm_sec = FAT_SECOND(time);
}

struct tm Services::TimeService::getEmptyTime()
{
    return tm{
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0};
}

void IRAM_ATTR Services::TimeService::onLogTimer(void* arg)
{
    releaseLogTimer();
    Services::LogService::setLogFileInit(false);
    Services::LogService::setOnTimer(true);
}

time_t Services::TimeService::timeToSeconds(uint64_t hours, uint64_t minutes, uint64_t seconds)
{
    return (hours * 3600) + (minutes * 60) + seconds;
}

void Services::TimeService::releaseLogTimer()
{
    if(_isLogTimerInit)
    {
        timer_pause(_logTimerInfo.timerGroup, _logTimerInfo.timerIdx);
        timer_disable_intr(_logTimerInfo.timerGroup, _logTimerInfo.timerIdx);
        timer_group_clr_intr_status_in_isr(_logTimerInfo.timerGroup, _logTimerInfo.timerIdx);
        timer_isr_callback_remove(_logTimerInfo.timerGroup, _logTimerInfo.timerIdx);
        timer_deinit(_logTimerInfo.timerGroup, _logTimerInfo.timerIdx);
        _isLogTimerInit = false;
    }
}

struct tm Services::TimeService::subtractMilliseconds(const tm &timeStruct, uint64_t milliseconds)
{
    time_t time = mktime(const_cast<tm *>(&timeStruct));
    int seconds = milliseconds / 1000;
    time -= seconds;
    tm *newTimeStruct = localtime(&time);

    return *newTimeStruct;
}

bool Services::TimeService::startLogTimer(const struct tm &time)
{
    time_t dailyTime = timeToSeconds(24, 0, 0);
    time_t interruptTime = timeToSeconds(time.tm_hour, time.tm_min, time.tm_sec);
    struct tm currentTimeInfo = getCurrentTime();
    time_t currentTime = timeToSeconds(currentTimeInfo.tm_hour, currentTimeInfo.tm_min, currentTimeInfo.tm_sec);
    time_t waitTime;
    if(currentTime < interruptTime)
    {
        waitTime = interruptTime - currentTime;
    }
    else
    {
        waitTime = (interruptTime + dailyTime) - currentTime;
    }

    const timer_config_t config = {
        .alarm_en = TIMER_ALARM_EN,
        .counter_en = TIMER_PAUSE,
        .intr_type = TIMER_INTR_LEVEL,
        .counter_dir = TIMER_COUNT_UP,
        .auto_reload = TIMER_AUTORELOAD_DIS,
        .divider = 80
    };

    bool init = false;
    uint64_t waitMicros = (uint64_t)waitTime * (uint64_t)1000000;
    int8_t initResult = timer_init(_logTimerInfo.timerGroup, _logTimerInfo.timerIdx, &config);
    if(initResult == 0)
    {
        init = true;
    }
    timer_set_counter_value(_logTimerInfo.timerGroup, _logTimerInfo.timerIdx, 0x00000000ULL);
    timer_set_alarm_value(_logTimerInfo.timerGroup, _logTimerInfo.timerIdx, waitMicros);
    timer_enable_intr(_logTimerInfo.timerGroup, _logTimerInfo.timerIdx);
    timer_isr_register(_logTimerInfo.timerGroup, _logTimerInfo.timerIdx, onLogTimer, NULL, ESP_INTR_FLAG_IRAM, NULL);
    timer_start(_logTimerInfo.timerGroup, _logTimerInfo.timerIdx);
    _isLogTimerInit = init;
    return init;
}
