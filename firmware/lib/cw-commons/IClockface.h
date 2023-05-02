#pragma once

#include "CWDateTime.h"

class IClockface {

    //virtual void setup(DateTime *dateTime) = 0;
    virtual void setup(CWDateTime *dateTime, String symbol1, String symbol2, String symbol3, String symbol4, String symbol5) = 0;
    virtual void update() = 0;

};
