#include "uart.hpp"

static Uart *pInstance;

extern "C" {
    void UARTE0_UART0_IRQHandler(void)
    {   
        pInstance->irqHandler();
    }
}

void Uart::irqHandler()
{
    // NrfLogger::writeToLogger<char>("UART IRQ Handler()\n"); 
    //NRF_UART_Type* uartInstance = getUartInstance();
    //Fifo *pFifoRx = pInstance->getFifoRx();

    bool isRxdRdyIrqSet	   = getIrqRegStatus(NRF_UART_INT_MASK_RXDRDY);
    bool isRxdRdyEvntSet	   = getNrfEventStatus(NRF_UART_EVENT_RXDRDY);

    bool isIrqMaskErrorSet	   = getIrqRegStatus(NRF_UART_INT_MASK_ERROR);
    bool isEventMaskErrorSet	   = getNrfEventStatus(NRF_UART_EVENT_ERROR);

    // TODO - fill TX...

    if (isRxdRdyIrqSet && isRxdRdyEvntSet)
    {
        if (newInput)
        {
	  fifoRx.resetStartIdx();				// set the starting index to parse the incoming data
	  newInput = false;
        }

        setNrfEvent(NRF_UART_EVENT_RXDRDY, StatusDisable);		// clear NRF_UART_EVENT_RXDRDY
        uint32_t data = pUARTx->RXD;				// read off RXD
        fifoRx.enque(data);					// append to FifoRx
        
        if (data == '\r')					// if end of data...
        {
	  newInput = true;
	  uartCallback(fifoRx, systemTaskQueue);	          // invoke callback
        }
    }
    else if (isIrqMaskErrorSet && isEventMaskErrorSet)
    {
        setNrfEvent(NRF_UART_EVENT_ERROR, StatusDisable);		// clear NRF_UART_EVENT_RXDRDY
        clearInterrupt(NRF_UART_INT_MASK_ERROR);
        setNrfEvent(NRF_UART_TASK_STOPRX, StatusEnable);		// trigger STOPRX
    }
    

    /* 1. read from the RXD 
         - RXDRDY event is generated indicating byte is received
         - read from RXD
      2.  Clear RXDRDY event prior to reading off RXD
    */
}

bool Uart::getNrfEventStatus(nrf_uart_event_t reg) const
{
    return (bool) *(volatile uint32_t *)((uint8_t *)pUARTx + (uint32_t)reg);	
}

bool Uart::getIrqRegStatus(uint32_t mask)
{
    return (bool) (pUARTx->INTENSET & mask);
}

Uart::Uart(const UartCommParams_t *commParams, NRF_UART_Type *uartInstance, const uint8_t irqPriority, void (*callback)(Fifo &pFifo, QueueHandle_t &systemTaskQueue),
	 QueueHandle_t &queue) 
	  : systemTaskQueue(queue)
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
    pUARTx = uartInstance;
    pInstance = this;
    uartCallback = callback;

    uartInit();

    enableInterrupt(uartConfig.interruptPriority);
    enableUart();

    setNrfEvent(NRF_UART_TASK_STARTRX, StatusEnable);	//Trigger StartRX
}

void Uart::uartInit()
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
    
    setConfigParams<volatile uint32_t> (pUARTx->BAUDRATE, uartConfig.baudRate);	// set baud rate
    setConfigParams<volatile uint32_t> (pUARTx->CONFIG, uartConfig.parity);	// set parity
    setConfigParams<volatile uint32_t> (pUARTx->CONFIG, uartConfig.hwFlowCtl);	// set hw flow control
    
    setConfigParams<volatile uint32_t> (pUARTx->PSEL.RXD, uartConfig.pselRxd);
    setConfigParams<volatile uint32_t> (pUARTx->PSEL.TXD, uartConfig.pselTxd);
   
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

void Uart::enableInterrupt(const uint8_t irqPriority)
{
    // clear events 
    setNrfEvent<nrf_uart_event_t> (NRF_UART_EVENT_RXDRDY, StatusDisable);
    setNrfEvent<nrf_uart_event_t> (NRF_UART_EVENT_RXTO, StatusDisable);
    setNrfEvent<nrf_uart_event_t> (NRF_UART_EVENT_TXDRDY, StatusDisable);
    
    // enable respective interrupt(s)
    pUARTx->INTENSET |= uartConfig.interruptFlags;
    
    //NRFX_IRQ_PRIORITY_SET(nrfx_get_irq_number((void *)p_instance->p_reg),
    //                      interrupt_priority);
    NRFX_IRQ_ENABLE(nrfx_get_irq_number((void *)pUARTx));
}

void Uart::clearInterrupt(uint32_t mask)
{
    pUARTx->INTENCLR |= mask;
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



