#include "TimeService.h"

Services::TimeService::TimeService(String ntpServer)
{
    _ntpServer = ntpServer;
}

void Services::TimeService::configTime()
{
    if(!_isTimeConfigured)
    {
        ::configTime(0, 0, _ntpServer.c_str());
        _isTimeConfigured = true;
    }
}

tm Services::TimeService::getCurrentTime()
{
    if(!_isTimeConfigured || !getLocalTime(&_timeInfo))
    {
        return getEmptyTime();
    }
    return _timeInfo;
}

bool Services::TimeService::isValidTime(tm time)
{
    tm emptyTime = getEmptyTime();
    if(time.tm_hour == emptyTime.tm_hour && time.tm_isdst == emptyTime.tm_isdst && time.tm_mday == emptyTime.tm_mday
    && time.tm_min == emptyTime.tm_min && time.tm_mon == emptyTime.tm_mon && time.tm_sec == emptyTime.tm_sec
    && time.tm_wday == emptyTime.tm_wday && time.tm_yday == emptyTime.tm_yday && time.tm_year == emptyTime.tm_year)
    {
        return false;
    }
    return true;
}

tm Services::TimeService::getEmptyTime()
{
    return tm
    {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0
    };
}
