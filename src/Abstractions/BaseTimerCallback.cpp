#include "BaseTimerCallback.h"

void Abstractions::BaseTimerCallback::setCurrentTimer(esp_timer_handle_t timer)
{
    _currentTimer = timer;
}

esp_timer_handle_t Abstractions::BaseTimerCallback::getCurrentTimer()
{
    return _currentTimer;
}

void Abstractions::BaseTimerCallback::releaseCurrentTimer()
{
    if(_currentTimer != nullptr)
    {
        esp_timer_stop(_currentTimer);
        esp_timer_delete(_currentTimer);
    }
}
