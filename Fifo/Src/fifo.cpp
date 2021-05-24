#include "fifo.hpp"

//static uint8_t ringBuffer[8] = {QueueEmpty};		// todo

/* todo -
 - what to do in enque() once queue == full?
*/

Fifo::Fifo(uint8_t bufferSize) : 
		         bufferSize(bufferSize), 
		         readIdx(0), 
		         writeIdx(0),
		         mask(bufferSize - 1)
{
    if (bufferSize % 2 != 0)
    {
        NRF_LOG_WARNING("Fifo size needs to be power of 2 for now...\n");
        return;
    }
    NRF_LOG_INFO("Fifo of size %d is created\n", bufferSize);
}

void Fifo::enque(uint8_t byte)
{
    NRF_LOG_DEBUG("Writing %u at index %u\n", byte, writeIdx);
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