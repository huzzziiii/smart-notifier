#include "Subject.hpp"


void Subject::attach(Observer *obs)
{
    if (headIdx >= observerMaxSize)
    {	  
        // TODO - add a print!
        return;
    }
    notificationSubscriber = obs;
}

void Subject::detach()
{
    notificationSubscriber = nullptr;
}

void Subject::notify(Subject *subject) // TODO - remove params
{
    if (notificationSubscriber == nullptr)
    {   
        // TODO - add a relevant comment on how there's no subcriber
        return;
    }
    notificationSubscriber->update(subject);
}
