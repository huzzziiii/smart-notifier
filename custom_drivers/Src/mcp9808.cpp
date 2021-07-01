#include "mcp9808.hpp"

static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);
static bool volatile m_xfer_done = true;


void MCP9808::readTempInC()			  // TODO - get value in float!
{
    uint8_t upperByte = _buffer[0] & 0x1f;	  // mask out Alert pins
    uint8_t signedData = _buffer[0] & 0x10;	  // check the signedness of the value
    //uint16_t _tempInC; 

    if (signedData)
    {
        upperByte &= 0x0f;			  // clear SIGN bit
        _tempInC = 256 - (upperByte << 4 | upperByte >> 4);
    }
    else
    {
        _tempInC = upperByte << 4 | _buffer[1] >> 4;
    } 
}

 /**
 * @brief TWI events handler. Called within the TWI interrupt handler
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)	   
{
    MCP9808 *obj = static_cast<MCP9808*>(p_context);
    
    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
            {
	      //vTaskNotifyGiveFromISR(obj->taskHandle, pdFALSE);
	      obj->readTempInC();	 // TODO - do the parsing in the task! (parsing the data now that the transfer has been completed
            }
	  else if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_TX)
	  {
	      NRF_LOG_INFO("TX transfer done...\n");
	  }
            m_xfer_done = true; 
            break;
        default:
            break;
    }
}

uint8_t tmpBuffer[10]; // TODO - remv

 MCP9808::MCP9808() 
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

    nrf_drv_twi_enable(&m_twi);	// enable the interrupt
    
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

void MCP9808::mainThread()
{
    xferData(tmpBuffer, 1);  

    while(true)
    {   
        uint16_t xaf = read();
        
        vTaskDelay(pdMS_TO_TICKS(2000));
   //     uint32_t taskNotify = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
   //     if (taskNotify != 0)
   //     {
	  //m++;
   //     }
   //     readTempInC();
        m++;
        notify(this);
        vTaskDelay(pdMS_TO_TICKS(500));
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
    while (m_xfer_done == false);
}

uint16_t MCP9808::getCurrentTempInC() const
{
    return _tempInC;
}

uint16_t MCP9808::read()
{
    m_xfer_done = false;
    ret_code_t err_code = nrf_drv_twi_rx(&m_twi, MCP9808_ADDR, _buffer, 2);
    
    //APP_ERROR_CHECK(err_code);
    //NRF_LOG_WARNING("Function: %s, error code: %s.",
    //                 __func__,
    //                 NRF_LOG_ERROR_STRING_GET(err_code));
    //NRF_LOG_FLUSH();
    
    //while(!m_xfer_done);

    //notify(this);	        // update the subscriber of the current value 
    return _tempInC;	         
}


