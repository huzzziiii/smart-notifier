#include "fifo.hpp"

//static uint8_t ringBuffer[8] = {QueueEmpty};		// todo

/* todo -
 - what to do in enque() once queue == full?
*/

Fifo::Fifo() : 
		         bufferSize(fifoSize), 
		         readIdx(0), 
		         writeIdx(0),
		         startIdx(0),
		         mask(fifoSize - 1),
		         buffer{0}
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
    //NRF_LOG_DEBUG("Writing %u at index %u\n", byte, writeIdx);
    buffer[writeIdx++] = byte;
    writeIdx &= mask;
}

uint8_t Fifo::deque()
{
    if (isEmpty())
    {
        NRF_LOG_WARNING("Nothing to read from the queue - readIdx: %d, writeIdx: %d\n", readIdx, writeIdx);
        return QueueEmpty;
    }

    NRF_LOG_DEBUG("Reading %u at index %u\n", buffer[readIdx], readIdx);
    uint8_t value = buffer[readIdx++];
    readIdx &= mask;
    return value;
}

bool Fifo::isEmpty() const
{
    NRF_LOG_DEBUG("writeIdx: %u, readIdx: %u\n", writeIdx, readIdx);
    return writeIdx == readIdx;
}

uint8_t const Fifo::getReadIdx() const
{
    return readIdx;
}

void Fifo::resetStartIdx()
{
    startIdx = writeIdx;
}

uint8_t const *Fifo::getFifo()
{
    return buffer;
}

uint8_t const Fifo::getStartIdx() const
{
    return startIdx;
}
