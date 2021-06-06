#include "system_task.hpp"

//static uint8_t ringBuffer[8] = {QueueEmpty};		// todo

/* todo -
 - what to do in enque() once queue == full?
*/
//static constexpr uint8_t queueSize = 10;
//static constexpr uint8_t itemSize  = 1;

SystemTask::SystemTask(Uart &pUart, QueueHandle_t &systemQueue) : uart(pUart), systemTaskQueue(systemQueue)
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

void SystemTask::mainThread()
{   
    Messages curMsg;

    // init peripherals?
    
    while(true)
    {
        if (xQueueReceive(systemTaskQueue, &msg, 0) == pdPASS)
        {
	  curMsg = static_cast<Messages>(msg);	  // TODO - might as well make msg type --> Message

	  switch(curMsg)
	  {
	      case Messages::readTemperature:
		

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



