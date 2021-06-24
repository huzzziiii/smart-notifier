#ifndef __SYSTEM_TASK_H
#define __SYSTEM_TASK_H

#include <stdbool.h>
#include <stdarg.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <uart.hpp>
#include <mcp9808.hpp>
#include <NotificationManager.hpp>

class SystemTask 
{   
    TaskHandle_t taskHandle;
    QueueHandle_t &systemTaskQueue;
    uint8_t msg;			    // stores the received queue data
    static void process(void* arg);
    void mainThread();
    
    
    // peripherals
    Uart& uart;
    MCP9808 &_tmpSensor;

    NotificationManager &_notificationManager;

    public:
    enum class Messages
    {
        subscribeTempNotifications,
        unsubscribeTempNotifications,
        invalidInput
    };
    static Messages messages;
    SystemTask(Uart &uart, MCP9808 &tmpSensor, NotificationManager &notificationManager, QueueHandle_t &systemTask);
    void pushMessage(SystemTask::Messages msg);

};
#endif
