#ifndef __FIFO_H
#define __FIFO_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "stdint.h"
#include <stdbool.h>
#include <stdarg.h>

constexpr uint8_t fifoSize = 8;

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
    volatile uint8_t readIdx;	      // TODO - volatile?
    volatile uint8_t writeIdx;
    volatile uint8_t startIdx;      // start index for the user input
    uint8_t buffer[fifoSize];
    
    public:
    Fifo();
    void enque(const uint8_t byte);
    uint8_t deque();
    bool isEmpty() const;
    uint8_t const getReadIdx() const;
    void resetStartIdx();
    uint8_t const *getFifo();
    uint8_t const getStartIdx() const;
};
#endif
