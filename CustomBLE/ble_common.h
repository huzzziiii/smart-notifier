#ifndef BLE_COMMON_FILE__H
#define BLE_COMMON_FILE__H

#ifdef __cplusplus
extern "C" {
#endif

#include "ble_gatt.h"
#include "ble_advertising.h"
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
#include "sdk_config.h"
#include "nrf_log.h"
#include <stdint.h>

/**@brief Custom Service structure. This contains various status information for the service. */
struct ble_cus_s
{
    uint16_t                      service_handle;                 /**< Handle of Custom Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t      custom_value_handles;           /**< Handles related to the Custom Value characteristic. */
    uint16_t                      conn_handle;                    /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
    uint8_t                       uuid_type; 
};

// Forward declaration of the ble_cus_t type.
typedef struct ble_cus_s ble_cus_t;

/**@brief Custom Service init structure. This contains all options and data needed for
 *        initialization of the service.*/
typedef struct
{
    uint8_t                       initial_custom_value;           /**< Initial custom value */
    ble_srv_cccd_security_mode_t  custom_value_char_attr_md;     /**< Initial security level for Custom characteristics attribute */
    void (*dataHandler)(ble_nus_evt_t * p_evt);
} ble_cus_init_t;

//ble_cus_t custom_service; // huzzi

//#define APP_BLE_CONN_CFG_TAG            1                                           /**< A tag identifying the SoftDevice BLE configuration. */

//#define DEVICE_NAME                     "Luz-Nordic_UART"                               /**< Name of device. Will be included in the advertising data. */
//#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */

//#define APP_BLE_OBSERVER_PRIO           3                                           /**< Application's BLE observer priority. You shouldn't need to modify this value. */

//#define APP_ADV_INTERVAL                64                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */

//#define APP_ADV_DURATION                18000                                       /**< The advertising duration (180 seconds) in units of 10 milliseconds. */

//#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(20, UNIT_1_25_MS)             /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
//#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(75, UNIT_1_25_MS)             /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
//#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
//#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
//#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)                       /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
//#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)                      /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
//#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

//#define DEAD_BEEF                       0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

//#define UART_TX_BUF_SIZE                256                                         /**< UART TX buffer size. */
//#define UART_RX_BUF_SIZE                256                                         /**< UART RX buffer size. */

///* This code belongs in ble_cus.h*/
//#define CUSTOM_SERVICE_UUID               0x1400
//#define CUSTOM_VALUE_CHAR_UUID            0x1401

//#define BLE_UUID_TYPE_VENDOR_BEGIN	  0x02 /**< Vendor UUID types start at this index (128-bit). */

//BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT);                                   /**< BLE NUS service instance. */
//                                                         /**< GATT module instance. */
//NRF_BLE_QWR_DEF(m_qwr);                                                             /**< Context for the Queued Write module.*/
//BLE_ADVERTISING_DEF(m_advertising);                                                 /**< Advertising module instance. */


// xxxx
void ble_stack_init();
void gatt_init(void);
void services_init(void);
void gap_params_init(void);
void advertising_init(void);
void conn_params_init(void);
void advertising_start(void);
void foo();

#ifdef __cplusplus
}
#endif

#endif