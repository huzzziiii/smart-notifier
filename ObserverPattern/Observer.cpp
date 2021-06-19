#ifndef __OBSERVER_H
#define __OBSERVER_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "stdint.h"
#include "Subject.hpp"
#include <stdbool.h>
#include <stdarg.h>

Subject subject;

class Observer 
{
    virtual ~Observer() = default;
    virtual void update(Subject&) = 0;

    

};

#endif
