#ifndef __UART_H
#define __UART_H

#include "fifo.hpp"
#include "nrf_gpio.h"
//#include <string>
//#include <functional>

//#include "nrfx_prs.h"

typedef enum 
{
    StatusDisable,
    StatusEnable
} StatusSet;

typedef enum
{
    FlowCntlDisabled,		/**< UART Hw Flow Control is disabled. */
    FlowCntlEnabled,		/**< Standard UART Hw Flow Control is enabled. */
} UartFlowCntl_t;

typedef enum 
{
    UartPselDisconnected = 0xFFFFFFFF
} UartPselStatus;


//typedef enum
//{
//    APP_UART_DATA_READY,          /**< An event indicating that UART data has been received. The data is available in the FIFO and can be fetched using @ref app_uart_get. */
//    APP_UART_FIFO_ERROR,          /**< An error in the FIFO module used by the app_uart module has occured. The FIFO error code is stored in app_uart_evt_t.data.error_code field. */
//    APP_UART_COMMUNICATION_ERROR, /**< An communication error has occured during reception. The error is stored in app_uart_evt_t.data.error_communication field. */
//    APP_UART_TX_EMPTY,            /**< An event indicating that UART has completed transmission of all available data in the TX FIFO. */
//    APP_UART_DATA,                /**< An event indicating that UART data has been received, and data is present in data field. This event is only used when no FIFO is configured. */
//} app_uart_evt_type_t;

/** @brief Baudrates supported by UART. */
typedef enum
{
    UartBaudrate_1200    = UART_BAUDRATE_BAUDRATE_Baud1200,   /**< 1200 baud. */
    NRF_UART_BAUDRATE_2400    = UART_BAUDRATE_BAUDRATE_Baud2400,   /**< 2400 baud. */
    NRF_UART_BAUDRATE_4800    = UART_BAUDRATE_BAUDRATE_Baud4800,   /**< 4800 baud. */
    NRF_UART_BAUDRATE_9600    = UART_BAUDRATE_BAUDRATE_Baud9600,   /**< 9600 baud. */
    NRF_UART_BAUDRATE_14400   = UART_BAUDRATE_BAUDRATE_Baud14400,  /**< 14400 baud. */
    NRF_UART_BAUDRATE_19200   = UART_BAUDRATE_BAUDRATE_Baud19200,  /**< 19200 baud. */
    NRF_UART_BAUDRATE_28800   = UART_BAUDRATE_BAUDRATE_Baud28800,  /**< 28800 baud. */
    NRF_UART_BAUDRATE_31250   = UART_BAUDRATE_BAUDRATE_Baud31250,  /**< 31250 baud. */
    NRF_UART_BAUDRATE_38400   = UART_BAUDRATE_BAUDRATE_Baud38400,  /**< 38400 baud. */
    NRF_UART_BAUDRATE_56000   = UART_BAUDRATE_BAUDRATE_Baud56000,  /**< 56000 baud. */
    NRF_UART_BAUDRATE_57600   = UART_BAUDRATE_BAUDRATE_Baud57600,  /**< 57600 baud. */
    NRF_UART_BAUDRATE_76800   = UART_BAUDRATE_BAUDRATE_Baud76800,  /**< 76800 baud. */
    NRF_UART_BAUDRATE_115200  = UART_BAUDRATE_BAUDRATE_Baud115200, /**< 115200 baud. */
    NRF_UART_BAUDRATE_230400  = UART_BAUDRATE_BAUDRATE_Baud230400, /**< 230400 baud. */
    NRF_UART_BAUDRATE_250000  = UART_BAUDRATE_BAUDRATE_Baud250000, /**< 250000 baud. */
    NRF_UART_BAUDRATE_460800  = UART_BAUDRATE_BAUDRATE_Baud460800, /**< 460800 baud. */
    NRF_UART_BAUDRATE_921600  = UART_BAUDRATE_BAUDRATE_Baud921600, /**< 921600 baud. */
    NRF_UART_BAUDRATE_1000000 = UART_BAUDRATE_BAUDRATE_Baud1M,     /**< 1000000 baud. */
} UartBaudrate_t;

