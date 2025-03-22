#include "BaseTimerCallback.h"

void Abstractions::BaseTimerCallback::setTimerInfo(timer_group_t timerGroup, timer_idx_t timerIdx)
{
    _timerInfo.timerGroup = timerGroup;
    _timerInfo.timerIdx = timerIdx;
}

Helpers::TimerInfo& Abstractions::BaseTimerCallback::getTimerInfo()
{
    return _timerInfo;
}

void Abstractions::BaseTimerCallback::releaseCurrentTimer()
{
    timer_disable_intr(_timerInfo.timerGroup, _timerInfo.timerIdx);
    timer_pause(_timerInfo.timerGroup, _timerInfo.timerIdx);
    timer_group_clr_intr_status_in_isr(_timerInfo.timerGroup, _timerInfo.timerIdx);
}
