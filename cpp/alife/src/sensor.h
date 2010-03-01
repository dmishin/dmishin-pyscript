#pragma once
#ifndef __SENSOR_INCLUDED__
#define __SENSOR_INCLUDED__

#include "oriented.h"
class Food;
class Sensor:public Oriented
{
private:
    ftype value;
    vec2 absolutePos;//precalculated sensor position in absolute coordinates
    rotation absoluteRotation;

    //sensor parameters
    ftype innerRadius;//below this radius, responce does not grows
    ftype radius;//maximal radius
    ftype respo

    ftype sensibility;//general sensibility koefficien
public:
    Sensor():value(0){};
    Sensor(const vec2& pos, ftype angle): Oriented(pos,angle), value(0){};
    
    //sensor paramters
    void setShape

    void update(Mobile&mob, World& w, ftype dt);
    ftype getValue()const;

    ftype operator(Food& food)const;
};

#endif
