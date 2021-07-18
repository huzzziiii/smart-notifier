#include "TimerApp.hpp"

TimerReturnCode Timer::create(TimerMode timerMode, const char *timerName, void *timerID, uint32_t blockTimeInMs, void (*timerCb)(TimerHandle_t xTimer))
{   
    TimerReturnCode retCode = timerSuccess;

    _timerMode = timerMode == singleShot ? pdFALSE : pdTRUE;
    _timerCb = timerCb;
    _timerHandler = xTimerCreate(timerName, pdMS_TO_TICKS(blockTimeInMs), _timerMode, (void *) 0, timerCb);
    
    if (_timerHandler == NULL)
    {
        return timerFailedToCreate;
    }
    return retCode;
}

// TODO - set active member as volatile and set it to xTimerIsTimerActive() -- verify!

TimerReturnCode Timer::start(TimerHandle_t timerHandle, uint32_t blockTimeInMs)
{
    TimerReturnCode retCode = timerSuccess;
    if (xTimerIsTimerActive(timerHandle) != pdFALSE)	// check if timer is already active
    {
        NRF_LOG_INFO("Timer %s is already active...!\n", pcTimerGetTimerName(timerHandle));
        return timerIsAlreadyActive;
    }
    
    if (__get_IPSR() != 0)	  // check if the context this function was called was within an ISR
    {
        BaseType_t yieldReq = pdFALSE;

    }
    else
    {
        if (xTimerStart(_timerHandler, pdMS_TO_TICKS(blockTimeInMs)) != pdPASS)
        {
	  NRF_LOG_INFO("Timer %s failed to start...!\n", pcTimerGetTimerName(timerHandle));
	  return timerFailedToStart;
        }
    }
    return retCode;
}


TimerReturnCode Timer::changeTimerPeriod(TimerHandle_t timerHandle, uint32_t newPeriodTime, uint32_t blockTimeInMs)
{
    //xTimerChangePeriod(timerReturn, above21/portTICK_PERIOD_MS, 0) != pdTRUE)
    if (xTimerIsTimerActive(timerHandle) != pdFALSE)	// check if timer is already active
    {
        NRF_LOG_INFO("Timer %s is already active...couldn't change the period!\n", pcTimerGetTimerName(timerHandle));
        NRF_LOG_FLUSH();
        xTimerDelete(timerHandle, 0);
        return timerIsAlreadyActive;
    }

    if (__get_IPSR() != 0)  // if an interrupt is set
    {
        // TODO...
        int m = 0;
        m++;
    }
    else 
    {
        if (xTimerChangePeriod(timerHandle, pdMS_TO_TICKS(newPeriodTime), pdMS_TO_TICKS(blockTimeInMs)) != pdTRUE)
        {
	  return timerFailedToChangePeriod;
        }
    }
    return timerSuccess;

}

