#ifndef __FIFO_H
#define __FIFO_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "stdint.h"
#include <stdbool.h>
#include <stdarg.h>

constexpr uint8_t fifoSize = 16;

//typedef enum 
//{
//    QueueEmpty = 0,
//    QueueFull = 1
// } FifoStatus;

 typedef enum 
 {
    indexOutOfBounds = 0,
    invalidRequestForBytes,
    fifoIsEmpty,
    success
} FifoReturnValues;


class Fifo 
{
    //uint8_t *buffer;
    uint8_t mask;
    uint8_t bufferSize; // TODO - remove?
    volatile uint8_t _readIdx;	      // volatile - cause the values would be modified inside an ISR
    volatile uint8_t _writeIdx;
    volatile uint8_t _startIdx;	     // start index for the user input
    
    //T _buffexr[fifoSize];	// TODO - volatile?!
    uint8_t _buffer[fifoSize];	// TODO - volatile?!
    
    
    public:
    Fifo();
    void enque(const uint8_t byte);
    uint8_t deque();
    bool isEmpty() const;
    uint8_t const getReadIdx() const;
    void resetStartIdx();
    uint8_t const *getFifo();
    uint8_t const getStartIdx() const;
    uint8_t getChunksOfData(uint8_t startIdx, uint8_t bytesToCopy, uint8_t *buffer);
};
#endif
