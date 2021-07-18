#include "fifo.hpp"

//static uint8_t ringBuffer[8] = {QueueEmpty};		// todo

/* TODO -
 - what to do in enque() once queue == full?
 - multiple tasks write to the FIFO? protection...
*/

//template<typename T>
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

//template<typename T>
//void Fifo<T>::enque(T item)      // TODO - change uint8_t -> 32 bits?
//{
//    //NRF_LOG_DEBUG("Writing %u at index %u\n", byte, _writeIdx);
//    _buffer[_writeIdx++] = item;
//    _writeIdx &= mask;
//}

//template<typename T>
//T Fifo<T>::deque()
//{
//    if (isEmpty())
//    {
//        //NRF_LOG_WARNING("Nothing to read from the queue - _readIdx: %d, _writeIdx: %d\n", _readIdx, _writeIdx);
//        return fifoIsEmpty;
//    }

//    //NRF_LOG_DEBUG("Reading %u at index %u\n", _buffer[_readIdx], _readIdx);
    
//    T value = _buffer[_readIdx++];
//    _readIdx &= mask;

//    return value;
//}

//template<typename T>
//bool Fifo<T>::isEmpty() const
//{
//    //NRF_LOG_DEBUG("_writeIdx: %u, _readIdx: %u\n", _writeIdx, _readIdx);
//    return _writeIdx == _readIdx;
//}

template<typename T>
uint8_t const Fifo<T>::getReadIdx() const
{
    return _readIdx;
}

//template<typename T>
//void Fifo<T>::resetStartIdx()
//{
//    _startIdx = _writeIdx;
//}

//template<typename T>
//uint8_t const *Fifo<T>::getFifo()
//{
//    return _buffer;
//}

//template<typename T>
//uint8_t const Fifo<T>::getStartIdx() const
//{
//    return _startIdx;
//}

//template<typename T>
//uint8_t Fifo<T>::getChunksOfData(uint8_t startIdx, uint8_t bytesToCopy, uint8_t *buffer)	  // 2 --> 2 + 4
//{
//    uint8_t idx = 0;
//    uint8_t bytesOffset = startIdx + bytesToCopy;

//    if (startIdx > (fifoSize - 1))
//    {
//        return indexOutOfBounds;
//    }
//    else if (isEmpty())
//    {
//        return fifoIsEmpty;
//    }
//    else if (bytesToCopy >= fifoSize)
//    {
//        return invalidRequestForBytes;
//    }
    
//    while (idx < bytesToCopy)
//    {
//        T val = deque();
//        if (val != '\r')
//        {
//	  buffer[idx++] = val;
//        }
//    }
//    //memcpy(buffer, _buffer + startIdx, bytesToCopy);
//    return success;
//}