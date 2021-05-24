#include "mcp9808.hpp"

 const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

 /**
 * @brief Function for handling data from temperature sensor.
 *
 * @param[in] temp          Temperature in Celsius degrees read from sensor.
 */
__STATIC_INLINE void data_handler(uint8_t *temp)
{
    NRF_LOG_INFO("Temperature: %d Celsius degrees.", *temp);
    NRF_LOG_FLUSH();
}

 /**
 * @brief TWI events handler. Called within the TWI interrupt handler
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)	    // todo 
{
    NRF_LOG_INFO("twi_handler()\n");

    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
            {
                //data_handler(p_context->getBuffer()); // todo 
            }
            m_xfer_done = true; // todo
            break;
        default:
            break;
    }
}
//nrf_drv_twi_config_t *configIn
 MCP9808::MCP9808() 
{
    const nrf_drv_twi_config_t config = {
       .scl                = TWI_SCLK,
       .sda                = TWI_SDA,
       .frequency          = static_cast<nrf_drv_twi_frequency_t> (TWI_FREQ),	  // can't implictly cast members of different types without cast
       .interrupt_priority = TWI_IRQ_PRIORITY,
       .clear_bus_init     = false
    };
    
    ret_code_t err_code = nrf_drv_twi_init(&m_twi, &config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);
}


void MCP9808::xferData(uint8_t *p_buffer, uint8_t size)
{
    // write the data to ambient register
    write(ambientTemp, p_buffer, size);

}

void MCP9808::ReadTempInC()			  // todo - get value in float!
{
    uint8_t upperByte = buffer[0] & 0x1f;	  // mask out Alert pins
    uint8_t signedData = buffer[0] & 0x10;	  // check the signedness of the value

    if (signedData)
    {
        upperByte &= 0x0f;			  // clear SIGN bit
        temp = 256 - (upperByte << 4 | upperByte >> 4);
    }
    else
    {
        temp = upperByte << 4 | buffer[1] >> 4;
    }
}

void MCP9808::write(uint8_t reg, uint8_t *buffer, uint8_t size)
{
    //NrfLogger::writeToLogger<char>("Writing to register address %x\n", reg);

    NRF_LOG_INFO("Writing to register address %x\n", reg);
    
    ret_code_t err_code;

    err_code = nrf_drv_twi_tx(&m_twi, MCP9808_ADDR, &reg, size, false);
    APP_ERROR_CHECK(err_code);
    while (m_xfer_done == false);
}

uint16_t MCP9808::read()
{
     m_xfer_done = false;

    /* Read 1 byte from the specified address - skip 3 bits dedicated for fractional part of temperature. */
    ret_code_t err_code = nrf_drv_twi_rx(&m_twi, MCP9808_ADDR, buffer, 2);
    APP_ERROR_CHECK(err_code);
    
    ReadTempInC();
    NRF_LOG_INFO("Temperature: %d, %d, %d Celsius degrees", buffer[0], buffer[1], temp);
    NRF_LOG_FLUSH();
    return *buffer;	        // todo 
}


