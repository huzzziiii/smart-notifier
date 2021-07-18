#ifndef __NRFLOGGER_H
#define __NRFLOGGER_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_drv_twi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stream_buffer.h"
#include <stdarg.h>
class NrfLogger 
{
    static TaskHandle_t loggerThread;
    static StreamBufferHandle_t logStreamBufferx;
    static uint8_t buffer[50]; // todo  -- know SIZE in compile time for storing chars?!

    public:
    NrfLogger();
    static void Process(void *arg);
    const uint8_t *getBuffer() const;

    template<typename T>
    static void writeToLogger(const T *data, ...)
    {
        char tmp[40] = {0};
        va_list args;
        va_start(args, data);
        vsprintf (tmp, data, args);
        xStreamBufferSend(logStreamBufferx, (void*)&tmp, 40, pdFALSE);
    }
};
#endif
