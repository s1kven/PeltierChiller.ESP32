#pragma once
#ifndef _BaseTimerCallback_
#define _BaseTimerCallback_ 

#include "esp_timer.h"

namespace Abstractions
{
    class BaseTimerCallback
    {
    private:
        esp_timer_handle_t _currentTimer = nullptr;
    public:
        virtual void onTimer() = 0;
        virtual void setCurrentTimer(esp_timer_handle_t timer);
        virtual esp_timer_handle_t getCurrentTimer();
        virtual void releaseCurrentTimer();
    };
}
#endif