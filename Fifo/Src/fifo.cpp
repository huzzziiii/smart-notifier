#include "fifo.hpp"

//static uint8_t ringBuffer[8] = {QueueEmpty};		// todo

/* TODO -
 - what to do in enque() once queue == full?
*/

Fifo::Fifo() : 
		         bufferSize(fifoSize), 
		         _readIdx(0), 
		         _writeIdx(0),
		         _startIdx(0),
		         mask(fifoSize - 1),
		         _buffer{0}
{
    //if (bufferSize % 2 != 0)
    //{
    //    NRF_LOG_WARNING("Fifo size needs to be power of 2 for now...\n");
    //    return;
    //}
   // NRF_LOG_INFO("Fifo of size %d is created\n", bufferSize);
   // NRF_LOG_FLUSH();
}

void Fifo::enque(uint8_t byte)      // TODO - change uint8_t -> 32 bits?
{
    //NRF_LOG_DEBUG("Writing %u at index %u\n", byte, _writeIdx);
    _buffer[_writeIdx++] = byte;
    _writeIdx &= mask;
}

uint8_t Fifo::deque()
{
    if (isEmpty())
    {
        //NRF_LOG_WARNING("Nothing to read from the queue - _readIdx: %d, _writeIdx: %d\n", _readIdx, _writeIdx);
        return fifoIsEmpty;
    }

    //NRF_LOG_DEBUG("Reading %u at index %u\n", _buffer[_readIdx], _readIdx);
    _buffer[_readIdx] = 0;
    uint8_t value = _buffer[_readIdx++];
    _readIdx &= mask;
    return value;
}

bool Fifo::isEmpty() const
{
    //NRF_LOG_DEBUG("_writeIdx: %u, _readIdx: %u\n", _writeIdx, _readIdx);
    return _writeIdx == _readIdx;
}

uint8_t const Fifo::getReadIdx() const
{
    return _readIdx;
}

void Fifo::resetStartIdx()
{
    _startIdx = _writeIdx;
}

uint8_t const *Fifo::getFifo()
{
    return _buffer;
}

uint8_t const Fifo::getStartIdx() const
{
    return _startIdx;
}

uint8_t Fifo::getChunksOfData(uint8_t startIdx, uint8_t bytesToCopy, uint8_t *buffer)	  // 2 --> 2 + 4
{
    uint8_t idx = 0;
    uint8_t bytesOffset = startIdx + bytesToCopy;

    if (startIdx > (fifoSize - 1))
    {
        return indexOutOfBounds;
    }
    else if (isEmpty())
    {
        return fifoIsEmpty;
    }
    
    while (startIdx < bytesOffset)
    {
        buffer[idx++] = deque();
    }
    //memcpy(buffer, _buffer + startIdx, bytesToCopy);
    return success;
}