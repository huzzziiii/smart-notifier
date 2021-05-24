#include "uart.hpp"
extern "C" {
    void UARTE0_UART0_IRQHandler(void)
    {
        int ba = 5;
        int bol = 1;

        // steps

        /* 1. read from the RXD 
	   - RXDRDY event is generated indicating byte is received
	   - read from RXD
	2.  Clear RXDRDY event prior to reading off RXD
        */

    }
}

void uartCallbackHandler(void)
{


}

//void nrfx_uart_0_irq_handler(void)
//{
//     NRF_LOG_INFO("UARTE0_UART0_IRQHandler\n");
//     int a;
//}

Uart::Uart(const UartCommParams_t *commParams, NRF_UART_Type *pUARTx, const uint8_t irqPriority)
{
    // init comm params
    uartConfig.interruptPriority = irqPriority;
    uartConfig.baudRate = commParams->baudRate;
    uartConfig.pselCts = commParams->ctsPinNo;
    uartConfig.pselRts = commParams->rtsPinNo;
    uartConfig.pselRxd = commParams->rxPinNo;
    uartConfig.pselTxd = commParams->txPinNo;
    uartConfig.hwFlowCtl = commParams->hwFlowCntl;
    uartConfig.interruptFlags = NRF_UART_INT_MASK_TXDRDY | NRF_UART_INT_MASK_RXDRDY | NRF_UART_INT_MASK_RXTO;
    this->pUARTx = pUARTx;

    UartInit();

    enableInterrupt(uartConfig.interruptPriority);
    enableUart();

    setNrfEvent(NRF_UART_TASK_STARTRX, StatusEnable); //Trigger StartRX
}

void Uart::UartInit()
{
     nrf_gpio_pin_set(uartConfig.pselTxd);

    if (uartConfig.pselTxd != UartPselDisconnected)
    {
        nrf_gpio_pin_set(uartConfig.pselTxd);
        nrf_gpio_cfg_output(uartConfig.pselTxd);
    }

    if (uartConfig.pselRxd != UartPselDisconnected)
    {
        nrf_gpio_cfg_input(uartConfig.pselRxd, NRF_GPIO_PIN_NOPULL);
    }

    //NRF_UART_BAUDRATE_115200
    //nrf_uart_baudrate_set(pUSARTx, uartConfig.baudRate);
    //setBaudRate(uartConfig.baudRate);
    
    setConfigParams<volatile uint32_t> (pUARTx->BAUDRATE, uartConfig.baudRate);	// set baud rate
    setConfigParams<volatile uint32_t> (pUARTx->CONFIG, uartConfig.parity);	// set parity
    setConfigParams<volatile uint32_t> (pUARTx->CONFIG, uartConfig.hwFlowCtl);	// set hw flow control

    //pUARTx->CONFIG = (uint32_t)uartConfig.parity | (uint32_t)uartConfig.hwFlowCtl;
    //nrf_uart_configure(pUSARTx, uartConfig.parity, uartConfig.hwFlowCtl);
    
    setConfigParams<volatile uint32_t> (pUARTx->PSEL.RXD, uartConfig.pselRxd);
    setConfigParams<volatile uint32_t> (pUARTx->PSEL.TXD, uartConfig.pselTxd);

    //pUARTx->PSEL.RXD = uartConfig.pselRxd;
    //pUARTx->PSEL.TXD = uartConfig.pselTxd;

    //nrf_uart_txrx_pins_set(pUSARTx, uartConfig.pselTxd, uartConfig.pselRxd);
   
    if (uartConfig.hwFlowCtl == UartHwFcEnabled)
    {
        if (uartConfig.pselCts != UartPselDisconnected)
        {
	  nrf_gpio_cfg_input(uartConfig.pselCts, NRF_GPIO_PIN_NOPULL);
        }
        if (uartConfig.pselRts != UartPselDisconnected)
        {
	  nrf_gpio_pin_set(uartConfig.pselRts);
	  nrf_gpio_cfg_output(uartConfig.pselRts);
        }
        pUARTx->PSEL.RTS = uartConfig.pselRts; 
        pUARTx->PSEL.CTS = uartConfig.pselCts;
        //nrf_uart_hwfc_pins_set(pUSARTx, uartConfig.pselRts, uartConfig.pselCts);
    }
}

////#if NRFX_CHECK(NRFX_UART0_ENABLED)
//void nrfx_uart_0_irq_handler(void)
//{   
//    NRF_LOG_INFO("UART_IRQ_HANDLER()\n");
//    //uart_irq_handler(NRF_UART0, &m_cb[NRFX_UART0_INST_IDX]);
//}
//#endif


//__STATIC_INLINE void nrf_uart_event_clear(NRF_UART_Type * p_reg, nrf_uart_event_t event)
//{
//    *((volatile uint32_t *)((uint8_t *)p_reg + (uint32_t)event)) = 0x0UL;
//#if __CORTEX_M == 0x04
//    volatile uint32_t dummy = *((volatile uint32_t *)((uint8_t *)p_reg + (uint32_t)event));
//    (void)dummy;
//#endif
//}



void Uart::enableInterrupt(const uint8_t irqPriority)
{
    // clear events 
    setNrfEvent<nrf_uart_event_t> (NRF_UART_EVENT_RXDRDY, StatusDisable);
    setNrfEvent<nrf_uart_event_t> (NRF_UART_EVENT_RXTO, StatusDisable);
    setNrfEvent<nrf_uart_event_t> (NRF_UART_EVENT_TXDRDY, StatusDisable);

    //nrf_uart_int_enable(p_instance->p_reg, uartConfig.interruptFlags);
    
    // enable respective interrupt(s)
    pUARTx->INTENSET |= uartConfig.interruptFlags;
    
    //NRFX_IRQ_PRIORITY_SET(nrfx_get_irq_number((void *)p_instance->p_reg),
    //                      interrupt_priority);
    NRFX_IRQ_ENABLE(nrfx_get_irq_number((void *)pUARTx));

    // 
    //static nrfx_irq_handler_t const irq_handlers[1] = 
    //{
    //    nrfx_uarte_0_irq_handler
    //};

//    if (nrfx_prs_acquire(pUARTx, irq_handlers[0]) != NRFX_SUCCESS)
//    {
//        err_code = NRFX_ERROR_BUSY;
//        NRFX_LOG_WARNING("Function: %s, error code: %s.",
//                         __func__,
//                         NRFX_LOG_ERROR_STRING_GET(err_code));
//    }
}

void Uart::clearInterrupts(uint8_t masks)
{
    //pUARTx->INTENCLR |= 
    pUARTx->INTENCLR = 1 << 2;
    //setNrfRegister(pUARTx->INTENCLR, masks, value, 1); // todo - change 1 to macro/enum

}

void Uart::enableUart()
{
    pUARTx->ENABLE = UART_ENABLE_ENABLE_Enabled;
}

//void Uart::enableRx()
//{

//}

//__STATIC_INLINE void nrf_uarte_task_trigger(NRF_UARTE_Type * p_reg, nrf_uarte_task_t task)
//{
//    *((volatile uint32_t *)((uint8_t *)p_reg + (uint32_t)task)) = 0x1UL;
//}

//void Uart::setBaudRate(UartBaudrate_t baudRate)
//{
//    pUARTx->BAUDRATE = baudRate;
//}



