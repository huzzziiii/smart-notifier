#include "NotificationManager.hpp"

void NotificationManager::unsubscribe(Subject *subscription)
{
    for (uint8_t idx = 0; idx < maxSubscriptions; idx++)
    {
        if (subscriptions[idx] == subscription)
        {
	  subscriptions[idx]->detach();
	  return;
        }
    }
}

void NotificationManager::subscribe(Subject *subscription)
{
    subscription->attach(this);
}

void NotificationManager::push()
{
    //_notifications.enque(notification);
    _notifications[_notification.idx++] = _notification;
}

/*
@update: receives notifications from all the subscribers (currently only one so no need for a loop!)
[TODO] -- Since this function is invoked from all the subscriptions. Protection guard would be required to protect the data...
*/
void NotificationManager::update(Subject *subject)
{
    int a = 5; // TODO - remove
    int h;

    if (subject == subscriptions[0])	    // tmpSensor
    {
        MCP9808 *mcp9808 = dynamic_cast<MCP9808*>(subject);
        uint16_t tmpValue = mcp9808->getCurrentTempInC();
        _notification.category = Category::tempReading;
        snprintf(_notification.msg, 40, "Current temperature value (C) = %d\n", tmpValue);
        push(); 

        // TODO - display to the output source
    }
    h++;	      // TODO - remove
}