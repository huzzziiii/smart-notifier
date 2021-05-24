#ifndef __FIFO_H
#define __FIFO_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "stdint.h"
#include <stdbool.h>
#include <stdarg.h>

#define FIFO_SIZE		  8

typedef enum 
{
    QueueEmpty = 0,
    QueueFull = 1
 } FifoStatus;

class Fifo 
{
    //uint8_t *buffer;
    uint8_t mask;
    uint8_t bufferSize;
    volatile uint8_t readIdx;	      
    volatile uint8_t writeIdx;
    uint8_t buffer[FIFO_SIZE];
    
    public:
    Fifo(uint8_t bufferSize);
    void enque(const uint8_t byte);
    uint8_t deque();
    bool isEmpty() const;
};
#endif
