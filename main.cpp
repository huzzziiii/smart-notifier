/**
 * Copyright (c) 2014 - 2020, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
// Board/nrf6310/ble/ble_app_hrs_rtx/main.c
/**
 *
 * @brief Heart Rate Service Sample Application with RTX main file.
 *
 * This file contains the source code for a sample application using RTX and the
 * Heart Rate service (and also Battery and Device Information services).
 * This application uses the @ref srvlib_conn_params module.
 */

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_bas.h"
#include "ble_hrs.h"
#include "ble_dis.h"
#include "ble_conn_params.h"
#include "sensorsim.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_freertos.h"
#include "app_timer.h"
#include "peer_manager.h"
#include "peer_manager_handler.h"
#include "bsp_btn_ble.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "fds.h"
#include "ble_conn_state.h"
#include "nrf_drv_clock.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "stream_buffer.h"

#include "mcp9808.hpp"
#include "NrfLogger.hpp"
#include "uart.hpp"
#include "uart_app.hpp"
#include "system_task.hpp"
#include <stdarg.h>
#include <cstring>
#include <cstdio>

//#include "queue.h"

//extern "C" size_t xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer,
//                             void *pvRxData,
//                             size_t xBufferLengthBytes,
//                             TickType_t xTicksToWait );

#if NRF_LOG_ENABLED
static TaskHandle_t m_logger_thread;                                /**< Definition of Logger thread. */
#endif

static TaskHandle_t m_huzzi_thread;
StreamBufferHandle_t streamBuffer;

//QueueHandle_t q;

/**@brief A function which is hooked to idle task.
 * @note Idle hook must be enabled in FreeRTOS configuration (configUSE_IDLE_HOOK).
 */
//void vApplicationIdleHook( void )
//{
//#if NRF_LOG_ENABLED
//     vTaskResume(m_logger_thread);
//#endif
//}


static void clock_init(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
}

#if NRF_LOG_ENABLED
/**@brief Thread for handling the logger.
 *
 * @details This thread is responsible for processing log entries if logs are deferred.
 *          Thread flushes all log entries and suspends. It is resumed by idle task hook.
 *
 * @param[in]   arg   Pointer used for passing some arbitrary information (context) from the
 *                    osThreadCreate() call to the thread.
 */
 volatile char rxBuff[45] = {0};
static void logger_thread(void * arg)
{ 
    NRF_LOG_INFO("start logger_thread()\n");
    NRF_LOG_FLUSH();  

    UNUSED_PARAMETER(arg);

    while (1)
    {
        size_t bytesRead = xStreamBufferReceive(streamBuffer, (void*) rxBuff, 40, portMAX_DELAY);
        NRF_LOG_INFO("Read bytes: %d -- ", bytesRead);

        //NRF_LOG_INFO("Received bytes...\n");
        NRF_LOG_INFO("RCVD: %s\n", rxBuff);
        NRF_LOG_FLUSH();  

       // vTaskDelay(1000);

       // vTaskSuspend(NULL); // Suspend myself
    }
}
#endif //NRF_LOG_ENABLED

/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

char loggerData[10] = "HAAHA";  // todo remove

void sendLog(const char *x, ...)
{
    char tmp[40] = {0};
    va_list args;
    va_start(args, x);
    vsprintf(tmp, x, args);

    //strncpy(tmp, log, strlen(log));
    size_t bytesSent = xStreamBufferSend(streamBuffer, (void*)tmp, sizeof(tmp), portMAX_DELAY);
    
}

void huzz(void *arg) 
{
    NRF_LOG_INFO("huzzi_thread()\n");
    NRF_LOG_FLUSH();
    
    //vTaskResume(m_logger_thread);

    char temp[] = "PASSING\n";
    strncpy(loggerData, temp, strlen(temp));
    temp[strlen(temp)] = '\0';
   
    while(1) 
    {
        vTaskDelay(5000);
        sendLog("YESS -- val: %d", 54);
        //size_t bytesSent = xStreamBufferSend(streamBuffer, (void*)loggerData, sizeof(loggerData), portMAX_DELAY);
        //NRF_LOG_INFO("Bytes sent: %d\n", bytesSent);
        //NRF_LOG_FLUSH();
        //vTaskDelay(2000);
    }
}
/* Instantiations */
static constexpr uint8_t queueSize = 10;
static constexpr uint8_t itemSize  = 1;
QueueHandle_t systemQueue = xQueueCreate(queueSize, itemSize);

// UART - assigning a uart instance to be static so it could be used inside UART IRQ handler
UartCommParams_t commParams = 
{
        .rxPinNo = RX_PIN_NUMBER,
        .txPinNo = TX_PIN_NUMBER,
        .rtsPinNo = RTS_PIN_NUMBER,
        .ctsPinNo = CTS_PIN_NUMBER,
        .hwFlowCntl = UartHwFcDisabled,
        .useParity = false,
        .baudRate = NRF_UART_BAUDRATE_115200
};

Uart uart(&commParams, NRF_UART0, APP_IRQ_PRIORITY_LOWEST, uartCallback, systemQueue);



//if (systemTaskQueue == NULL)
//{
//    APP_ERROR_HANDLER(NRF_ERROR_RESOURCES);
//}

SystemTask systemTask(uart, systemQueue);

/**@brief Function for application main entry.
 */
int main(void)
{   
    NrfLogger logger;

    //NrfLogger::writeToLogger<char>("Writing to register address %x", 10);

    // Initialize modules  
    auto res = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(res);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    clock_init();
     
    // uint8_t tmp[40];
    // MCP9808 tmpSensor;
    //tmpSensor.xferData(tmp, 1);
    
    //while(true) 
    //{
    //    tmpSensor.read();
    //    nrf_delay_ms(3000);
    //}

    vTaskStartScheduler();		// Start FreeRTOS scheduler

    //while(1);

    /*
    char buff[6];
    sprintf(buff, "ZUZ%s\n", "i");

    //char ugh[] = "NRF_LOG_INFO";
    NRF_LOG_INFO("%s\r\n", buff);
    //NRF_LOG_INFO("hua");
    NRF_LOG_FLUSH();
    
   
    uint8_t tmp[1] = {0};
    MCP9808 tmpSensor;
    tmpSensor.xferData(tmp, 1);
    
    while(true) 
    {
        tmpSensor.read();
        nrf_delay_ms(3000);
    }
    
    // Start FreeRTOS scheduler.
    vTaskStartScheduler();*/

/*
    streamBuffer = xStreamBufferCreate(45, 5);

    if (streamBuffer == NULL)
    {
        NRF_LOG_INFO("Couldn't alloc stream buffer\n");
    }
    else 
    {
        NRF_LOG_INFO("STREAM buff == alloc'd\n");
    }

    

    #if NRF_LOG_ENABLED
    // Start execution.
    if (pdPASS != xTaskCreate(logger_thread, "LOGGER", 256, NULL, 5, &m_logger_thread))
    {
        //APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
    #endif
    
    if (pdPASS != xTaskCreate(huzz, "LOGGER", 256, NULL, 1, &m_huzzi_thread))
    {
        //APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
        

    // MCP9808
    initMcp9808();

    // Start FreeRTOS scheduler.
    //vTaskStartScheduler();*/
}




