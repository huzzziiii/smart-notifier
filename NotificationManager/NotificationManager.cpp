#include "NotificationManager.hpp"

void NotificationManager::printNotificationList()
{
    uint8_t size = _notifications.GetFifoSize(); 

    //Notification ntfx = _notifications.GetFiFoAt(0);
    //NRF_LOG_INFO("Idx: %d, message: %s\n", ntfx.idx, ntfx.msg);
    //NRF_LOG_FLUSH();

    for (uint8_t idx = 0; idx < size; idx++)
    {
        Notification ntfx = _notifications.GetFiFoAt(idx);
        NRF_LOG_INFO("[%d] NtfIdx: %d, message: %s\n", idx, ntfx.idx, ntfx.msg);
        //NRF_LOG_FLUSH();
    }
}

void NotificationManager::unsubscribe(Subject *subscription)
{
    for (uint8_t idx = 0; idx < maxSubscriptions; idx++) // TODO - might wanna get rid of the loop since there's only ine subscription per Subject class
    {
        if (subscriptions[idx] == subscription)
        {
	  subscriptions[idx]->detach(this);
	  return;
        }
    }
}

void NotificationManager::subscribe(Subject *subscription)
{
    subscription->attach(this);
}

void NotificationManager::pushNotification()
{
    _notifications.enque(_notification);

    /* TODO 
    - send notification over to BLE and terminal
    */
}

/*
@update: receives notifications from all the subscribers (currently only one so no need for a loop!)
[TODO] -- Since this function is invoked from all the subscriptions. Protection guard would be required to protect the data...
*/
void NotificationManager::update(Subject *subject)
{
    int a = 5; // TODO - remove
    int h;

    NRF_LOG_WARNING("[NotificationManager::update] -- %d\n", subject == subscriptions[0]);  // TODO remove
    //NRF_LOG_FLUSH();
    
   
    if (subject == subscriptions[0])	    // tmpSensor
    {
        MCP9808 *mcp9808 = dynamic_cast<MCP9808*>(subject);
        uint16_t tmpValue = mcp9808->getCurrentTempInC();
        _notification.category = Category::tempReading;
        snprintf(_notification.msg, 50, "Current temperature value (C) = %u", tmpValue);

        //NRF_LOG_INFO("__NotifMsg: %s\n", _notification.msg);

        pushNotification();
         
        //printNotificationList();

        // TODO - display to the output source
    }
    h++;	      // TODO - remove
}