/** @brief Types of UART error masks.  TODO -- name*/
typedef enum
{
    NRF_UART_ERROR_OVERRUN_MASK = UART_ERRORSRC_OVERRUN_Msk,   /**< Overrun error. */
    NRF_UART_ERROR_PARITY_MASK  = UART_ERRORSRC_PARITY_Msk,    /**< Parity error. */
    NRF_UART_ERROR_FRAMING_MASK = UART_ERRORSRC_FRAMING_Msk,   /**< Framing error. */
    NRF_UART_ERROR_BREAK_MASK   = UART_ERRORSRC_BREAK_Msk,     /**< Break error. */
} UartErrorMask;

//__STATIC_INLINE void nrf_uart_event_clear(NRF_UART_Type * p_reg, nrf_uart_event_t event)
//{
//    *( (volatile uint32_t *)((uint8_t *)p_reg + (uint32_t)event)) = 0x0UL;
//#if __CORTEX_M == 0x04
//    volatile uint32_t dummy = *((volatile uint32_t *)((uint8_t *)p_reg + (uint32_t)event));
//    (void)dummy;
//#endif
//}


/** @brief UART events. */
typedef enum
{
    NRF_UART_EVENT_CTS    = offsetof(NRF_UART_Type, EVENTS_CTS),   /**< Event from CTS line activation. */
    NRF_UART_EVENT_NCTS   = offsetof(NRF_UART_Type, EVENTS_NCTS),  /**< Event from CTS line deactivation. */
    NRF_UART_EVENT_RXDRDY = offsetof(NRF_UART_Type, EVENTS_RXDRDY),/**< Event from data ready in RXD. */
    NRF_UART_EVENT_TXDRDY = offsetof(NRF_UART_Type, EVENTS_TXDRDY),/**< Event from data sent from TXD. */
    NRF_UART_EVENT_ERROR  = offsetof(NRF_UART_Type, EVENTS_ERROR), /**< Event from error detection. */
    NRF_UART_EVENT_RXTO   = offsetof(NRF_UART_Type, EVENTS_RXTO)   /**< Event from receiver timeout. */
} nrf_uart_event_t;

/** @brief UART interrupts. */
typedef enum
{
    NRF_UART_INT_MASK_CTS    = UART_INTENCLR_CTS_Msk,    /**< CTS line activation interrupt. */
    NRF_UART_INT_MASK_NCTS   = UART_INTENCLR_NCTS_Msk,   /**< CTS line deactivation interrupt. */
    NRF_UART_INT_MASK_RXDRDY = UART_INTENCLR_RXDRDY_Msk, /**< Data ready in RXD interrupt. */
    NRF_UART_INT_MASK_TXDRDY = UART_INTENCLR_TXDRDY_Msk, /**< Data sent from TXD interrupt. */
    NRF_UART_INT_MASK_ERROR  = UART_INTENCLR_ERROR_Msk,  /**< Error detection interrupt. */
    NRF_UART_INT_MASK_RXTO   = UART_INTENCLR_RXTO_Msk    /**< Receiver timeout interrupt. */
} nrf_uart_int_mask_t;

/** @brief UART tasks. */
typedef enum
{
    NRF_UART_TASK_STARTRX = offsetof(NRF_UART_Type, TASKS_STARTRX), /**< Task for starting reception. */
    NRF_UART_TASK_STOPRX  = offsetof(NRF_UART_Type, TASKS_STOPRX),  /**< Task for stopping reception. */
    NRF_UART_TASK_STARTTX = offsetof(NRF_UART_Type, TASKS_STARTTX), /**< Task for starting transmission. */
    NRF_UART_TASK_STOPTX  = offsetof(NRF_UART_Type, TASKS_STOPTX),  /**< Task for stopping transmission. */
    NRF_UART_TASK_SUSPEND = offsetof(NRF_UART_Type, TASKS_SUSPEND), /**< Task for suspending UART. */
} nrf_uart_task_t;

