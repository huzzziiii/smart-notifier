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
#include <algorithm>
#include "NotificationManager.hpp"

static void clock_init(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
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

// uart instance
Uart uart(&commParams, NRF_UART0, APP_IRQ_PRIORITY_LOWEST, uartCallback, systemQueue);

// system task
SystemTask systemTask(uart, systemQueue);


static constexpr uint32_t idleTime = 3000;
int m = 0;
void IdleTimerCallback(TimerHandle_t xTimer)
{
    NRF_LOG_INFO("......Idle timeout -> Going to sleepXxX !!!! ");
    m++;
    //NRF_LOG_FLUSH();
}

/**@brief Function for application main entry.
 */
int main()
{  
   //ulTaskNotifyTake()
    NrfLogger logger;

    //NrfLogger::writeToLogger<char>("Writing to register address %x", 10);

    // Initialize modules  
    auto res = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(res);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    clock_init();
    
    //TimerHandle_t idleTimer = xTimerCreate ("idleTimer", pdMS_TO_TICKS(idleTime), pdFALSE, 0, IdleTimerCallback);
    //xTimerStart(idleTimer, 0);
    

    //vTaskStartScheduler();		// Start FreeRTOS scheduler

     uint8_t tmp[40];
     MCP9808 tmpSensor;
     NotificationManager ntf(tmpSensor);
    tmpSensor.xferData(tmp, 1);
    
    while(true) 
    {
        uint16_t data = tmpSensor.read();
        ntf.unsubscribe();
        //nrf_delay_ms(6000);
    }
}




