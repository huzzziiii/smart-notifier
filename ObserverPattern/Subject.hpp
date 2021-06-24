#ifndef __SUBJECT_H
#define __SUBJECT_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "stdint.h"
#include "Observer.hpp"
#include <stdbool.h>
#include <stdarg.h>

static uint8_t constexpr observerMaxSize = 3;

class Observer;

class Subject 
{   
    uint8_t headIdx = 0;	      // the max index till observers are populated in the buffer
    uint8_t tailIdx = 0;	

    //Subject *nextSubscription = nullptr;
    //Observer *firstSubscription = nullptr;
    Observer *notificationSubscriber = nullptr;

    public:
    virtual ~Subject() = default;
         
    Observer *observerList[observerMaxSize];  // TODO - really need an array just to store a ref to Notification class?
    void attach(Observer *obs);
    void detach();
    void notify(Subject*);
    void setSubscriptionHead(Subject *head);
};

#endif
