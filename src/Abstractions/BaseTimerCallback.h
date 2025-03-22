#pragma once
#ifndef _BaseTimerCallback_
#define _BaseTimerCallback_ 

#include "driver/timer.h"
#include "Helpers/TimerInfo.h"

namespace Abstractions
{
    class BaseTimerCallback
    {
    private:
        Helpers::TimerInfo _timerInfo;

    protected:
        virtual void setTimerInfo(timer_group_t timerGroup, timer_idx_t timerIdx);

    public:
        virtual void onTimer() = 0;
        virtual Helpers::TimerInfo& getTimerInfo();
        virtual void releaseCurrentTimer();
    };
}
#endif