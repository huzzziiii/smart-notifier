///**@file
// *
// * @defgroup ble_nus Nordic UART Service
// * @{
// * @ingroup  ble_sdk_srv
// * @brief    Nordic UART Service implementation.
// *
// * @details The Nordic UART Service is a simple GATT-based service with TX and RX characteristics.
// *          Data received from the peer is passed to the application, and the data received
// *          from the application of this service is sent to the peer as Handle Value
// *          Notifications. This module demonstrates how to implement a custom GATT-based
// *          service and characteristics using the SoftDevice. The service
// *          is used by the application to send and receive ASCII text strings to and from the
// *          peer.
// *
// * @note    The application must register this module as BLE event observer using the
// *          NRF_SDH_BLE_OBSERVER macro. Example:
// *          @code
// *              ble_nus_t instance;
// *              NRF_SDH_BLE_OBSERVER(anything, BLE_NUS_BLE_OBSERVER_PRIO,
// *                                   ble_nus_on_ble_evt, &instance);
// *          @endcode
// */
//#ifndef BLE_UART_SERVICE_H__
//#define BLE_UART_SERVICE_H__

//#include <stdint.h>
//#include <stdbool.h>
//#include "ble_nus.h"
//#include "bsp.h"
//#include "nrf_sdh.h"
//#include "nrf_sdh_ble.h"
//#include "nrf_ble_gatt.h"
//#include "sdk_config.h"
//#include "ble.h"
//#include "ble_srv_common.h"
//#include "nrf_log.h"

//#include "nrf_sdh_ble.h"
//#include "ble_link_ctx_manager.h"
//#include "ble_advdata.h"
//#include "ble_advertising.h"
//#include "ble_conn_params.h"
//#include "TimerApp.hpp"
//#include "nrf_ble_qwr.h"

//#include "nrf_ble_qwr.h"
//typedef uint32_t returnCode;

////#define BLE_UUID_NUS_TX_CHARACTERISTIC 0x0003               /**< The UUID of the TX Characteristic. */
////#define BLE_UUID_NUS_RX_CHARACTERISTIC 0x0002               /**< The UUID of the RX Characteristic. */

//#define APP_BLE_CONN_CFG_TAG            1  
//#define APP_BLE_OBSERVER_PRIO           3    
//#define DEVICE_NAME			"nRF_BLE_SERVICE"
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
//#define BLE_UUID_NUS_SERVICE		0x0001
//#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */

//#define APP_ADV_INTERVAL                64                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */

//#define APP_ADV_DURATION                18000 

//NRF_BLE_GATT_DEF(m_gatt);                                                           /**< GATT module instance. */
////NRF_BLE_QWR_DEF(m_qwr);                                                             /**< Context for the Queued Write module.*/
////BLE_ADVERTISING_DEF(m_advertising);  

//static uint16_t   m_conn_handle          = BLE_CONN_HANDLE_INVALID;                 /**< Handle of the current connection. */
//static uint16_t   m_ble_nus_max_data_len = BLE_GATT_ATT_MTU_DEFAULT - 3;            /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Nordic UART service module. */
//static ble_uuid_t m_adv_uuids[]          =                                          /**< Universally unique service identifier. */
//{
//    {BLE_UUID_NUS_SERVICE, NUS_SERVICE_UUID_TYPE}
//};


//void ble_stack_init(void);
//void gap_params_init(void);
//void gatt_init(void);
//void advertising_init(void);
//void services_init(void);
//void conn_params_init(void);
//void advertising_start(void);
//// TODO ---------------------------  

//uint8_t constexpr bleUartServiceUuid = 0x0001;  /**< The UUID of the UART Service. */
//uint8_t constexpr baseID[16] = {0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0, 0x93, 0xF3, 0xA3, 0xB5, 0x00, 0x00, 0x40, 0x6E}; /**< Used vendor specific UUID. */ 


//typedef enum 
//{
//    UuidTxCharacteristic = 0x003,
//    UuidRxCharacteristic = 0x002
//} UuidCharacteristics;

//#define OPCODE_LENGTH        1
//#define HANDLE_LENGTH        2

///**@brief   Maximum length of data (in bytes) that can be transmitted to the peer by the Nordic UART service module. */
//uint32_t constexpr uartMaxDataLength =  NRF_SDH_BLE_GATT_MAX_MTU_SIZE - OPCODE_LENGTH - HANDLE_LENGTH;

//typedef enum
//{
//    bleMaxRxCharLength =		uartMaxDataLength,   /**< Maximum length of the RX Characteristic (in bytes). */
//    bleMaxTxCharLength =		uartMaxDataLength    /**< Maximum length of the TX Characteristic (in bytes). */
//} MaxCharacteristicLength;


///** @brief  Bluetooth Low Energy UUID type, encapsulates both 16-bit and 128-bit UUIDs. */
//typedef struct
//{
//  uint16_t    uuid; /**< 16-bit UUID value or octets 12-13 of 128-bit UUID. */
//  uint8_t     type; /**< UUID type, see @ref BLE_UUID_TYPES. If type is @ref BLE_UUID_TYPE_UNKNOWN, the value of uuid is undefined. */
//} bleUuidType_t;


///**@brief   Nordic UART Service @ref BLE_NUS_EVT_RX_DATA event data.
// *
// * @details This structure is passed to an event when @ref BLE_NUS_EVT_RX_DATA occurs.
// */
//typedef struct
//{
//    uint8_t const * pData;	  /**< A pointer to the buffer with received data. */
//    uint16_t        length;	  /**< Length of received data. */
//} BleUartEventRxData;

//// TODO - (change to camelCase) UART service event types
//typedef enum
//{
//    //BLE_NUS_EVT_RX_DATA,      /**< Data received. */
//    //BLE_NUS_EVT_TX_RDY,       /**< Service is ready to accept new data to be transmitted. */
//    //BLE_NUS_EVT_COMM_STARTED, /**< Notification has been enabled. */
//    //BLE_NUS_EVT_COMM_STOPPED, /**< Notification has been disabled. */
//} BleUartEvtType_t;

//typedef struct 
//{
//    BleUartEvtType_t	  type;
//    uint16_t		  connectionHandle;	  // TODO - verify its usage!
//    union
//    {
//        BleUartEventRxData rxData;
//    } params;

//} BleUartEvent_t;

//typedef void (*bleUartSrvHandler)(BleUartEvent_t *pEvent);

//typedef struct
//{
//    uint8_t		  uuidType;
//    uint16_t		  serviceHandle;
//    ble_gatts_char_handles_t	  txHandles;
//    ble_gatts_char_handles_t	  rxHandles;
//    bleUartSrvHandler	  dataHandler;
//    bleUuidType_t		  bleUuid;
//} BleUartSrv_t;

//class BleUartService 
//{
//    BleUartSrv_t		  bleService;

//    public:
//    BleUartService(BleUartSrv_t *pService);
//    uint32_t initService(BleUartSrv_t *pService);
    
//};

//void ble_stack_init();
//void    gap_params_init();
//void    gatt_init();
//void    services_init();
//void    advertising_init();
//void    conn_params_init();
//void    advertising_start();

//#endif