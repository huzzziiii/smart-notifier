#ifndef BLE_COMMON_H
#define BLE_COMMON_H

//#include "controller.h"

typedef struct
{
    uint8_t const *rxBuffer;
    uint16_t rxdBytes;
} BleCusRxData;

/**@brief  Custom Service event types. */
typedef enum
{
    bleCusEvtRxData,	      /**< Data received. */
    bleCusEvtTxRdy,		      /**< Service is ready to accept new data to be transmitted. */
    bleCusEvtCommStarted,	      /**< Notification has been enabled. */
    bleCusEvtCommStopped	      /**< Notification has been disabled. */
} BleCusEvtType;

// registering the ble_cus_on_ble_evt event handler as event observer using the NRF_SDH_BLE_OBSERVER() macro
#define BLE_CUS_DEF(_name)                                                                          \
static BleCustSrvInfo _name;                                                                             \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                                 \
                     BLE_HRS_BLE_OBSERVER_PRIO,                                                     \
                     ble_cus_on_ble_evt, &_name)


typedef struct BleCust_s BleCustSrvInfo;

typedef struct
{
    //BleCustSrvInfo	*pCustSrvInfo; 
    BleCusEvtType	eventType;
    uint16_t	connectionHandle;
    BleCusRxData	rxData;
} CustEvent;

typedef void (*BLE_SRV_HDLR)(CustEvent *bleCustEvent);

typedef void (*BleCustDataHndlr)(CustEvent *p_evt);


/**@brief Custom Service structure. This contains various status information for the service. */
struct BleCust_s
{
    uint16_t                      serviceHandle;			  /**< Handle of Custom Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t      customValueHandle;		  /**< Handles related to the Custom Value characteristic. */
    uint16_t                      connectionHandle;                    /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
    uint8_t                       uuidType;
    BleCustDataHndlr	    DataHandler;	      /**< Callback for data handling */
};

//typedef struct
//{
//    //BleCustSrvInfo	*pCustSrvInfo; 
//    BleCusEvtType	eventType;
//    uint16_t	connectionHandle;
//    BleCusRxData	rxData;
//} CustEvent;




//typedef void (*BleCustDataHndlr)(CustEvent *p_evt);



#endif 