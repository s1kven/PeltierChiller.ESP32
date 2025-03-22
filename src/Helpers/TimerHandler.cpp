#include "TimerHandler.h"

Helpers::TimerHandler::TimerHandler(Abstractions::BaseTimerCallback *onTimerInstance, const tm &interruptTime, const TimerInfo &timerInfo)
{
    _onTimerInstance = onTimerInstance;
    _interruptTime = interruptTime;
    _timerInfo = timerInfo;
}

Abstractions::BaseTimerCallback *Helpers::TimerHandler::getOnTimerInstance()
{
    return _onTimerInstance;
}

tm Helpers::TimerHandler::getInterruptTime()
{
    return _interruptTime;
}

Helpers::TimerInfo Helpers::TimerHandler::getTimerInfo()
{
    return _timerInfo;
}
