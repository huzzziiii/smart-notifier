#ifndef __SYSTEM_TASK_H
#define __SYSTEM_TASK_H

#include <stdbool.h>
#include <stdarg.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <uart.hpp>

class SystemTask 
{   
    TaskHandle_t taskHandle;
    QueueHandle_t &systemTaskQueue;
    uint8_t msg;			    // stores the received queue data
    static void process(void* arg);
    void mainThread();
    
    
    // peripheral
    Uart& uart;
    public:
    enum class Messages
    {
        enableTempNotification,
        disableTempNotification,
        invalidInput
    };
    static Messages messages;
    SystemTask(Uart &uart, QueueHandle_t &systemTask);
    void pushMessage(SystemTask::Messages msg);

};
#endif
