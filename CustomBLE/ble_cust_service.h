#ifndef BLE_CUST_SERVICE_H__
#define BLE_CUST_SERVICE_H__

#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"
//#include "nrf_ble_qwr.h"
#define CUSTOM_SERVICE_UUID_BASE         {0xBC, 0x8A, 0xBF, 0x45, 0xCA, 0x05, 0x50, 0xBA, \
                                          0x40, 0x42, 0xB0, 0x00, 0xC9, 0xAD, 0x64, 0xF3}

#define CUSTOM_SERVICE_UUID               0x1400
#define CUSTOM_VALUE_CHAR_UUID            0x1401

//#define BLE_CUS_DEF(_name)                                                                          \
//static BleCust _name;  


// registering the ble_cus_on_ble_evt event handler as event observer using the NRF_SDH_BLE_OBSERVER() macro
#define BLE_CUS_DEF(_name)                                                                          \
static BleCust _name;                                                                             \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                                 \
                     BLE_HRS_BLE_OBSERVER_PRIO,                                                     \
                     ble_cus_on_ble_evt, &_name)






///**@brief Custom Service init structure. This contains all options and data needed for
// *        initialization of the service.*/
//typedef struct
//{
//    uint8_t                       initCustomValue;	        /**< Initial custom value */
//    ble_srv_cccd_security_mode_t  customCharAttr;     /**< Initial security level for Custom characteristics attribute */
//} BleCustInit_t;

///**@brief Custom Service structure. This contains various status information for the service. */
//typedef struct 
//{
//    uint16_t                      serviceHandle;			  /**< Handle of Custom Service (as provided by the BLE stack). */
//    ble_gatts_char_handles_t      customValueHandle;		  /**< Handles related to the Custom Value characteristic. */
//    uint16_t                      connectionHandle;                    /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
//    uint8_t                       uuidType; 
//} BleCust;



//uint32_t BleCustInit(BleCust *pCustom, const BleCustInit_t * pCustInit);
//uint32_t BleCustInit(BleCust *pCustom, BleCustInit_t * pCustInit);
//void cust_services_init();

void ble_cus_on_ble_evt( ble_evt_t const * p_ble_evt, void * p_context);



#endif