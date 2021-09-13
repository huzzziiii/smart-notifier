#ifndef __BLE_H
#define __BLE_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "ble_conn_params.h"
#include "TimerApp.hpp"
#include "stdint.h"
//#include "SystemTask.hpp"
#include "FreeRTOS.h"
#include "timers.h"
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>



#define OPCODE_LENGTH        1
#define HANDLE_LENGTH        2

void ble_stack_init();
void gap_params_init();
void gatt_init();
void services_init();
void advertising_init();
void conn_params_init();
void advertising_start();

#endif
