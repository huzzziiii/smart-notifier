#ifndef BLE_CONTROLLER_H
#define BLE_CONTROLLER_H

#include "custom_service.h"

#define APP_BLE_CONN_CFG_TAG            1                                           /**< A tag identifying the SoftDevice BLE configuration. */

#define DEVICE_NAME                     "HUZZi-CUS"				    /**< Name of device. Will be included in the advertising data. */
#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */

#define APP_BLE_OBSERVER_PRIO           3                                           /**< Application's BLE observer priority. You shouldn't need to modify this value. */

#define APP_ADV_INTERVAL                64                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */

#define APP_ADV_DURATION                18000                                       /**< The advertising duration (180 seconds) in units of 10 milliseconds. */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(20, UNIT_1_25_MS)             /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(75, UNIT_1_25_MS)             /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)                       /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)                      /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

#define DEAD_BEEF                       0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define UART_TX_BUF_SIZE                256                                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE                256                                         /**< UART RX buffer size. */

/* This code belongs in ble_cus.h*/
#define CUSTOM_SERVICE_UUID               0x1400
#define CUSTOM_VALUE_CHAR_UUID            0x1401

#define BLE_UUID_TYPE_VENDOR_BEGIN	  0x02 /**< Vendor UUID types start at this index (128-bit). */

//BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT);                                   /**< BLE NUS service instance. */
                                                         /**< GATT module instance. */
                                                         /**< Context for the Queued Write module.*/
BLE_ADVERTISING_DEF(m_advertising);   



//static BleCustSrvInfo _bleCustSrvInfo;

//typedef struct
//{
//    uint8_t const *rxBuffer;
//    uint16_t rxdBytes;
//} BleCusRxData;

/**@brief  Custom Service event types. */
//typedef enum
//{
//    bleCusEvtRxData,	      /**< Data received. */
//    bleCusEvtTxRdy,		      /**< Service is ready to accept new data to be transmitted. */
//    bleCusEvtCommStarted,	      /**< Notification has been enabled. */
//    bleCusEvtCommStopped	      /**< Notification has been disabled. */
//} BleCusEvtType;


static uint16_t   m_conn_handle          = BLE_CONN_HANDLE_INVALID;                 /**< Handle of the current connection. */
static uint16_t   m_ble_nus_max_data_len = BLE_GATT_ATT_MTU_DEFAULT - 3;            /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Nordic UART service module. */

static ble_uuid_t m_adv_uuids_cust[]          =                                          /**< Universally unique service identifier. */
{
    {CUSTOM_SERVICE_UUID, BLE_UUID_TYPE_VENDOR_BEGIN}
};

//typedef struct
//{
//    //BleCustSrvInfo	*pCustSrvInfo; 
//    BleCusEvtType	eventType;
//    uint16_t	connectionHandle;
//    BleCusRxData	rxData;
//} CustEvent;

//typedef void (*BleCustDataHndlr)(CustEvent *p_evt);

class BleController
{   
    BleCustomService &_bleCustomSrv;

    public:
    BleController(BleCustomService &bleCustomSrv) : _bleCustomSrv(bleCustomSrv) {}
    void Init(BleCustDataHndlr custSrvHdlr);
    void InitCommParams();
};

#endif 