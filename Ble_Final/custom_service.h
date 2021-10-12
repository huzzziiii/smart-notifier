#ifndef BLE_CUSTOM_SERVICE_H
#define BLE_CUSTOM_SERVICE_H

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
#include "ble_common.h"
#include "SystemTask.hpp"


#define CUSTOM_SERVICE_UUID_BASE         {0xBC, 0x8A, 0xBF, 0x45, 0xCA, 0x05, 0x50, 0xBA, \
                                          0x40, 0x42, 0xB0, 0x00, 0xC9, 0xAD, 0x64, 0xF3}

#define CUSTOM_SERVICE_UUID               0x1400
#define CUSTOM_VALUE_CHAR_UUID            0x1401

//#include "controller.h

//typedef struct
//{
//    uint8_t const *rxBuffer;
//    uint16_t rxdBytes;
//} BleCusRxData;

///**@brief  Custom Service event types. */
//typedef enum
//{
//    bleCusEvtRxData,	      /**< Data received. */
//    bleCusEvtTxRdy,		      /**< Service is ready to accept new data to be transmitted. */
//    bleCusEvtCommStarted,	      /**< Notification has been enabled. */
//    bleCusEvtCommStopped	      /**< Notification has been disabled. */
//} BleCusEvtType;

//typedef struct
//{
//    //BleCustSrvInfo	*pCustSrvInfo; 
//    BleCusEvtType	eventType;
//    uint16_t	connectionHandle;
//    BleCusRxData	rxData;
//} CustEvent;

//typedef void (*BleCustDataHndlr)(CustEvent *p_evt);

/**@brief Custom Service init structure. This contains all options and data needed for
 *        initialization of the service. */
typedef struct
{
    uint8_t                       initCustomValue;      /**< Initial custom value */
    ble_srv_cccd_security_mode_t  customCharAttr;	      /**< Initial security level for Custom characteristics attribute */
    BleCustDataHndlr	    DataHandler;	      /**< Callback for data handling */
} CustInitChar;

void ble_cus_on_ble_evt( ble_evt_t const * p_ble_evt, void * p_context);

//// registering the ble_cus_on_ble_evt event handler as event observer using the NRF_SDH_BLE_OBSERVER() macro
//#define BLE_CUS_DEF(_name)                                                                          \
//static BleCustSrvInfo _name;                                                                             \
//NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                                 \
//                     BLE_HRS_BLE_OBSERVER_PRIO,                                                     \
//                     ble_cus_on_ble_evt, &_name)

//typedef struct BleCust_s BleCustSrvInfo;

///**@brief Custom Service structure. This contains various status information for the service. */
//struct BleCust_s
//{
//    uint16_t                      serviceHandle;		  /**< Handle of Custom Service (as provided by the BLE stack). */
//    ble_gatts_char_handles_t      customValueHandle;		  /**< Handles related to the Custom Value characteristic. */
//    uint16_t                      connectionHandle;                   /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
//    uint8_t                       uuidType;
//    //BleCustDataHndlr	    DataHandler;	      /**< Callback for data handling */
//};



class BleCustomService
{
    //CustEvent custEvent;
    CustInitChar bleInitChar;
    //BleCustSrvInfo custSrvInfo;
    SystemTask *_systemTask;

    public:
    BleCustomService(SystemTask *systemTask) : _systemTask(systemTask) {}

    //void Init(BleCustDataHndlr custHandler, CustEventHandler EventHandler);
    void Init();

    uint32_t ServiceInit(BleCustSrvInfo *pSrvInfo, CustInitChar *custInitCharac);
    uint32_t AddCustChar();
    BleCustDataHndlr GetDataHdnlr();
    //void CustDataHandler();
};

#endif 