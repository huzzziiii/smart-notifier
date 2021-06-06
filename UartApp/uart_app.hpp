#ifndef __UART_APP_H
#define __UART_APP_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "stdint.h"
#include "fifo.hpp"
#include "system_task.hpp"
#include <stdbool.h>
#include <stdarg.h>



void uartCallback(Fifo &pFifo, QueueHandle_t &systemTaskQueue);


#endif
