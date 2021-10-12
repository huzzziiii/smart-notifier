#ifndef __FIFO_H
#define __FIFO_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "stdint.h"
#include <stdbool.h>
#include <stdarg.h>
#include <typeinfo>
// TODO
/*
    - include protection guard for the buffer in case BLE service also accesses it
*/
constexpr uint8_t fifoSize = 65; // 17; // (16) +1 to accomodate for a NULL terminator

 typedef enum 
 {
    indexOutOfBounds = -1,
    invalidRequestForBytes = -2,
    fifoIsEmpty = -3,
    success = 1
} FifoReturnValues;

template<typename T>
class Fifo 
{
    //uint8_t *buffer;
    uint8_t mask;
    uint8_t bufferSize; // TODO - remove?
    volatile uint8_t _readIdx;	      // volatile - cause the values would be modified inside an ISR
    volatile uint8_t _writeIdx;
    volatile uint8_t _startIdx;	     // start index for the user input
    bool _isFull = false;
    
    T _buffer[fifoSize] = {0};	// TODO - volatile?!
    //uint8_t _buffer[fifoSize];	// TODO - remove -- volatile?!
    
    
    public:
    Fifo() :  bufferSize(fifoSize), 
		         _readIdx(0), 
		         _writeIdx(0),
		         _startIdx(0),
		         mask(fifoSize - 2)
    {}

//    template<typename T>
//Fifo<T>::Fifo() : 
//		         bufferSize(fifoSize), 
//		         _readIdx(0), 
//		         _writeIdx(0),
//		         _startIdx(0),
//		         mask(fifoSize - 1),
//		         _buffer{0}
//{
//    //if (bufferSize % 2 != 0)
//    //{
//    //    NRF_LOG_WARNING("Fifo size needs to be power of 2 for now...\n");
//    //    return;
//    //}
//   // NRF_LOG_INFO("Fifo of size %d is created\n", bufferSize);
//   // NRF_LOG_FLUSH();
//}


    //void enque(const T byte);

    void printFifo() const
    {
        //NRF_LOG_FLUSH();
        //NRF_LOG_INFO("Template Type: %s\n", __PRETTY_FUNCTION__);
        //NRF_LOG_FLUSH();

   //     for (uint8_t idx = 0; idx < fifoSize; idx++)
   //     {
	  //NRF_LOG_INFO("%
   //     }
    }

    uint8_t const getReadIdx() const;
    void resetStartIdx()
    {
        _startIdx = _writeIdx;
    }

    bool isEmpty() const
    {
        return _writeIdx == _readIdx;
    }

    bool isFull() const
    {
        return _isFull;
    }
    
    T deque()
    {
   // TODO --     if (isEmpty())
   //     {
	  ////NRF_LOG_WARNING("Nothing to read from the queue - _readIdx: %d, _writeIdx: %d\n", _readIdx, _writeIdx);
	  //return fifoIsEmpty;
   //     }

        //NRF_LOG_DEBUG("Reading %u at index %u\n", _buffer[_readIdx], _readIdx);
        if (isEmpty())
        {
	  // NRF_LOG_WARNING("Nothing to read from the queue - _readIdx: %d, _writeIdx: %d\n", _readIdx, _writeIdx);
	  return fifoIsEmpty;
        }

        T value = _buffer[_readIdx];
        
        ++_readIdx &= mask;


   //     if (value == '\r')
   //     {	  
	  //++_readIdx &= mask;
   //     }

        //value = _buffer[_readIdx++];
        //_readIdx &= mask;

        return value;
    }

    void enque(T item)    
    {
         //NRF_LOG_INFO("[ENQUE] Writing at index %u\n", _writeIdx);
        //NRF_LOG_FLUSH();

        _buffer[_writeIdx++] = item;
        _writeIdx &= mask;

        _isFull = (_writeIdx - _readIdx == fifoSize - 2);    
    }
    
    void enqueElems(T *buffer, uint8_t size)
    {
        for (uint8_t idx = 0; idx < size; idx++)
        {
	  enque(buffer[idx]);  
        }
    }

    uint8_t const *getFifo()
    {
        return _buffer;
    }

    uint8_t const getStartIdx() const
    {
        return _startIdx;
    }
    
    uint8_t parseBytes(T *buffer, uint8_t bytesToCopy, char delimiter = '\r')
    {
        uint8_t count = 0;
        //uint8_t bytesOffset = startIdx + bytesToCopy;

   //     if (startIdx > (fifoSize - 1))
   //     {
	  //return indexOutOfBounds;
   //     }
   //     else if (isEmpty())
   //     {
	  //return fifoIsEmpty;
   //     }
   //     else if (bytesToCopy >= fifoSize)
   //     {
	  //return invalidRequestForBytes;
   //     }
    
        while (count < bytesToCopy)
        {
	  T val = deque();
	  if (val == fifoIsEmpty)
	  {
	      return fifoIsEmpty;
	  }
	  
	  if (val == delimiter)
	  {
	      _readIdx++;
	      break;
	  }
	  
	  *(buffer++) = val;
	  count++;  
        }
        //memcpy(buffer, _buffer + startIdx, bytesToCopy);
        _readIdx++;
        return success;
    }

    uint8_t *getNullTermAddr()
    {
        return &_buffer[fifoSize - 1];
    }

    T GetFiFoAt(uint8_t idx) const 
    {
        return _buffer[idx];
    }

    uint8_t GetFifoSize() const
    {
        //NRF_LOG_INFO("Fifo Write size: %d\n", _writeIdx);
        //NRF_LOG_FLUSH();

        return _writeIdx;
    }
};
#endif
