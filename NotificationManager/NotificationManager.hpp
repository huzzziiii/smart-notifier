#ifndef __NOTIFICATION_MGR_H
#define __NOTIFICATION_MGR_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "stdint.h"
#include "Observer.hpp"
#include "mcp9808.hpp"
#include "fifo.hpp"
#include "uart.hpp"
#include "custom_service.h"

#include <stdbool.h>
#include <stdarg.h>

uint8_t constexpr maxSubscriptions = 5;

enum class Category
{
    tempReading
};

struct Notification
{
    uint8_t idx = 0;
    char msg[50];
    Category category;
};

class BleCustomService;
 
//template<typename T>
class NotificationManager : public Observer
{
    Subject *subscriptions[maxSubscriptions] = {nullptr};	// store references to Subject
    uint8_t subscriptionIdx = 0;
    Uart _uart;
    BleCustomService &_bleCustSrv;
    Fifo<Notification> _notifications;

    public:
    //NotificationManager(MCP9808 &tmpSensor);
    
    // constructor
    template<typename ...Args>
    NotificationManager(BleCustomService &bleCustSrv, Uart &uart, Args ...args) : _bleCustSrv(bleCustSrv), _uart(uart), _notification{0}
    {
        ((subscriptions[subscriptionIdx] = args), ...);
        subscriptions[subscriptionIdx++]->attach(this);
    }

    //NotificationManager(Uart &uart, Args ...args) : _uart(uart), _notification{0}
    //{
    //    ((subscriptions[subscriptionIdx] = args), ...);
    //    subscriptions[subscriptionIdx++]->attach(this);
    //}
    
    void printNotificationList();
    void pushNotification();
    void update(Subject *subject);
    Notification const &getPrev(Notification &notification);

    void subscribe(Subject *subscription);
    void unsubscribe(Subject *subscription);

    Notification _notification;
};

#endif
