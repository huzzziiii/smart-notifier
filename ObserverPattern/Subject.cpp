#include "Subject.hpp"


void Subject::attach(Observer *obs)
{
    if (headIdx >= observerMaxSize)
    {	  
        // TODO - add a print!
        return;
    }
    observerList[headIdx++] = obs;	// TODO - rmv

    //if (firstSubscription == nullptr)	    // first subscriber - store the head
    //{
    //    firstSubscriber = obs;
    //}
}

void Subject::detach(Observer *obs)
{
    if (headIdx == 0)
    {
        // TODO - add a print!
        return;	
    }
    headIdx--;
}

void Subject::notify(Subject *subject) // TODO - remove params
{
    for (uint8_t idx = tailIdx; idx < headIdx; idx++)
    {
        observerList[idx]->update(subject);
    }
}
