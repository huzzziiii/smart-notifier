#ifndef __NOTIFICATION_MGR_H
#define __NOTIFICATION_MGR_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "stdint.h"
#include "Observer.hpp"
#include "mcp9808.hpp"
#include "fifo.hpp"
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
    char msg[40];
    Category category;
};

class NotificationManager : public Observer
{
    Subject *subscriptions[maxSubscriptions] = {nullptr};	// store references to Subject
    uint8_t subscriptionIdx = 0;

    Notification _notifications[40];			// TODO - think about using Fifo instead...
    //template<typename T>?
    //Fifo _notifications;
    public:
    //NotificationManager(MCP9808 &tmpSensor);

    template<typename ...Args>
    NotificationManager(Args ...args)
    {
        ((subscriptions[subscriptionIdx] = args), ...);
        subscriptions[subscriptionIdx++]->attach(this);
    }
  
    void push();
    void update(Subject *subject);
    Notification const &getPrev(Notification &notification);

    void subscribe(Subject *subscription);
    void unsubscribe(Subject *subscription);

    Notification _notification;
};

#endif
