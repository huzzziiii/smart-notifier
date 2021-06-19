#include "mcp9808.hpp"

static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);
static bool m_xfer_done; 


void MCP9808::readTempInC()			  // TODO - get value in float!
{
    uint8_t upperByte = _buffer[0] & 0x1f;	  // mask out Alert pins
    uint8_t signedData = _buffer[0] & 0x10;	  // check the signedness of the value
    uint16_t _tempInC; 

    if (signedData)
    {
        upperByte &= 0x0f;			  // clear SIGN bit
        _tempInC = 256 - (upperByte << 4 | upperByte >> 4);
    }
    else
    {
        _tempInC = upperByte << 4 | _buffer[1] >> 4;
    }
    notify(this);
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
	      obj->readTempInC();				  // parsing the data now that the transfer has been completed
            }
            m_xfer_done = true; 
            break;
        default:
            break;
    }
}

 MCP9808::MCP9808() 
{
    const nrf_drv_twi_config_t config = {
       .scl                = TWI_SCLK,
       .sda                = TWI_SDA,
       .frequency          = static_cast<nrf_drv_twi_frequency_t> (TWI_FREQ),	  
       .interrupt_priority = TWI_IRQ_PRIORITY,
       .clear_bus_init     = false
    };
    
    ret_code_t err_code = nrf_drv_twi_init(&m_twi, &config, twi_handler, this);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);
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

uint16_t MCP9808::read()
{
     m_xfer_done = false;
    ret_code_t err_code = nrf_drv_twi_rx(&m_twi, MCP9808_ADDR, _buffer, 2);
    APP_ERROR_CHECK(err_code);
    
    while(!m_xfer_done);
    return _tempInC;	         
}


