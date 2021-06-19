#ifndef __NOTIFICATION_MGR_H
#define __NOTIFICATION_MGR_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "stdint.h"
#include "Observer.hpp"
#include "mcp9808.hpp"
#include <stdbool.h>
#include <stdarg.h>

enum class Category
{
    tempReading
};

struct Notification
{
    uint8_t idx;
    char msg[20];
    Category category;
};

class NotificationManager : public Observer
{
    MCP9808 &_subject;		
    //Subject *subscriptionHead;

    public:
    NotificationManager(MCP9808 &tmpSensor);
    Notification notification;

    void push(Notification &notification);
    void update(Subject *subject);
    Notification const &getPrev(Notification &notification);
    void unsubscribe();

};

#endif
