#include "NrfLogger.hpp"

// define static variables
TaskHandle_t NrfLogger::loggerThread;
TaskHandle_t NrfLogger::logStreamBuffer;
uint8_t NrfLogger::buffer[50];

NrfLogger::NrfLogger()
{
    auto res = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(res);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    
    size_t heapSize1 = xPortGetFreeHeapSize();
    // creating a stream buffer (instead of a queue) because its ability to take variable-sized data
    logStreamBuffer = xStreamBufferCreate(50, 5);

    // creating a task
    if (pdPASS != xTaskCreate(NrfLogger::Process, "PROCESS_LOG", 100, this, 5, &NrfLogger::loggerThread))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }

    size_t heapSize2 = xPortGetFreeHeapSize();
    size_t heapTaken = heapSize1 - heapSize2;
}

void NrfLogger::Process(void *arg)
{
    //NrfLogger *obj = static_cast<NrfLogger*>(arg);
    size_t bytesRead = xStreamBufferReceive(logStreamBuffer, (void*) buffer, 40, portMAX_DELAY);

    //NRF_LOG_INFO("%s\r\n", buffer); // TODO - fix the multiple printing part!
    //NRF_LOG_FLUSH();
}

//template<typename T>
//void NrfLogger::writeToLogger(T *data, ...)
//{
//    char tmp[40];
//    va_list args;
//    va_start(args, data);
//    vsprintf (tmp, data, args);
//    xStreamBufferSend(nrfLogTaskMsgBuffer, (void*)&tmp, 40, pdFALSE);
//} 

const uint8_t *NrfLogger::getBuffer() const
{
    return buffer;
}
