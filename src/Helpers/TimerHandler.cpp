#include "TimerHandler.h"

Helpers::TimerHandler::TimerHandler(const tm &interruptTime)
{
    _interruptTime = interruptTime;
}

tm Helpers::TimerHandler::getInterruptTime()
{
    return _interruptTime;
}
