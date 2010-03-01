#include "sensor.h"

ftype sensibility(ftype r, ftype angle, ftype d0, ftype dphi)
{
    ftype kd = 1/(sqr(r/d0)*3+1);
    ftype kr = 1;
    return kd*kr;
}

void Sensor::update( World& w, ftype dt)
{
    
}
ftype Sensor:: getValue()const
{
    
}
