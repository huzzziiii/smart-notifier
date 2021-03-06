//#include "sdk_common.h"
//#include "ble_nus.h"
//#include "nrf_log.h"
#include "BleUartService.hpp"

//#define NRF_LOG_MODULE_NAME ble_nus
//NRF_LOG_MODULE_REGISTER();



//#define BLE_NUS_MAX_RX_CHAR_LEN        BLE_NUS_MAX_DATA_LEN /**< Maximum length of the RX Characteristic (in bytes). */
//#define BLE_NUS_MAX_TX_CHAR_LEN        BLE_NUS_MAX_DATA_LEN /**< Maximum length of the TX Characteristic (in bytes). */

#define NUS_BASE_UUID                  {{0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0, 0x93, 0xF3, 0xA3, 0xB5, 0x00, 0x00, 0x40, 0x6E}} /**< Used vendor specific UUID. */

static constexpr uint8_t bleUuidUartService = 0x0001;

/**@defgroup BLE_GAP_PHYS GAP PHYs
 * @{ */
#define BLE_GAP_PHY_AUTO                         0x00    /**< Automatic PHY selection. Refer @ref sd_ble_gap_phy_update for more information.*/
#define BLE_GAP_PHY_1MBPS                        0x01    /**< 1 Mbps PHY. */
#define BLE_GAP_PHY_2MBPS                        0x02    /**< 2 Mbps PHY. */
#define BLE_GAP_PHY_CODED                        0x04    /**< Coded PHY. */
#define BLE_GAP_PHY_NOT_SET                      0xFF    /**< PHY is not configured. */


///**@brief Function for handling the @ref BLE_GAP_EVT_CONNECTED event from the SoftDevice.
// *
// * @param[in] p_nus     Nordic UART Service structure.
// * @param[in] p_ble_evt Pointer to the event received from BLE stack.
// */
//static void on_connect(ble_nus_t * p_nus, ble_evt_t const * p_ble_evt)
//{
//    ret_code_t                 err_code;
//    ble_nus_evt_t              evt;
//    ble_gatts_value_t          gatts_val;
//    uint8_t                    cccd_value[2];
//    ble_nus_client_context_t * p_client = NULL;
    
//    NRF_LOG_INFO("ble_nus -- on_connect()\n");	  // huzzi 
//    err_code = blcm_link_ctx_get(p_nus->p_link_ctx_storage,
//                                 p_ble_evt->evt.gap_evt.conn_handle,
//                                 (void **) &p_client);
//    if (err_code != NRF_SUCCESS)
//    {
//        NRF_LOG_ERROR("Link context for 0x%02X connection handle could not be fetched.",
//                      p_ble_evt->evt.gap_evt.conn_handle);
//    }

//    /* Check the hosts CCCD value to inform of readiness to send data using the RX characteristic */
//    memset(&gatts_val, 0, sizeof(ble_gatts_value_t));
//    gatts_val.p_value = cccd_value;
//    gatts_val.len     = sizeof(cccd_value);
//    gatts_val.offset  = 0;

//    err_code = sd_ble_gatts_value_get(p_ble_evt->evt.gap_evt.conn_handle,
//                                      p_nus->tx_handles.cccd_handle,
//                                      &gatts_val);

//    if ((err_code == NRF_SUCCESS)     &&
//        (p_nus->data_handler != NULL) &&
//        ble_srv_is_notification_enabled(gatts_val.p_value))
//    {
//        if (p_client != NULL)
//        {
//            p_client->is_notification_enabled = true;
//        }

//        memset(&evt, 0, sizeof(ble_nus_evt_t));
//        evt.type        = BLE_NUS_EVT_COMM_STARTED;
//        evt.p_nus       = p_nus;
//        evt.conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
//        evt.p_link_ctx  = p_client;

//        p_nus->data_handler(&evt);
//    }
//}


///**@brief Function for handling the @ref BLE_GATTS_EVT_WRITE event from the SoftDevice.
// *
// * @param[in] p_nus     Nordic UART Service structure.
// * @param[in] p_ble_evt Pointer to the event received from BLE stack.
// */
//static void on_write(ble_nus_t * p_nus, ble_evt_t const * p_ble_evt)
//{
//    ret_code_t                    err_code;
//    ble_nus_evt_t                 evt;
//    ble_nus_client_context_t    * p_client;
//    ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

