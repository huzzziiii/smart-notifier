#include "port_cmsis_systick.c"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

//#include "SEGGER_SYSVIEW.h"


#include "nordic_common.h"
#include "nrf.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "app_timer.h"
#include "ble_nus.h"
#include "app_uart.h"
#include "app_util_platform.h"
#include "bsp_btn_ble.h"
#include "nrf_pwr_mgmt.h"

#include "uart.hpp"
#include "uart_app.hpp"
#include "controller.h"
#include "mcp9808.hpp"
#include "SystemTask.hpp"
#include "NotificationManager.hpp"
//#include "NrfLogger.hpp"


static void clock_init(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
}

/* Instantiations */
static constexpr uint8_t queueSize = 10;
static constexpr uint8_t itemSize  = 1;

QueueHandle_t systemQueue = xQueueCreate(queueSize, itemSize); // creating a queue to store bytes

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

// Temp sensor
MCP9808 tmpSensor;

// BLE service
BleCustomService bleCustomService;
BleController bleController(bleCustomService);

// Notification Manager
NotificationManager notificationManager(bleCustomService, uart, &tmpSensor);

// system task
SystemTask systemTask(uart, tmpSensor, notificationManager, systemQueue);

//NrfLogger logger; // TODO - uncomment!


extern "C" {
    void vApplicationMallocFailedHook(void)
    {
        NRF_LOG_INFO("Heap allocation failed...\n");
        NRF_LOG_FLUSH();
    }
}

 extern "C" {
    void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
    {
        NRF_LOG_INFO("Stack overflow ... task name: %s\n", pcTaskName);
        NRF_LOG_FLUSH();
    }
}


void initLeds()
{
    bsp_board_init(BSP_INIT_LEDS);   // configure all LEDs (1-4) to output

    //bsp_board_led_on(3);
    //APP_ERROR_CHECK(err_code);

    //if (timerReturn != NULL)
    //{
    //    timerStarted = xTimerStart(timerReturn, 0);
    //}
    //xTaskNotifyGive(systemTask.taskHandle);
}

/**@brief Function for putting the chip into sleep mode.
 *
 * @note This function will not return.
 */
static void sleep_mode_enter(void)
{
    uint32_t err_code = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(err_code);

    // Prepare wakeup buttons.
    err_code = bsp_btn_ble_sleep_mode_prepare();
    APP_ERROR_CHECK(err_code);

    // Go to system-off mode (this function will not return; wakeup will cause a reset).
    err_code = sd_power_system_off();
    APP_ERROR_CHECK(err_code);
}



/**@brief Function for application main entry.
 */
int main()
{
    const char *openApp = "SmartWatch.";
    uart.PrintUart("Welcome to the %s", openApp);	    // TODO - fix the missed TX'd bytes - make sure the transmission is done before sending a next byte

    // Initialize nRF logger module 
    auto res = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(res);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    
    // init 
    bleController.Init(CustSrvDataHdlr);

    initLeds();
    vTaskStartScheduler();	
}
