#include "NotificationManager.hpp"

NotificationManager::NotificationManager(MCP9808 &tmpSensor) : _subject(tmpSensor) 
{
    _subject.attach(this);	      // subscribing an observer by default

    // TODO - subscribe to dynamic subjects...
}

void NotificationManager::push(Notification &notification)
{

}

void NotificationManager::unsubscribe()
{
    _subject.detach(this);
}

/*
@NotificationManager: receives notifications from all the subscribers
TODO - must use protection guard for member variables
*/
void NotificationManager::update(Subject *subject)
{
    int a = 5; // TODO - remove
    int h;
    if (subject == &_subject)
    {
        a++;
        h = 5;

        // display to the output
    }
    h++;
}