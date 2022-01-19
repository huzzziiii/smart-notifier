#include "SystemTask.hpp"
#include "Observer.hpp"
#include "Subject.hpp" // TODO ---

/* todo -
 - what to do in enque() once queue == full?
    - override the data or wait till some vacancy?
*/

SystemTask *_systemTask;

SystemTask::SystemTask(Uart &pUart, MCP9808 &tmpSensor, NotificationManager &notificationManager, QueueHandle_t &systemQueue) : 
		   uart(pUart), _tmpSensor(tmpSensor), _notificationManager(notificationManager), systemTaskQueue(systemQueue)
{   
    size_t heapSize1 = xPortGetFreeHeapSize();
    _systemTask = this;

    // create a task
    if (xTaskCreate(SystemTask::process, "PROCESS", 100, this, 0, &taskHandle) != pdPASS)	  // TODO - think about stack size!
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    } 
      
    size_t heapSize2 = xPortGetFreeHeapSize();
    size_t heapTaken = heapSize1 - heapSize2;
}

void SystemTask::process(void *instance)
{
    auto pInstance = static_cast<SystemTask*>(instance);
    pInstance->mainThread();
}


void CustSrvDataHdlr(CustEvent *bleCustEvent)
{
    uint8_t rxdBytes[100] = {0};
    SystemTask::Messages systemMsg;

    for (uint8_t idx = 0; idx < bleCustEvent->rxData.rxdBytes; idx++)
    {
        rxdBytes[idx] = bleCustEvent->rxData.rxBuffer[idx];
    }
        
    uint8_t const *rxData = bleCustEvent->rxData.rxBuffer;
    //char const val = (char ) *rxData;
    
    // get an appropriate message out of the requested bytes
    systemMsg = convertParsedInputToMsg((char *) rxdBytes);

    // route the corresponding message to the system task to take an apt action
    _systemTask->pushMessage(systemMsg, true);
}

/**
@brief: main state machine that handles requests from the user 
@description: current supported requests: enableNotifications(<typeOfNotif>), disableNotifications(<typeOfNotif>), 
	    where <typeOfNotif> corresponds to any supported sensor
*/
void SystemTask::mainThread()
{   
    Messages curMsg;
  
    while(true)
    {
        if (xQueueReceive(systemTaskQueue, &msg, portMAX_DELAY) == pdPASS)      // wait for the user input over UART (for now!)
        {
	  curMsg = static_cast<Messages>(msg);		        
	  Lookup lookupVal = lookupTable[msg];
	  uart.PrintUart("Message received: %s", lookupVal.name); 
	  	       
	  switch(curMsg)
	  {
	      case Messages::subscribeTempNotifications:
		_notificationManager.subscribe(&_tmpSensor);
		break;
	      
	      case Messages::unsubscribeTempNotifications:
		_notificationManager.unsubscribe(&_tmpSensor);
		break;
	      
	      default:
		break;
	  }
        }
    }
}

SystemTask::Messages convertParsedInputToMsg(const char *inputToCmp)
{
    if (!strcmp(inputToCmp, "tempOn"))
    {
        return SystemTask::Messages::subscribeTempNotifications;
    }
    else if (!strcmp(inputToCmp, "tempOff"))
    {
        return SystemTask::Messages::unsubscribeTempNotifications;
    }
    return SystemTask::Messages::invalidInput;
}

void SystemTask::pushMessage(SystemTask::Messages dataToQueue, bool fromISR)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; // TODO - need?

    if (fromISR)
    {
        xQueueSendFromISR(systemTaskQueue, &dataToQueue, &xHigherPriorityTaskWoken);
    }
    else
    {
        xQueueSend(systemTaskQueue, &dataToQueue, 0);
    }

    //portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}



