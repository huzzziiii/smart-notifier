#include "ble_cust_service.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_log.h"
#include "sdk_common.h"
#include "ble_srv_common.h"

/* Function prototypes */
/**@brief Function for initializing the Custom Service.
 *
 * @param[out]  p_cus       Custom Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_cus_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t BleCustInit(BleCust *pCustom, BleCustInit_t * pCustInit)
{
    if (pCustom == NULL || pCustInit == NULL)
    {
        return NRF_ERROR_NULL;
    }
    uint32_t   errorCode;
    ble_uuid_t bleUuid;

    // Initialize service structure
    pCustom->connectionHandle               = BLE_CONN_HANDLE_INVALID;

    // Add Custom Service UUID
    ble_uuid128_t baseUuid = {CUSTOM_SERVICE_UUID_BASE};
    errorCode =  sd_ble_uuid_vs_add(&baseUuid, &pCustom->uuidType);
    VERIFY_SUCCESS(errorCode);
    
    bleUuid.type = pCustom->uuidType;
    bleUuid.uuid = CUSTOM_SERVICE_UUID;

    //// Add the Custom Service
    errorCode = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &bleUuid, &pCustom->serviceHandle);
 
    return errorCode;
}

BLE_CUS_DEF(m_cust);
void cust_services_init()
{
    ret_code_t         err_code;
    //nrf_ble_qwr_init_t qwr_init = {0};
    BleCustInit_t     cus_init;

    // Initialize Queued Write Module.
    //qwr_init.error_handler = nrf_qwr_error_handler;

    //err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    //APP_ERROR_CHECK(err_code);

    // Initialize CUS Service init structure to zero.
    memset(&cus_init, 0, sizeof(cus_init));
	
    err_code = BleCustInit(&m_cust, &cus_init);
    APP_ERROR_CHECK(err_code);	
}
s