//    NRF_LOG_INFO("ble_nus -- on_write()\n");	  // huzzi

//    err_code = blcm_link_ctx_get(p_nus->p_link_ctx_storage,
//                                 p_ble_evt->evt.gatts_evt.conn_handle,
//                                 (void **) &p_client);
//    if (err_code != NRF_SUCCESS)
//    {
//        NRF_LOG_ERROR("Link context for 0x%02X connection handle could not be fetched.",
//                      p_ble_evt->evt.gatts_evt.conn_handle);
//    }

//    memset(&evt, 0, sizeof(ble_nus_evt_t));
//    evt.p_nus       = p_nus;
//    evt.conn_handle = p_ble_evt->evt.gatts_evt.conn_handle;
//    evt.p_link_ctx  = p_client;

//    if ((p_evt_write->handle == p_nus->tx_handles.cccd_handle) &&
//        (p_evt_write->len == 2))
//    {
//        if (p_client != NULL)
//        {
//            if (ble_srv_is_notification_enabled(p_evt_write->data))
//            {
//                p_client->is_notification_enabled = true;
//                evt.type                          = BLE_NUS_EVT_COMM_STARTED;
//            }
//            else
//            {
//                p_client->is_notification_enabled = false;
//                evt.type                          = BLE_NUS_EVT_COMM_STOPPED;
//            }

//            if (p_nus->data_handler != NULL)
//            {
//                p_nus->data_handler(&evt);
//            }

//        }
//    }
//    else if ((p_evt_write->handle == p_nus->rx_handles.value_handle) &&
//             (p_nus->data_handler != NULL))
//    {
//        NRF_LOG_INFO("HUZZI - BLE_NUS_EVT_RX_DATA\n");
//        evt.type                  = BLE_NUS_EVT_RX_DATA;
//        evt.params.rx_data.p_data = p_evt_write->data;
//        evt.params.rx_data.length = p_evt_write->len;

//        p_nus->data_handler(&evt);
//    }
//    else
//    {
//        // Do Nothing. This event is not relevant for this service.
//    }
//}


///**@brief Function for handling the @ref BLE_GATTS_EVT_HVN_TX_COMPLETE event from the SoftDevice.
// *
// * @param[in] p_nus     Nordic UART Service structure.
// * @param[in] p_ble_evt Pointer to the event received from BLE stack.
// */
//static void on_hvx_tx_complete(ble_nus_t * p_nus, ble_evt_t const * p_ble_evt)
//{
//    ret_code_t                 err_code;
//    ble_nus_evt_t              evt;
//    ble_nus_client_context_t * p_client;

//    err_code = blcm_link_ctx_get(p_nus->p_link_ctx_storage,
//                                 p_ble_evt->evt.gatts_evt.conn_handle,
//                                 (void **) &p_client);
//    if (err_code != NRF_SUCCESS)
//    {
//        NRF_LOG_ERROR("Link context for 0x%02X connection handle could not be fetched.",
//                      p_ble_evt->evt.gatts_evt.conn_handle);
//        return;
//    }

//    if ((p_client->is_notification_enabled) && (p_nus->data_handler != NULL))
//    {
//        memset(&evt, 0, sizeof(ble_nus_evt_t));
//        evt.type        = BLE_NUS_EVT_TX_RDY;
//        evt.p_nus       = p_nus;
//        evt.conn_handle = p_ble_evt->evt.gatts_evt.conn_handle;
//        evt.p_link_ctx  = p_client;

//        p_nus->data_handler(&evt);
//    }
//}


//void ble_nus_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
//{
//    if ((p_context == NULL) || (p_ble_evt == NULL))
//    {
//        return;
//    }
//    NRF_LOG_INFO("HUZZI - ble_nus_on_ble_evt\n");

//    ble_nus_t * p_nus = (ble_nus_t *)p_context;

//    //switch (p_ble_evt->header.evt_id)
//    //{
//    //    case BLE_GAP_EVT_CONNECTED:
//    //        on_connect(p_nus, p_ble_evt);
//    //        break;

