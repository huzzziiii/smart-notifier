#include "NrfLogger.hpp"

// define static variables
TaskHandle_t NrfLogger::loggerThread;
TaskHandle_t NrfLogger::nrfLogTaskMsgBuffer;

NrfLogger::NrfLogger()
{
    auto res = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(res);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    // create a task
    if (pdPASS != xTaskCreate(NrfLogger::Process, "PROCESS_LOG", 256, this, 5, &NrfLogger::loggerThread))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}

void NrfLogger::Process(void *arg)
{
    //NrfLogger *obj = (NrfLogger*) arg;
    NrfLogger *obj = static_cast<NrfLogger*>(arg);
    size_t bytesRead = xStreamBufferReceive(nrfLogTaskMsgBuffer, (void*) obj->getBuffer(), 40, portMAX_DELAY);

    NRF_LOG_INFO("%s\r\n", obj->getBuffer());
    NRF_LOG_FLUSH();
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
