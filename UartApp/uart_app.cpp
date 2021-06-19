#include "uart_app.hpp"

char parsedInput[50];
SystemTask::Messages dataToQueue = SystemTask::Messages::invalidInput;

void convertParsedInputToMsg(void)
{
    if (!strcmp(parsedInput, "tempEnable"))
    {
        dataToQueue = SystemTask::Messages::enableTempNotification;
    }

}

void parseUserInput(Fifo &pFifo)
{
    char const *buffer = reinterpret_cast<const char*>(pFifo.getFifo());	     // 
    uint8_t const startIdx = pFifo.getStartIdx();
    char const *startBuffer = &buffer[startIdx];
    char *endPtr = strchr(startBuffer, '\r');
    uint8_t endBytes, startBytes;
    
    if (endPtr != nullptr)	      
    {
        endBytes = endPtr - startBuffer;
        pFifo.getChunksOfData(startIdx, endBytes, reinterpret_cast<uint8_t*>(parsedInput));
        // memcpy(parsedInput, startBuffer, endBytes);
    }
    else								    // TODO - verify!! -- if there's a wrap around
    {   
        char *init = strchr(buffer, '\r');
        char *bufferEnd = strchr(buffer + startIdx, '\0');
        endBytes = bufferEnd - startBuffer;
        startBytes = init - startBuffer;
        memcpy(parsedInput, startBuffer, endBytes);
        memcpy(parsedInput + endBytes, buffer, startBytes);
    }

    convertParsedInputToMsg();
    memset(parsedInput, 0, sizeof(parsedInput));		// reset the buffer 
}

void uartCallback(Fifo &pFifo, QueueHandle_t &systemQueue)
{
    int a = 0;
    a++;
    parseUserInput(pFifo);
    
    if (dataToQueue != SystemTask::Messages::invalidInput)
    {   
        xQueueSendFromISR(systemQueue, &dataToQueue, 0); // TODO - no instance! -> SystemTask::pushMessage(SystemTask::Messages dataToQueue)
    }
    // parse the input
    // unblock the task of system task - which signals the corresponding task
}
