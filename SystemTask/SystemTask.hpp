#ifndef __SYSTEM_TASK_H
#define __SYSTEM_TASK_H

#include <stdbool.h>
#include <stdarg.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <uart.hpp>
#include "mcp9808.hpp"
#include "BleUartService.hpp"
//#include "BleCommon.hpp"
//#include "ble_nus.h"
#include <NotificationManager.hpp>

class SystemTask 
{   
    
    QueueHandle_t &systemTaskQueue;
    uint8_t msg;			    // stores the received queue data
    
    static void process(void* arg);
    
    // peripherals
    Uart& uart;
    MCP9808 &_tmpSensor;

    NotificationManager &_notificationManager;
    //BleUartService &_bleService;

    public:
    TaskHandle_t taskHandle;
    
    void mainThread();
    enum class Messages
    {
        subscribeTempNotifications,
        unsubscribeTempNotifications,
        invalidInput
    };
    static Messages messages;
    //SystemTask(Uart &uart, MCP9808 &tmpSensor, NotificationManager &notificationManager, QueueHandle_t &systemTask, BleUartService &bleService);
    SystemTask(Uart &uart, MCP9808 &tmpSensor, NotificationManager &notificationManager, QueueHandle_t &systemTask);
    void pushMessage(SystemTask::Messages msg);

};
#endif
