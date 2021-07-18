#ifndef __TIMER_APP_H
#define __TIMER_APP_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "stdint.h"
//#include "SystemTask.hpp"
#include "FreeRTOS.h"
#include "timers.h"
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

// TODO ... V

#define APP_TIMER_CLOCK_FREQ            32768                     /**< Clock frequency of the RTC timer used to implement the app timer module. */
#define APP_TIMER_MIN_TIMEOUT_TICKS     5    

#define APP_TIMER_TICKS(MS)                                \
            ((uint32_t)ROUNDED_DIV(                        \
            (MS) * (uint64_t)APP_TIMER_CLOCK_FREQ,         \
            1000 * (APP_TIMER_CONFIG_RTC_FREQUENCY + 1)))

typedef enum 
{
    singleShot,
    autoReload
} TimerMode;

//void (*timerHandler)(

typedef enum
{
    timerSuccess,
    timerFailedToCreate,
    timerFailedToStart,
    timerIsAlreadyActive,
    timerFailedToChangePeriod
} TimerReturnCode;

class Timer
{
    BaseType_t _timerMode;
    
    void (*_timerCb)(TimerHandle_t xTimer);

    public:
    TimerHandle_t _timerHandler;

    //void create(TimerMode timerMode, const char *timerName, void *timerID, void (*timerHandler)(void));
    TimerReturnCode create(TimerMode timerMode, const char *timerName, void *timerID, uint32_t blockTimeInMs, void (*timerCb)(TimerHandle_t xTimer));
    //void create(TimerMode timerMode, const char *timerName, void *timerID, void (*timerCb)());
    TimerReturnCode start(TimerHandle_t timerHandle, TickType_t ticksToWait);

    TimerReturnCode changeTimerPeriod(TimerHandle_t timerHandle, uint32_t newPeriodTime, uint32_t blockTimeInMs);
};

#endif
