#ifndef BLE_COMMON__H
#define BLE_COMMON__H

//#include "ble_gatt.h"
//#include "ble_advertising.h"
//#include "BleUartService.hpp"
//#include "ble_nus.h"
#include "bsp.h"
#include "nrf_sdh.h"

#include "nrf_ble_gatt.h"
//#include "ble_types.h"
//#include "nrf_log.h"

//#include "nrf_ble_qwr.h"
//#include "ble_conn_params.h"
//#include "nrf_ble_qwr.h"

#define APP_BLE_OBSERVER_PRIO           3                                           /**< Application's BLE observer priority. You shouldn't need to modify this value. */

#define APP_BLE_CONN_CFG_TAG            1                                           /**< A tag identifying the SoftDevice BLE configuration. */
#define BLE_UUID_TYPE_VENDOR_BEGIN	0x02				    /**< Vendor UUID types start at this index (128-bit). */

#define DEVICE_NAME                     "HUZZi_UART"                               /**< Name of device. Will be included in the advertising data. */
#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN 

#define APP_ADV_INTERVAL                64                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */

#define APP_ADV_DURATION                18000                                       /**< The advertising duration (180 seconds) in units of 10 milliseconds. */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(20, UNIT_1_25_MS)             /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(75, UNIT_1_25_MS)             /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)                       /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)                      /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3    


static uint16_t   m_conn_handle          = BLE_CONN_HANDLE_INVALID;                 /**< Handle of the current connection. */
static uint16_t   m_ble_nus_max_data_len = BLE_GATT_ATT_MTU_DEFAULT - 3;            /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Nordic UART service module. */


/**@brief Add characteristic parameters structure.
 * @details This structure contains the parameters needed to use the @ref characteristic_add function.
 */
//typedef struct
//{
//    uint16_t                    uuid;                     /**< Characteristic UUID (16 bits UUIDs).*/
//    uint8_t                     uuid_type;                /**< Base UUID. If 0, the Bluetooth SIG UUID will be used. Otherwise, this should be a value returned by @ref sd_ble_uuid_vs_add when adding the base UUID.*/
//    uint16_t                    max_len;                  /**< Maximum length of the characteristic value.*/
//    uint16_t                    init_len;                 /**< Initial length of the characteristic value.*/
//    uint8_t *                   p_init_value;             /**< Initial encoded value of the characteristic.*/
//    bool                        is_var_len;               /**< Indicates if the characteristic value has variable length.*/
//    ble_gatt_char_props_t       char_props;               /**< Characteristic properties.*/
//    ble_gatt_char_ext_props_t   char_ext_props;           /**< Characteristic extended properties.*/
//    bool                        is_defered_read;          /**< Indicate if deferred read operations are supported.*/
//    bool                        is_defered_write;         /**< Indicate if deferred write operations are supported.*/
//    security_req_t              read_access;              /**< Security requirement for reading the characteristic value.*/
//    security_req_t              write_access;             /**< Security requirement for writing the characteristic value.*/
//    security_req_t              cccd_write_access;        /**< Security requirement for writing the characteristic's CCCD.*/
//    bool                        is_value_user;            /**< Indicate if the content of the characteristic is to be stored in the application (user) or in the stack.*/
//    ble_add_char_user_desc_t    *p_user_descr;            /**< Pointer to user descriptor if needed*/
//    ble_gatts_char_pf_t         *p_presentation_format;   /**< Pointer to characteristic format if needed*/
//}  ble_add_char_params_t;


// Characteristic parameters structurer
typedef struct
{
    uint16_t		  uuid;
    uint8_t		  uuidType;
    uint16_t		  maxLength;
    bool			  isVariableLength;
    ble_gatt_char_props_t	  charProperties;
} bleAddCharParam_t;


void    ble_stack_init();
void    gap_params_init();
void    gatt_init();
void    services_init();
void    advertising_init();
void    conn_params_init();
void    advertising_start();

#endif