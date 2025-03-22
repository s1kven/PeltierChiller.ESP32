#pragma once
#ifndef _TimerHandler_
#define _TimerHandler_ 

#include "Abstractions/BaseTimerCallback.h"
#include "TimerInfo.h"

namespace Helpers
{
	class TimerHandler
	{
    private:
        Abstractions::BaseTimerCallback* _onTimerInstance;
        struct tm _interruptTime;
        struct TimerInfo _timerInfo;

	public:
        TimerHandler(Abstractions::BaseTimerCallback* onTimerInstance, const struct tm &interruptTime, const struct TimerInfo &timerInfo);

        Abstractions::BaseTimerCallback* getOnTimerInstance();
        struct tm getInterruptTime();
        struct TimerInfo getTimerInfo();
	};
}
#endif

