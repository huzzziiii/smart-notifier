#ifndef __MCP9808_H
#define __MCP9808_H

#include "sensors.hpp"
#include "nrf_drv_twi.h"
#include "NrfLogger.hpp"

#define MCP9808_ADDR	      0x18
#define TWI_INSTANCE_ID	      0
#define BYTES_PER_TRANSACTION	      2

typedef enum 
{
    TWI_SCLK		    =          27,
    TWI_SDA		    =	     26,
    TWI_FREQ		    =          NRF_DRV_TWI_FREQ_100K,
    TWI_IRQ_PRIORITY	    =	     APP_IRQ_PRIORITY_HIGH,
    TWI_CLR_BUS_INIT	    =          false
} I2cPins;

enum MCP9808_REG  
{
    criticalTemp = 0x4, 
    ambientTemp = 0x5,
    manufucterurId = 0x6
};

static bool m_xfer_done;  // todo

//class MCP9808 : public Sensors 
class MCP9808 
{
    //static const nrf_drv_twi_t m_twi;
    uint8_t buffer[10] = {0};		// todo - size!
    uint16_t temp;

    //union xferState
    //{
       
        //bool m_xfer_
    //}

    public:
    //MCP9808(uint32_t sclPin, uint32_t sdaPin, nrf_drv_twi_frequency_t i2cFreq, uint8_t irqPriority);
    MCP9808(nrf_drv_twi_config_t *config);
    MCP9808();
    void begin();
    void write(uint8_t address, uint8_t *buffer, uint8_t size);
    //float ReadTempInC();
    void ReadTempInC();
    float readTempInF();	      // TODO
    uint16_t read();
    
    void xferData(uint8_t *p_buffer, uint8_t size);
    //void initXfer(uint8_t *buffer, uint8_t size);

    const uint8_t *getBuffer() const 
    {
        return buffer;
    }
    //void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context); // todo
    

};
#endif