//    //    case BLE_GATTS_EVT_WRITE:
//    //        on_write(p_nus, p_ble_evt);
//    //        break;

//    //    case BLE_GATTS_EVT_HVN_TX_COMPLETE:
//    //        on_hvx_tx_complete(p_nus, p_ble_evt);
//    //        break;

//    //    default:
//    //        // No implementation needed.
//    //        break;
//    //}
//}

NRF_BLE_QWR_DEF(m_qwr); 
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    ret_code_t err_code = NRF_SUCCESS;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_DISCONNECTED:
            NRF_LOG_INFO("Disconnected.");
            // LED indication will be changed when advertising starts.
            break;

        case BLE_GAP_EVT_CONNECTED:
            NRF_LOG_INFO("Connected.");
            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
            APP_ERROR_CHECK(err_code);
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            NRF_LOG_DEBUG("PHY update request.");
            ble_gap_phys_t phys =
            {
                .tx_phys = BLE_GAP_PHY_AUTO,
	      .rx_phys = BLE_GAP_PHY_AUTO
                
            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            NRF_LOG_DEBUG("GATT Client Timeout.");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            NRF_LOG_DEBUG("GATT Server Timeout.");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        default:
            // No implementation needed.
            break;
    }
}

void ble_stack_init()
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);
    
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);  // Register a handler for BLE events.
}

void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *) DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling events from the GATT library. */
void gatt_evt_handler(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t const * p_evt)
{
    if ((m_conn_handle == p_evt->conn_handle) && (p_evt->evt_id == NRF_BLE_GATT_EVT_ATT_MTU_UPDATED))
    {
        m_ble_nus_max_data_len = p_evt->params.att_mtu_effective - OPCODE_LENGTH - HANDLE_LENGTH;
        NRF_LOG_INFO("Data len is set to 0x%X(%d)", m_ble_nus_max_data_len, m_ble_nus_max_data_len);
        NRF_LOG_INFO("Connection Handle: %x\n", m_conn_handle);
    }
    NRF_LOG_DEBUG("ATT MTU exchange completed. central 0x%x peripheral 0x%x",
                  p_gatt->att_mtu_desired_central,
                  p_gatt->att_mtu_desired_periph);
}

/**@brief Function for initializing the GATT library. */
void gatt_init(void)
{
    ret_code_t err_code;

    err_code = nrf_ble_gatt_init(&m_gatt, gatt_evt_handler);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_ble_gatt_att_mtu_periph_set(&m_gatt, NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
    APP_ERROR_CHECK(err_code);
}

void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    uint32_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            APP_ERROR_CHECK(err_code);
            break;
        //case BLE_ADV_EVT_IDLE:  //
        //    sleep_mode_enter();
        //    break;
        default:
            break;
    }
}

BLE_ADVERTISING_DEF(m_advertising);   
void advertising_init(void)
{
    uint32_t               err_code;
    ble_advertising_init_t init;

    memset(&init, 0, sizeof(init));

    size_t initSize = sizeof(init);
    size_t advdataSize = sizeof(init.advdata);

    init.advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    //init.advdata.name_type          = BLE_ADVDATA_SHORT_NAME;
    //init.advdata.short_name_len     = 6;
    init.advdata.include_appearance = true;
    init.advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;

    init.srdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    init.srdata.uuids_complete.p_uuids  = m_adv_uuids;

    init.config.ble_adv_fast_enabled  = true;
    init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
    init.config.ble_adv_fast_timeout  = APP_ADV_DURATION;
    init.evt_handler = on_adv_evt;

    err_code = ble_advertising_init(&m_advertising, &init);
    APP_ERROR_CHECK(err_code);

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}

/**@brief Function for handling an event from the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module
 *          which are passed to the application.
 *
 * @note All this function does is to disconnect. This could have been done by simply setting
 *       the disconnect_on_fail config parameter, but instead we use the event handler
 *       mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    uint32_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}

/**@brief Function for handling errors from the Connection Parameters module.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling Queued Write Module errors.
 *
 * @details A pointer to this function will be passed to each service which may need to inform the
 *          application about an error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void nrf_qwr_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

void nus_data_handler(ble_nus_evt_t * p_evt)
{

    if (p_evt->type == BLE_NUS_EVT_RX_DATA)
    {
        uint32_t err_code;

        NRF_LOG_DEBUG("Received data from BLE NUS. Writing data on UART.");
        NRF_LOG_HEXDUMP_DEBUG(p_evt->params.rx_data.p_data, p_evt->params.rx_data.length);

    }
}

/**@brief Function for starting advertising.
 */
