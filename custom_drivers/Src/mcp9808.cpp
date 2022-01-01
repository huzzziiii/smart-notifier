#include "mcp9808.hpp"

static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);
static bool volatile m_xfer_done = true;

// TODO
/*
- change timer period based on the temperature value
- write a function to get a floating point temp value
- think about when notifications are unsubscribed, whether it'd be worth still reading the temp value
*/

void MCP9808::readTempInC()			  // TODO - get value in float!
{
    uint8_t upperByte = _buffer[0] & 0x1f;	  // mask out Alert pins
    uint8_t lowerByte = _buffer[1];

    uint8_t signedData = _buffer[0] & 0x10;	  // check the signedness of the value
    //uint16_t _tempInC; 

    if (signedData)
    {
        upperByte &= 0x0f;			  // clear SIGN bit
        _tempInC = 256 - (upperByte << 4 | lowerByte >> 4);
    }
    else
    {
        _tempInC = upperByte << 4 | lowerByte >> 4;
    } 

  //NRF_LOG_INFO("_tempInC: %u\n", _tempInC);
    //NRF_LOG_FLUSH();
    int m = 0; // TODO remove
}

 /**
 * @brief TWI events handler. Called within the TWI interrupt handler
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)	   
{
    MCP9808 *obj = static_cast<MCP9808*>(p_context);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
            {
	      // unlocks the receiver thread by giving a semaphore (incrementing ulNotifiedValue by 1)
	      vTaskNotifyGiveFromISR(obj->taskHandle, &xHigherPriorityTaskWoken); 
	      
	      //obj->readTempInC();	 // TODO - do the parsing in the task! (parsing the data now that the transfer has been completed
	       //m_xfer_done = true; // TODO -- comment out! 
            }
	  else if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_TX)
	  {
	      //NRF_LOG_INFO("TX transfer done...\n");
	       m_xfer_done = true; 
	  }
            //m_xfer_done = true; 
            break;
        default:
            break;
    }
}

uint8_t tmpBuffer[10]; // TODO - remv

 MCP9808::MCP9808() : _tempInC(-1)
{
    i2cConfig = {
       .scl                = TWI_SCLK,
       .sda                = TWI_SDA,
       .frequency          = static_cast<nrf_drv_twi_frequency_t> (TWI_FREQ),	  
       .interrupt_priority = TWI_IRQ_PRIORITY,
       .clear_bus_init     = false
    };
    
    ret_code_t err_code = nrf_drv_twi_init(&m_twi, &i2cConfig, twi_handler, this);
    APP_ERROR_CHECK(err_code);

    // enable the interrupt
    nrf_drv_twi_enable(&m_twi);	
    
    size_t heapSize1 = xPortGetFreeHeapSize();
    
    //taskHandle = xTaskGetCurrentTaskHandle();

    // create a task
    if (xTaskCreate(MCP9808::process, "process", 100, this, 0, &taskHandle) != pdPASS)	  // TODO - think about stack size!
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    } 

    size_t heapSize2 = xPortGetFreeHeapSize(); 
    size_t heapTaken = heapSize1 - heapSize2;
}

void MCP9808::process(void *instance)
{
    auto pInstance = static_cast<MCP9808*>(instance);
    pInstance->mainThread();
}

static volatile uint8_t m = 0;
static bool blockTd = false;

void MCP9808::mainThread()
{
    xferData(tmpBuffer, 1);  
    vTaskDelay(pdMS_TO_TICKS(2000));
    uint32_t notifiedValue;

    while(true)
    {   
        // block the thread if notification is unsubscribed
        if (blockTd)
        {
	  xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        }

        uint32_t xaf = read();
        
        //NRF_LOG_INFO("READ ret: %d\n", xaf);
        //NRF_LOG_FLUSH();

        //vTaskDelay(pdMS_TO_TICKS(2000));
        uint32_t taskNotify = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);  // blocks the current thread by decrementing ulNotifiedValue to 0 (taking a semaphore)
        if (taskNotify != 0)
        {
	  //NRF_LOG_WARNING("Transmission ended as expected...\n");
	  //NRF_LOG_FLUSH();
	  m++;
        }
        else
        {
	  //NRF_LOG_WARNING("The call to ulTaskNotifyTake timedout!!\n");
        }

        readTempInC();
        //m++;
        notify(this);
  
        vTaskDelay(pdMS_TO_TICKS(DELAY_PER_READ));
        
    }
}

void MCP9808::xferData(uint8_t *p_buffer, uint8_t size)
{
    write(ambientTemp, p_buffer, size);	    // write the data to ambient register
}

void MCP9808::write(uint8_t reg, uint8_t *buffer, uint8_t size)
{
    //NrfLogger::writeToLogger<char>("Writing to register address %x\n", reg);

    //NRF_LOG_INFO("Writing to register address %x\n", reg);
    
    ret_code_t err_code;

    err_code = nrf_drv_twi_tx(&m_twi, MCP9808_ADDR, &reg, size, false);
    APP_ERROR_CHECK(err_code);
    while (!m_xfer_done);
}

uint32_t MCP9808::read()
{
    m_xfer_done = false;
    ret_code_t err_code = nrf_drv_twi_rx(&m_twi, MCP9808_ADDR, _buffer, 2);
    
    APP_ERROR_CHECK(err_code);
    //NRF_LOG_WARNING("Function: %s, error code: %s.", __func__,NRF_LOG_ERROR_STRING_GET(err_code));
    //NRF_LOG_FLUSH();
    
    //while(!m_xfer_done);
    return err_code;	 // TODO - change to pascalCase         
}


uint16_t MCP9808::getCurrentTempInC() const
{
    return _tempInC;
}

void MCP9808::onSubscriberChange(bool resumeThread)
{
    if (resumeThread)
    {
        if (_tempInC != -1)
        {
	  blockTd = false;
	  xTaskNotify(taskHandle, 0, eNoAction);
	  // vTaskResume(taskHandle);
        }
    }
    else
    {
        blockTd = true;
       // vTaskSuspend(taskHandle);
    }
}