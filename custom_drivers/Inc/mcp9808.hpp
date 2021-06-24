#ifndef __MCP9808_H
#define __MCP9808_H

#include "sensors.hpp"
#include "nrf_drv_twi.h"
#include "NrfLogger.hpp"
#include "Subject.hpp"

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

/*
@MCP9808 class - inherits from the Subject class
@brief: invokes driver functions for reading the raw temperature value.
Every time the value is read, it invokes notify() for its observers
*/
//class MCP9808 : public Sensors		  // TODO - add inheritance...
class MCP9808 : public Subject
{
    uint8_t _buffer[10] = {0};		// TODO - size!
    uint16_t _tempInC;			// stores temperature value in Celcius

    public:
    MCP9808(nrf_drv_twi_config_t *config);
    MCP9808();
    void begin();
    void write(uint8_t address, uint8_t *buffer, uint8_t size);
    void readTempInC();
    float readTempInF();	      // TODO
    uint16_t read();
    uint16_t getCurrentTempInC() const;
    
    void xferData(uint8_t *p_buffer, uint8_t size);
   
    const uint8_t *getBuffer() const 
    {
        return _buffer;
    }

};
#endif