void advertising_start(void)
{
    uint32_t err_code = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);
}

//NRF_BLE_QWR_DEF(m_qwr); 
BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT);  
void services_inxit(void)
{
    uint32_t           err_code;
    ble_nus_init_t     nus_init;
    nrf_ble_qwr_init_t qwr_init = {0};

    // Initialize Queued Write Module.
    qwr_init.error_handler = nrf_qwr_error_handler;

    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);

    // Initialize NUS.
    memset(&nus_init, 0, sizeof(nus_init));
    nus_init.data_handler = nus_data_handler;
    err_code = ble_nus_init(&m_nus, &nus_init);
    APP_ERROR_CHECK(err_code);
}



// TODO --------------------------------------------------------------------------

BleUartService::BleUartService(BleUartSrv_t *pService) 				      
{
    bleService.dataHandler = pService->dataHandler;
    bleService.bleUuid.type = pService->uuidType;
    bleService.bleUuid.uuid = bleUuidUartService;
    
    bleService.bleUuid.uuid = bleUartServiceUuid;
    //memcpy(uuid128.uuid128, baseID, 16);
}

uint32_t BleUartService::initService(BleUartSrv_t *pService)
{
    uint32_t		  errorCode;
    ble_uuid128_t		  uuidBase = NUS_BASE_UUID;
    ble_uuid_t		  bleUuid;
    ble_add_char_params_t	  characteristicParams;

    // Add a custom base UUID to the SoftDevice
    errorCode = sd_ble_uuid_vs_add(&uuidBase, &bleService.bleUuid.type);
    VERIFY_SUCCESS(errorCode);
    
    // Initialize our service as primary - after this call, our service can be uniquely identified by the BLE stack 
    errorCode = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                         &bleUuid,
                                         &bleService.serviceHandle);
    VERIFY_SUCCESS(errorCode);

    // Add the RX characteristic
    memset(&characteristicParams, 0, sizeof(characteristicParams));
    characteristicParams.uuid                     = UuidRxCharacteristic;
    characteristicParams.uuid_type                = bleService.uuidType;
    characteristicParams.max_len                  = bleMaxRxCharLength;
    characteristicParams.init_len                 = sizeof(uint8_t);
    characteristicParams.is_var_len               = true;
    characteristicParams.char_props.write         = 1;
    characteristicParams.char_props.write_wo_resp = 1;

    characteristicParams.read_access  = SEC_OPEN;
    characteristicParams.write_access = SEC_OPEN;

    errorCode = characteristic_add(bleService.serviceHandle, &characteristicParams, &bleService.rxHandles);
    if (errorCode != NRF_SUCCESS)
    {
        return errorCode;
    }

    // Add the TX characteristic
    memset(&characteristicParams, 0, sizeof(characteristicParams));
    characteristicParams.uuid                     = UuidTxCharacteristic;
    characteristicParams.uuid_type                = bleService.uuidType;
    characteristicParams.max_len                  = bleMaxTxCharLength;
    characteristicParams.init_len                 = sizeof(uint8_t);
    characteristicParams.is_var_len               = true;
    characteristicParams.char_props.write         = 1;
    characteristicParams.char_props.write_wo_resp = 1;

    characteristicParams.read_access  = SEC_OPEN;
    characteristicParams.write_access = SEC_OPEN;

    return characteristic_add(bleService.serviceHandle, &characteristicParams, &bleService.txHandles);
}

//uint32_t ble_nus_init(ble_nus_t * p_nus, ble_nus_init_t const * p_nus_init)
//{
//   ret_code_t            err_code;
 ble_uuid_t            ble_uuid;
 //ble_uuid128_t         nus_base_uuid = NUS_BASE_UUID;
//    ble_add_char_params_t add_char_params;

//    VERIFY_PARAM_NOT_NULL(p_nus);
//    VERIFY_PARAM_NOT_NULL(p_nus_init);

