#include "custom_service.h"


BLE_CUS_DEF(m_cust);


void BleCustomService::Send(uint8_t *data)
{
    uint16_t               bytesToTx = 5; //1;
    ble_gatts_hvx_params_t hvx_params;
    memset(&hvx_params, 0, sizeof(hvx_params));
    ClientContext *clientContext;
    int connHandle;
    
    if (!_pSrvInfo->isNotificationEnabled)
    {
        return;
    }
    //int err_code = blcm_link_ctx_get(_pSrvInfo->p_link_ctx_storage, _pSrvInfo->connectionHandle, (void **) &clientContext);
    //if (err_code != NRF_SUCCESS)
    //{
    //    NRF_LOG_ERROR("Link context for 0x%02X connection handle could not be fetched.");
    //}

    //// TODO - define isNotificationEnabled in _pSrvInfo struct ....
    //if (!clientContext->isNotificationEnabled)
    //{
    //    return;	    // TODO -- make Send() return of int
    //}

        // Send value if connected and notifying
    if (_pSrvInfo->connectionHandle != BLE_CONN_HANDLE_INVALID)
    {
        hvx_params.handle = _pSrvInfo->rxCustomValueHandle.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len  = &bytesToTx;
        hvx_params.p_data = data;  
        
        ret_code_t  err_code = sd_ble_gatts_hvx(_pSrvInfo->connectionHandle, &hvx_params);
        NRF_LOG_INFO("sd_ble_gatts_hvx()");

        if (err_code == BLE_ERROR_GATTS_SYS_ATTR_MISSING)
        {
	  err_code = sd_ble_gatts_sys_attr_set(_pSrvInfo->connectionHandle, NULL, 0, 0);
	  APP_ERROR_CHECK(err_code);
	  NRF_LOG_INFO("BLE_ERROR_GATTS_SYS_ATTR_MISSING");

	  // resending 
	  err_code = sd_ble_gatts_hvx(_pSrvInfo->connectionHandle, &hvx_params);
	  //APP_ERROR_CHECK(err_code);   
        }
        nrf_gpio_pin_toggle(LED_2); 
        APP_ERROR_CHECK(err_code);  
     }
}


static uint32_t custom_value_char_add(BleCustSrvInfo *p_cus, const CustInitChar * p_cus_init)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.char_props.notify = 1; 
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL; 
    char_md.p_sccd_md         = NULL;
		
    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_cus_init->customCharAttr.read_perm;
    attr_md.write_perm = p_cus_init->customCharAttr.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    ble_uuid.type = p_cus->uuidType;
    ble_uuid.uuid = CUSTOM_VALUE_CHAR_UUID;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(uint8_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = sizeof(uint8_t) * 10;	// TODO - modify the max length

    err_code = sd_ble_gatts_characteristic_add(p_cus->serviceHandle, 
				       &char_md,
                                               &attr_char_value,
                                               &p_cus->rxCustomValueHandle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}


// initialize the service
uint32_t BleCustomService::ServiceInit(BleCustSrvInfo *pSrvInfo, CustInitChar *custInitCharac, BleCustDataHndlr srvDataHdlr)
{
    //if (pCustom == NULL || pCustInit == NULL)
    //{
    //    return NRF_ERROR_NULL;
    //}
    uint32_t   errorCode;
    ble_uuid_t bleUuid;

    _pSrvInfo = pSrvInfo;

    // Initialize service structure
    pSrvInfo->connectionHandle = BLE_CONN_HANDLE_INVALID;
        //pSrvInfo->DataHandler = DataHandler;
    pSrvInfo->DataHandler = srvDataHdlr;

    // Add Custom Service UUID
    ble_uuid128_t baseUuid = {CUSTOM_SERVICE_UUID_BASE};
    errorCode =  sd_ble_uuid_vs_add(&baseUuid, &pSrvInfo->uuidType);
    VERIFY_SUCCESS(errorCode);
    
    bleUuid.type = pSrvInfo->uuidType;
    bleUuid.uuid = CUSTOM_SERVICE_UUID;

    // Add the Custom Service
    errorCode = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &bleUuid, &pSrvInfo->serviceHandle);
    
    // Add read/write permissions for the characteristics
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&custInitCharac->customCharAttr.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&custInitCharac->customCharAttr.write_perm); //(&cus_init.custom_value_char_attr_md.write_perm);

    // Add Custom Value characteristic
    return custom_value_char_add(pSrvInfo, custInitCharac);
//    return errorCode;
}

static void nrf_qwr_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


NRF_BLE_QWR_DEF(m_qwr);  // TODO needed??
void BleCustomService::Init(BleCustDataHndlr srvDataHdlr)
{
    ret_code_t         err_code;
    nrf_ble_qwr_init_t qwr_init = {0};
    CustInitChar	   custInitCharac;

    // Initialize Queued Write Module.
    qwr_init.error_handler = nrf_qwr_error_handler;

    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);

    // Initialize CUS Service init structure to zero.
    //memset(&cus_init, 0, sizeof(cus_init));
	
    err_code = ServiceInit(&m_cust, &custInitCharac, srvDataHdlr);
    APP_ERROR_CHECK(err_code);

}