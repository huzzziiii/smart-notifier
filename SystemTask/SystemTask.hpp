#ifndef __SYSTEM_TASK_H
#define __SYSTEM_TASK_H

#include <stdbool.h>
#include <stdarg.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <uart.hpp>
#include "mcp9808.hpp"
//#include "BleUartService.hpp"
// TODO -- #include "bleApp.hpp"
//#include "BleCommon.hpp"
//#include "ble_nus.h"
#include "NotificationManager.hpp"


class SystemTask 
{   
    QueueHandle_t &systemTaskQueue;
    uint8_t msg;			    // stores the received queue data

    // peripherals
    Uart &uart;
    MCP9808 &_tmpSensor;

    NotificationManager &_notificationManager;
    //BleUartService &_bleService;

    static void process(void* arg);

    public:

    TaskHandle_t taskHandle;
    enum class Messages
    {
        subscribeTempNotifications,
        unsubscribeTempNotifications,
        invalidInput
    };
    static Messages messages;
    //SystemTask(Uart &uart, MCP9808 &tmpSensor, NotificationManager &notificationManager, QueueHandle_t &systemTask, BleUartService &bleService);
    
    SystemTask(Uart &uart, MCP9808 &tmpSensor, NotificationManager &notificationManager, QueueHandle_t &systemTask);
    void pushMessage(SystemTask::Messages msg, bool fromISR = true);
    void mainThread();
};

SystemTask::Messages convertParsedInputToMsg(const char *inputToCmp);

#define ENUM_ENTRY(x) { x, #x }

struct Lookup
{
    SystemTask::Messages message;
    const char* name;
};

using Messages = SystemTask::Messages;
 
static Lookup lookupTable[3] = {
  ENUM_ENTRY(Messages::subscribeTempNotifications),
  ENUM_ENTRY(Messages::unsubscribeTempNotifications),
  ENUM_ENTRY(Messages::invalidInput)
};


#endif
