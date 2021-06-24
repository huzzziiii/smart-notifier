#include "system_task.hpp"
#include "Observer.hpp"
#include "Subject.hpp" // TODO ---

//static uint8_t ringBuffer[8] = {QueueEmpty};		// todo

/* todo -
 - what to do in enque() once queue == full?
    - override the data or wait till some vacancy?
*/
SystemTask::SystemTask(Uart &pUart, MCP9808 &tmpSensor, NotificationManager &notificationManager, QueueHandle_t &systemQueue) : 
		   uart(pUart), _tmpSensor(tmpSensor), _notificationManager(notificationManager), systemTaskQueue(systemQueue)
{   
    // create a task
    if (xTaskCreate(SystemTask::process, "PROCESS", 256, this, 0, &taskHandle) != pdPASS)	  // TODO - think about stack size!
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }   
}

void SystemTask::process(void *instance)
{
    auto pInstance = static_cast<SystemTask*>(instance);
    pInstance->mainThread();
}

uint8_t temp[40]; // TODO - remove
uint16_t xaf = 0;
int count = 0;
/**
@brief: main state machine that handles requests from the user 
@description: current supported requests: enableNotifications(<typeOfNotif>), disableNotifications(<typeOfNotif>), 
	    where <typeOfNotif> corresponds to any supported sensor
*/
void SystemTask::mainThread()
{   
    Messages curMsg;
    _tmpSensor.xferData(temp, 1);
    // TODO - init peripherals?
    
    while(true)
    {
        count++;
        xaf = _tmpSensor.read();
        //vTaskDelay(pdMS_TO_TICKS(1000));

        if (xQueueReceive(systemTaskQueue, &msg, 0) == pdPASS)      // wait for the user input over UART
        {
	  curMsg = static_cast<Messages>(msg);	  // TODO - might as well make msg type --> Message
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

void SystemTask::pushMessage(SystemTask::Messages dataToQueue)
{
    BaseType_t xHigherPriorityTaskWoken; // TODO - need?
    xQueueSendFromISR(systemTaskQueue, &dataToQueue, 0);

}



