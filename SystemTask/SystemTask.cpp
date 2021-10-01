#include "SystemTask.hpp"
#include "Observer.hpp"
#include "Subject.hpp" // TODO ---
//#include "bleApp.hpp"
//#include "BleUartService.hpp"
//static uint8_t ringBuffer[8] = {QueueEmpty};		// todo

/* todo -
 - what to do in enque() once queue == full?
    - override the data or wait till some vacancy?
*/
//SystemTask::SystemTask(Uart &pUart, MCP9808 &tmpSensor, NotificationManager &notificationManager, QueueHandle_t &systemQueue, BleUartService &bleService) : 
//		   uart(pUart), _tmpSensor(tmpSensor), _notificationManager(notificationManager), systemTaskQueue(systemQueue), _bleService(bleService)

SystemTask::SystemTask(Uart &pUart, MCP9808 &tmpSensor, NotificationManager &notificationManager, QueueHandle_t &systemQueue) : 
		   uart(pUart), _tmpSensor(tmpSensor), _notificationManager(notificationManager), systemTaskQueue(systemQueue)
{   
    size_t heapSize1 = xPortGetFreeHeapSize();

    // create a task
    if (xTaskCreate(SystemTask::process, "PROCESS", 100, this, 0, &taskHandle) != pdPASS)	  // TODO - think about stack size!
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    } 
      
    size_t heapSize2 = xPortGetFreeHeapSize();
    size_t heapTaken = heapSize1 - heapSize2;

    // BLE advertising and configuration
    //ble_stack_init();
    //gap_params_init();
    //gatt_init();
    //services_init();
    //advertising_init();
    //conn_params_init();

    // Start execution.
    //printf("\r\nUART started.\r\n");
    //NRF_LOG_INFO("Debug logging for UART over RTT started.");
    //advertising_start();
}

void SystemTask::process(void *instance)
{
    auto pInstance = static_cast<SystemTask*>(instance);
    pInstance->mainThread();
}

static uint8_t temp[40]; // TODO - remove
static uint16_t xaf = 0;
static int count = 0;

/**
@brief: main state machine that handles requests from the user 
@description: current supported requests: enableNotifications(<typeOfNotif>), disableNotifications(<typeOfNotif>), 
	    where <typeOfNotif> corresponds to any supported sensor
*/
void SystemTask::mainThread()
{   
    Messages curMsg;

    //// BLE advertising and configuration
    //ble_stack_init();
    //gap_params_init();
    //gatt_init();
    //services_init();
    //advertising_init();
    //conn_params_init();

    ////// Start execution.
    //////printf("\r\nUART started.\r\n");
    ////NRF_LOG_INFO("Debug logging for UART over RTT started.");
    //advertising_start();


    //_tmpSensor.xferData(temp, 1);
    //_tmpSensor.read();
    // TODO - init peripherals?
    
        count++;
        //xaf = _tmpSensor.read();
        //vTaskDelay(pdMS_TO_TICKS(1000));
    while(true)
    {
        if (xQueueReceive(systemTaskQueue, &msg, portMAX_DELAY) == pdPASS)      // wait for the user input over UART (for now!)
        {
	  curMsg = static_cast<Messages>(msg);		         // TODO - might as well make msg type --> Message
	  NRF_LOG_INFO("[SystemTask] -- message received: %d\n", curMsg);
	  	       
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
    NRF_LOG_WARNING("SystemTask::pushMessage()...\n");
    NRF_LOG_FLUSH();
    BaseType_t xHigherPriorityTaskWoken; // TODO - need?
    xQueueSendFromISR(systemTaskQueue, &dataToQueue, 0);

}



