#include "system_task.hpp"

//static uint8_t ringBuffer[8] = {QueueEmpty};		// todo

/* todo -
 - what to do in enque() once queue == full?
*/
static constexpr uint8_t queueSize = 10;
static constexpr uint8_t itemSize  = 1;

SystemTask::SystemTask()
{   
    // create a queue
    systemTaskQueue = xQueueCreate(queueSize, itemSize);
    if (systemTaskQueue == NULL)
    {
        APP_ERROR_HANDLER(NRF_ERROR_RESOURCES);
    }

    // create a task
    if (xTaskCreate(SystemTask::process, "MAIN", 256, this, 0, &taskHandle) != pdPASS)
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }   
}

void SystemTask::process(void* instance)
{
    auto pInstance = static_cast<SystemTask*>(instance);
    pInstance->mainThread();
}

void SystemTask::mainThread()
{
    // init peripherals
    
    while(true)
    {
        if (xQueueReceive(systemTaskQueue, &msg, 0) == pdPASS)
        {
	  
        }

    }
}



