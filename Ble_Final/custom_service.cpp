#include "custom_service.h"

BLE_CUS_DEF(m_cust);

static SystemTask *systemTask;

void DataHandler(CustEvent *bleCustEvent)
{
    uint8_t rxdBytes[100] = {0};
    SystemTask::Messages systemMsg;

    for (uint8_t idx = 0; idx < bleCustEvent->rxData.rxdBytes; idx++)
    {
        rxdBytes[idx] = bleCustEvent->rxData.rxBuffer[idx];
    }
        
    uint8_t const *rxData = bleCustEvent->rxData.rxBuffer;
    char const val = (char ) *rxData;
    int a;
    a++;
    
    systemMsg = convertParsedInputToMsg((char *) rxdBytes);
    systemTask->pushMessage(systemMsg);

}

// TODO - remove
BleCustDataHndlr BleCustomService::GetDataHdnlr()
{
     return this->bleInitChar.DataHandler;
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
    char_md.char_props.notify = 0; 
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

    err_code = sd_ble_gatts_characteristic_add(p_cus->serviceHandle, &char_md,
                                               &attr_char_value,
                                               &p_cus->customValueHandle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}


uint32_t BleCustomService::ServiceInit(BleCustSrvInfo *pSrvInfo, CustInitChar *custInitCharac)
{
    //if (pCustom == NULL || pCustInit == NULL)
    //{
    //    return NRF_ERROR_NULL;
    //}
    uint32_t   errorCode;
    ble_uuid_t bleUuid;

    // Initialize service structure
    pSrvInfo->connectionHandle               = BLE_CONN_HANDLE_INVALID;
    pSrvInfo->DataHandler = DataHandler;

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
    return errorCode;
}

static void nrf_qwr_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


NRF_BLE_QWR_DEF(m_qwr);  // TODO needed??
void BleCustomService::Init()
{
    ret_code_t         err_code;
    nrf_ble_qwr_init_t qwr_init = {0};
    CustInitChar	   custInitCharac;

    systemTask = _systemTask;
  
    ////BleCustInit_t     cus_init;

    // Initialize Queued Write Module.
    qwr_init.error_handler = nrf_qwr_error_handler;

    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);



    // Initialize CUS Service init structure to zero.
    //memset(&cus_init, 0, sizeof(cus_init));
	
    err_code = ServiceInit(&m_cust, &custInitCharac);
    APP_ERROR_CHECK(err_code);

}