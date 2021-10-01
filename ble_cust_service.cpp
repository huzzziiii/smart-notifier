#include "ble_cust_service.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_log.h"
#include "sdk_common.h"
#include "ble_srv_common.h"

//static uint32_t custom_value_char_add(BleCust * p_cus, const BleCustInit_t * p_cus_init)
//{
//    uint32_t            err_code;
//    ble_gatts_char_md_t char_md;
//    ble_gatts_attr_md_t cccd_md;
//    ble_gatts_attr_t    attr_char_value;
//    ble_uuid_t          ble_uuid;
//    ble_gatts_attr_md_t attr_md;

//    memset(&char_md, 0, sizeof(char_md));

//    char_md.char_props.read   = 1;
//    char_md.char_props.write  = 1;
//    char_md.char_props.notify = 0; 
//    char_md.p_char_user_desc  = NULL;
//    char_md.p_char_pf         = NULL;
//    char_md.p_user_desc_md    = NULL;
//    char_md.p_cccd_md         = NULL; 
//    char_md.p_sccd_md         = NULL;
		
//    memset(&attr_md, 0, sizeof(attr_md));

//    attr_md.read_perm  = p_cus_init->customCharAttr.read_perm;
//    attr_md.write_perm = p_cus_init->customCharAttr.write_perm;
//    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
//    attr_md.rd_auth    = 0;
//    attr_md.wr_auth    = 0;
//    attr_md.vlen       = 0;

//    ble_uuid.type = p_cus->uuidType;
//    ble_uuid.uuid = CUSTOM_VALUE_CHAR_UUID;

//    memset(&attr_char_value, 0, sizeof(attr_char_value));

//    attr_char_value.p_uuid    = &ble_uuid;
//    attr_char_value.p_attr_md = &attr_md;
//    attr_char_value.init_len  = sizeof(uint8_t);
//    attr_char_value.init_offs = 0;
//    attr_char_value.max_len   = sizeof(uint8_t);

//    err_code = sd_ble_gatts_characteristic_add(p_cus->serviceHandle, &char_md,
//                                               &attr_char_value,
//                                               &p_cus->customValueHandle);
//    if (err_code != NRF_SUCCESS)
//    {
//        return err_code;
//    }

//    return NRF_SUCCESS;
//}

/* Function prototypes */
///**@brief Function for initializing the Custom Service.
// *
// * @param[out]  p_cus       Custom Service structure. This structure will have to be supplied by
// *                          the application. It will be initialized by this function, and will later
// *                          be used to identify this particular service instance.
// * @param[in]   p_cus_init  Information needed to initialize the service.
// *
// * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
// */
//uint32_t BleCustInit(BleCust *pCustom, BleCustInit_t * pCustInit)
//{
//    if (pCustom == NULL || pCustInit == NULL)
//    {
//        return NRF_ERROR_NULL;
//    }
//    uint32_t   errorCode;
//    ble_uuid_t bleUuid;

//    // Initialize service structure
//    pCustom->connectionHandle = BLE_CONN_HANDLE_INVALID;

//    // Add Custom Service UUID
//    ble_uuid128_t baseUuid = {CUSTOM_SERVICE_UUID_BASE};
//    errorCode =  sd_ble_uuid_vs_add(&baseUuid, &pCustom->uuidType);
//    VERIFY_SUCCESS(errorCode);
    
//    bleUuid.type = pCustom->uuidType;
//    bleUuid.uuid = CUSTOM_SERVICE_UUID;

//    //// Add the Custom Service
//    errorCode = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &bleUuid, &pCustom->serviceHandle);
    
//    // Add Custom Value characteristic
//    return custom_value_char_add(pCustom, pCustInit);

//    //return errorCode;
//}

//static void nrf_qwr_error_handler(uint32_t nrf_error)
//{
//    APP_ERROR_HANDLER(nrf_error);
//}

//BLE_CUS_DEF(m_cust);
//NRF_BLE_QWR_DEF(m_qwr); 

//void cust_services_init()
//{
//    ret_code_t         err_code;
//    nrf_ble_qwr_init_t qwr_init = {0};
//    BleCustInit_t     cus_init;

//    // Initialize Queued Write Module.
//    qwr_init.error_handler = nrf_qwr_error_handler;

//    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
//    APP_ERROR_CHECK(err_code);

//    // Initialize custom init service init structure to zero.
//    memset(&cus_init, 0, sizeof(cus_init));
	
//    err_code = BleCustInit(&m_cust, &cus_init);
//    APP_ERROR_CHECK(err_code);	
//}

/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_cus       Custom Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
//static void on_connect(BleCust *p_cus, ble_evt_t const * p_ble_evt)
//{
//    p_cus->connectionHandle = p_ble_evt->evt.gap_evt.conn_handle;
//}

///**@brief Function for handling the Disconnect event.
// *
// * @param[in]   p_cus       Custom Service structure.
// * @param[in]   p_ble_evt   Event received from the BLE stack.
// */
//static void on_disconnect(BleCust *p_cus, ble_evt_t const * p_ble_evt)
//{
//    UNUSED_PARAMETER(p_ble_evt);
//    p_cus->connectionHandle = BLE_CONN_HANDLE_INVALID;
//}

/**@brief Function for handling the Write event.
 *
 * @param[in]   p_cus       Custom Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
//static void on_write(BleCust * p_cus, ble_evt_t const * p_ble_evt)
//{
//    ble_gatts_evt_write_t const *p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
    
//    // Check if the handle passed with the event matches the Custom Value Characteristic handle.
//    if (p_evt_write->handle == p_cus->customValueHandle.value_handle)
//    {
//        nrf_gpio_pin_toggle(LED_4);  // TODO - do useful stuff! 
//    }
//}

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Battery Service.
 *
 * @note 
 *
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 * @param[in]   p_context  Custom Service structure.
 */
//void ble_cus_on_ble_evt( ble_evt_t const * p_ble_evt, void * p_context)
//{
//    BleCust * p_cus = (BleCust *) p_context;
    
//    if (p_cus == NULL || p_ble_evt == NULL)
//    {
//        return;
//    }

//    switch (p_ble_evt->header.evt_id)
//    {
//        case BLE_GAP_EVT_CONNECTED:
//	  on_connect(p_cus, p_ble_evt);
//            break;

//        case BLE_GAP_EVT_DISCONNECTED:
//	  on_disconnect(p_cus, p_ble_evt);
//            break;
        
//        case BLE_GATTS_EVT_WRITE:
//	  on_write(p_cus, p_ble_evt);
//	  break;

//        default:
//            // No implementation needed.
//            break;
//    }
//}