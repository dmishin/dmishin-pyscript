#pragma once
#ifndef __SENSOR_INCLUDED__
#define __SENSOR_INCLUDED__
#include "ftype.h"
#include "vec2.h"
#include "rotation.h"
#include "oriented.h"
class Food;
class Mobile;
class World;

class Sensor:public Oriented
{
private:
    ftype value;
    vec2 absolutePos;//precalculated sensor position in absolute coordinates
    rotation absoluteRotation;

    //sensor parameters
    ftype innerRadius;//below this radius, responce does not grows
    ftype radius;//maximal radius


    ftype sensibility;//general sensibility koefficien
public:
    Sensor():value(0){};
    Sensor(const vec2& pos, ftype angle): Oriented(pos,angle), value(0){};
    Sensor(const vec2& pos, ftype angle, ftype sensibility, ftype _radius): Oriented(pos,angle), value(0){
	setParameters( sensibility, _radius );
    };
    
    void setParameters(ftype sensibility, ftype _radius);
    //sensor paramters

    //Update the perceived value
    void update(Mobile&mob, World& w, ftype dt);
    //get the perceived value (recalculated only when update() called)
    ftype getValue()const;

    ftype operator()(Food& food)const;
};

#endif
