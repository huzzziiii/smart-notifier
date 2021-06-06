#include "uart_app.hpp"

char parsedInput[50];
SystemTask::Messages dataToQueue = SystemTask::Messages::invalidInput;

void validateAndExecute(void)
{
    if (!strcmp(parsedInput, "temp"))
    {
        dataToQueue = SystemTask::Messages::readTemperature;

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
        memcpy(parsedInput, startBuffer, endBytes);
    }
    else								    // if there's a wrap around
    {   
        char *init = strchr(buffer, '\r');
        char *bufferEnd = strchr(buffer + startIdx, '\0');
        endBytes = bufferEnd - startBuffer;
        startBytes = init - startBuffer;
        memcpy(parsedInput, startBuffer, endBytes);
        memcpy(parsedInput + endBytes, buffer, startBytes);
    }
    
    //int b = 5; // TODO - remove

    validateAndExecute();

    /*
    char *startPtr = strchr(arr + start, '\r');
    printf ("StartVal: %c, Start_ptr: %p\n", arr[start], (startPtr - arr));
    uint8_t bytes;
    if (startPtr != nullptr)
    {
    bytes = startPtr - &arr[start];

    memcpy(ans, &arr[start], bytes);
    printf ("bytes: %d -- arr: %s\n", bytes, ans);
    }
    else
    {
    printf ("Enter char couldn't be found...\n");

    startPtr = strchr(arr, '\r');
    char *endPtr = strchr(arr + start, '\0');
    bytes = endPtr - &arr[start];
    uint8_t startBytes = startPtr - arr;
    memcpy(ans, arr + start, bytes);
    memcpy(ans + bytes, arr, startBytes);

    printf ("Start ptr: %p, end ptr: %p, end bytes: %d, start bytes: %d, ans: %s\n",
	  (startPtr - arr), (endPtr - arr), bytes, startBytes, ans);
    }*/
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


