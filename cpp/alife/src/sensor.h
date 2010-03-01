#pragma once
#ifndef __SENSOR_INCLUDED__
#define __SENSOR_INCLUDED__

#include "oriented.h"

class Sensor:public Oriented
{
private:
    ftype value;
public:
    Sensor():value(0){};
    Sensor(const vec2& pos, ftype angle): Oriented(pos,angle), value(0){};
    
    void update( World& w, ftype dt);
    ftype getValue()const;
};

#endif