/** @brief Types of UART parity modes. */
typedef enum
{
    UartParityExcluded = UART_CONFIG_PARITY_Excluded << UART_CONFIG_PARITY_Pos, /**< Parity excluded. */
    UartParityIncluded = UART_CONFIG_PARITY_Included << UART_CONFIG_PARITY_Pos  /**< Parity included. */
} UartParity_t;

///** @brief Types of UART flow control modes. */
typedef enum
{
    UartHwFcDisabled = UART_CONFIG_HWFC_Disabled, /**< Hardware flow control disabled. */
    UartHwFcEnabled  = UART_CONFIG_HWFC_Enabled,  /**< Hardware flow control enabled. */
} UartHwFlow_t;

//typedef struct 
//{
//    uint8_t *rxBuffer;
//    uint8_t *txBuffer;
//    uint8_t txSize;
//    uint8_t rxSize;
//} UartBuffers_t;

typedef struct 
{
    uint8_t fifoTx;
    uint8_t fifoRx;
} UartBuffers_t;

typedef struct
{
    uint32_t                rxPinNo;    /**< RX pin number. */
    uint32_t                txPinNo;    /**< TX pin number. */
    uint32_t                rtsPinNo;   /**< RTS pin number, only used if flow control is enabled. */
    uint32_t                ctsPinNo;   /**< CTS pin number, only used if flow control is enabled. */
    UartHwFlow_t	        hwFlowCntl;	// ** TODO **/
    bool                    useParity;  /**< Even parity if TRUE, no parity if FALSE. */
    UartBaudrate_t          baudRate;   /**< Baud rate configuration. */
} UartCommParams_t;


//typedef struct
//{
//    uint32_t            pseltxd;            ///< TXD pin number.
//    uint32_t            pselrxd;            ///< RXD pin number.
//    uint32_t            pselcts;            ///< CTS pin number.
//    uint32_t            pselrts;            ///< RTS pin number.
//    void *              p_context;          ///< Context passed to interrupt handler.
//    nrf_uart_hwfc_t     hwfc;               ///< Flow control configuration.
//    nrf_uart_parity_t   parity;             ///< Parity configuration.
//    nrf_uart_baudrate_t baudrate;           ///< Baudrate.
//    uint8_t             interrupt_priority; ///< Interrupt priority.
//#if defined(NRF_DRV_UART_WITH_UARTE) && defined(NRF_DRV_UART_WITH_UART)
//    bool                use_easy_dma;
//#endif
//} nrf_drv_uart_config_t;

typedef struct
{
    volatile uint32_t	  pselTxd;
    volatile uint32_t	  pselRxd;
    uint32_t		  pselCts;
    uint32_t		  pselRts;
    UartBaudrate_t		  baudRate;
    UartParity_t		  parity;
    UartHwFlow_t		  hwFlowCtl;	/**< Flow control setting, if flow control is used, the system will use low power UART mode, based on CTS signal. */
    uint8_t		  interruptPriority;
    uint32_t		  interruptFlags;
} UartConfig_t;

class Uart
{
    UartBuffers_t	    *pUartBufferInfo;
    UartCommParams_t    *pUartCommParams;
    NRF_UART_Type	    *pUARTx;
    UartConfig_t	     uartConfig;
    typedef void	     (uartCallback(void));

    //std::function<void()> callback;

    // add a callback -- todo
    
    template<typename T>
    void setConfigParams(T& member, T value)
    {
        member = value;
    }
    
    template<typename T>
    void setNrfEvent(T reg, uint32_t value) // todo - change fctn name
    {
        *((volatile uint32_t *)((uint8_t *)pUARTx + (uint32_t)reg)) = value;

        #if __CORTEX_M == 0x04
        volatile uint32_t dummy = *((volatile uint32_t *)((uint8_t *)reg + (uint32_t)reg));
        (void)dummy;
        #endif
    }

    void enableInterrupt(const uint8_t irqPriority);
    void clearInterrupts(uint8_t masks);
    void clearNrfEvent(nrf_uart_event_t reg, uint32_t value);
    void enableUart();

    public:
    Uart(const UartCommParams_t *commParams, NRF_UART_Type *pUARTx, const uint8_t irqPriority);
    void UartInit();

};


#endif
