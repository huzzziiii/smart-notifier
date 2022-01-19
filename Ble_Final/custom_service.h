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
#include "bsp_btn_ble.h"
#include "nrf_pwr_mgmt.h"
#include "sdk_config.h"
#include "ble_link_ctx_manager.h"
#include <stdint.h>
#include "ble_common.h"
#include "SystemTask.hpp"


#define CUSTOM_SERVICE_UUID_BASE         {0xBC, 0x8A, 0xBF, 0x45, 0xCA, 0x05, 0x50, 0xBA, \
                                          0x40, 0x42, 0xB0, 0x00, 0xC9, 0xAD, 0x64, 0xF3}

#define CUSTOM_SERVICE_UUID               0x1400
#define CUSTOM_VALUE_CHAR_UUID            0x1401


/**@brief Custom Service init structure. This contains all options and data needed for
 *        initialization of the service. */
typedef struct
{
    uint8_t                       initCustomValue;      /**< Initial custom value */
    ble_srv_cccd_security_mode_t  customCharAttr;	      /**< Initial security level for Custom characteristics attribute */
    BleCustDataHndlr	    DataHandler;	      /**< Callback for data handling */
    
} CustInitChar;

void ble_cus_on_ble_evt( ble_evt_t const * p_ble_evt, void * p_context);

class SystemTask;

typedef struct
{
    bool isNotificationEnabled; /**< Variable to indicate if the peer has enabled notification of the RX characteristic.*/
} ClientContext;


class BleCustomService
{
    CustInitChar bleInitChar;
    SystemTask *_systemTask;
    BleCustSrvInfo *_pSrvInfo;
    
    public:
    void Init(BleCustDataHndlr bleCustSrvHdlr);
    uint32_t ServiceInit(BleCustSrvInfo *pSrvInfo, CustInitChar *custInitCharac, BleCustDataHndlr srvDataHdlr);
    uint32_t AddCustChar();
    BleCustDataHndlr GetDataHdnlr();
    void Send(uint8_t *data);
};

#endif 