#ifndef __SENSORS_H
#define __SENSORS_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"

template <class T>
class Sensors 
{   
    union address {
        int _i2cAddr;
        int _spiAddr;
    };

    public:
    //virtual void init(Sensor *sensor) = 0;
    ////virtual void read() = 0;;
    ////virtual void write() = 0;

    //virtual T read(Sensor *sensor);


};

#endif
