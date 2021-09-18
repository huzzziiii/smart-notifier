#ifndef __UART_APP_H
#define __UART_APP_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "stdint.h"
#include "fifo.hpp"
#include "SystemTask.hpp"
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include "BleUartService.hpp"

void uartCallback(Fifo<uint8_t> &pFifo, QueueHandle_t &systemTaskQueue);
//void bleUartHandler(BleUartEvent_t *pEventType);


#endif
