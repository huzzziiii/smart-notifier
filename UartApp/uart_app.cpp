#include "uart_app.hpp"

char parsedInput[50];
SystemTask::Messages dataToQueue = SystemTask::Messages::invalidInput;

template<typename T>
void parseUserInput(Fifo<T> &pFifo)
{
    const char *buffer = reinterpret_cast<const char*>(pFifo.getFifo());	     // 
    const uint8_t startIdx = pFifo.getStartIdx();
    const char *startPtr = &buffer[startIdx];
    char *endPtr = strchr(startPtr, '\r');
    uint8_t endBytes, startBytes;
    
    if (endPtr != nullptr)	      
    {
        endBytes = endPtr - startPtr;
        uint8_t parseBytesRet = pFifo.parseBytes(reinterpret_cast<uint8_t*>(parsedInput), endBytes);
     
        // memcpy(parsedInput, startBuffer, endBytes);
    }
    else								    // TODO - verify!! -- if there's a wrap around
    {   
        char *endOfFifo = reinterpret_cast<char*>(pFifo.getNullTermAddr());
        endPtr = strchr(buffer, '\r');	    // TODO - error handling?
        startBytes = endOfFifo - startPtr;
        endBytes = endPtr - buffer;

        pFifo.parseBytes(reinterpret_cast<uint8_t*>(parsedInput), startBytes, '\0');
        pFifo.parseBytes(reinterpret_cast<uint8_t*>(parsedInput + startBytes), endBytes);

        
        //memcpy(parsedInput, startPtr, endBytes);	// call getChunksOfData()
        //memcpy(parsedInput + endBytes, buffer, startBytes);
    }

    dataToQueue = convertParsedInputToMsg(parsedInput);
    memset(parsedInput, 0, sizeof(parsedInput));		// reset the buffer 
}


void uartCallback(Fifo<uint8_t> &pFifo, QueueHandle_t &systemQueue)
{
    int a = 0;  // TODO - remove
    a++;
    parseUserInput(pFifo);
    
    // TODO - use push message...
    if (dataToQueue != SystemTask::Messages::invalidInput)
    {   
        xQueueSendFromISR(systemQueue, &dataToQueue, 0); // TODO - no instance! -> SystemTask::pushMessage(SystemTask::Messages dataToQueue)
    }
    // parse the input
    // unblock the task of system task - which signals the corresponding task
}

// TODO ....
//void bleUartHandler(BleUartEvent_t *pEventType)
//{
//    for (uint32_t i = 0; i < pEventType->params.rxData.length; i++)
//    {
//        int m = 0;
//        m++;
//        // send to UART buffer


//    }
//}
