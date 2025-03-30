#pragma once
#ifndef _TimerHandler_
#define _TimerHandler_ 

#include "TimerInfo.h"

namespace Helpers
{
    class TimerHandler
    {
    private:
        struct tm _interruptTime;

	public:
        TimerHandler(const struct tm &interruptTime);

        struct tm getInterruptTime();
    };
}
#endif

