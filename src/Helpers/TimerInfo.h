#pragma once
#ifndef _TimerInfo_
#define _TimerInfo_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "driver/timer.h"

namespace Helpers
{
    struct TimerInfo 
    {
        timer_group_t timerGroup;
        timer_idx_t timerIdx;
    };
}
#endif