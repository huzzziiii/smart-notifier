#include "Subject.hpp"

//int Subject::GetObserverIdx(Observer *obs)
//{
//    for (uint8_t idx = 0; idx < observerMaxSize; idx++)
//    {
//        if (observerList[idx] == obs)
//        {
//	  return idx;
//        }	
//    }
//    return -1;
//}

/*
@brief: attach an observer to the subject for notifications i.e subscribing
*/
void Subject::attach(Observer *obs)
{
    if (headIdx >= observerMaxSize)
    {	  
        // TODO - add a print!
        return;
    }
    //observerList[headIdx++] = obs;
    _notificationSubscriber = obs;
    
    bool resumeThread = true;
    onSubscriberChange(resumeThread);

    //if (!resumeThread)
    //{
    //    onSubscriberChange(!resumeThread);
    //}
}

void Subject::detach(Observer *observerToDetach)	      // TODO -- remove the parameter
{
    bool resumeThread = false;
    _notificationSubscriber = nullptr;
    onSubscriberChange(resumeThread);
}

void Subject::notify(Subject *subject) // TODO - remove params
{
    if (_notificationSubscriber == nullptr)
    {   
        NRF_LOG_WARNING("No subscribers to notify...\n");
        // TODO - add a relevant comment on how there's no subcriber
        return;
    }
    _notificationSubscriber->update(subject);
}

bool Subject::ObserverSubscribed() const
{
    return _notificationSubscriber;
}