//    // Initialize the service structure.
//    p_nus->data_handler = p_nus_init->data_handler;

//    /**@snippet [Adding proprietary Service to the SoftDevice] */
//    // Add a custom base UUID.
//    err_code = sd_ble_uuid_vs_add(&nus_base_uuid, &p_nus->uuid_type);
//    VERIFY_SUCCESS(err_code);

//    ble_uuid.type = p_nus->uuid_type;
   //ble_uuid.uuid = BLE_UUID_NUS_SERVICE;

//    // Add the service.
//    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
//                                        &ble_uuid,
//                                        &p_nus->service_handle);
//    /**@snippet [Adding proprietary Service to the SoftDevice] */
//    VERIFY_SUCCESS(err_code);

//    // Add the RX Characteristic.
//    memset(&add_char_params, 0, sizeof(add_char_params));
//    add_char_params.uuid                     = BLE_UUID_NUS_RX_CHARACTERISTIC;
//    add_char_params.uuid_type                = p_nus->uuid_type;
//    add_char_params.max_len                  = BLE_NUS_MAX_RX_CHAR_LEN;
//    add_char_params.init_len                 = sizeof(uint8_t);
//    add_char_params.is_var_len               = true;
//    add_char_params.char_props.write         = 1;
//    add_char_params.char_props.write_wo_resp = 1;

//    add_char_params.read_access  = SEC_OPEN;
//    add_char_params.write_access = SEC_OPEN;

//    err_code = characteristic_add(p_nus->service_handle, &add_char_params, &p_nus->rx_handles);
//    if (err_code != NRF_SUCCESS)
//    {
//        return err_code;
//    }

//    // Add the TX Characteristic.
//    /**@snippet [Adding proprietary characteristic to the SoftDevice] */
//    memset(&add_char_params, 0, sizeof(add_char_params));
//    add_char_params.uuid              = BLE_UUID_NUS_TX_CHARACTERISTIC;
//    add_char_params.uuid_type         = p_nus->uuid_type;
//    add_char_params.max_len           = BLE_NUS_MAX_TX_CHAR_LEN;
//    add_char_params.init_len          = sizeof(uint8_t);
//    add_char_params.is_var_len        = true;
//    add_char_params.char_props.notify = 1;

//    add_char_params.read_access       = SEC_OPEN;
//    add_char_params.write_access      = SEC_OPEN;
//    add_char_params.cccd_write_access = SEC_OPEN;

//    return characteristic_add(p_nus->service_handle, &add_char_params, &p_nus->tx_handles);
//    /**@snippet [Adding proprietary characteristic to the SoftDevice] */
//}


//uint32_t ble_nus_data_send(ble_nus_t * p_nus,
//                           uint8_t   * p_data,
//                           uint16_t  * p_length,
//                           uint16_t    conn_handle)
//{
//    ret_code_t                 err_code;
//    ble_gatts_hvx_params_t     hvx_params;
//    ble_nus_client_context_t * p_client;
    
//    NRF_LOG_INFO("huzzi - ble_nus_data_send\n");
//    //NRF_LOG_FLUSH();

//    VERIFY_PARAM_NOT_NULL(p_nus);

//    err_code = blcm_link_ctx_get(p_nus->p_link_ctx_storage, conn_handle, (void **) &p_client);
//    VERIFY_SUCCESS(err_code);

//    if ((conn_handle == BLE_CONN_HANDLE_INVALID) || (p_client == NULL))
//    {
//        return NRF_ERROR_NOT_FOUND;
//    }

//    if (!p_client->is_notification_enabled)
//    {
//        return NRF_ERROR_INVALID_STATE;
//    }

//    if (*p_length > BLE_NUS_MAX_DATA_LEN)
//    {
//        return NRF_ERROR_INVALID_PARAM;
//    }

//    memset(&hvx_params, 0, sizeof(hvx_params));

//    hvx_params.handle = p_nus->tx_handles.value_handle;
//    hvx_params.p_data = p_data;
//    hvx_params.p_len  = p_length;
//    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

//    return sd_ble_gatts_hvx(conn_handle, &hvx_params);
//}


//#endif // NRF_MODULE_ENABLED(BLE_NUS)
