#include "sensor.h"
#include "grid.h"
#include "world.h"

ftype sensibility(ftype x, ftype y, ftype d0, ftype dphi)
{
    ftype kd = 1/(sqr(r/d0)*3+1);
    ftype kr = 1;
    return kd*kr;
}


void Sensor::update(Mobile& mob, World& w, ftype dt)
{
    //Updateing absolute position
    absolutePos = mob.getPos() + mob.getRotation().apply( getPos());
    absoluteRotation = mob.getRotation()+getRotation();
    
    value = w.calcFoodFunction( radius, functor);
}
ftype Sensor:: getValue()const
{
    return value;
}
ftype Sensor::operator(Food& food)const
{
    //calculate sensor response to a food item
    //food item in the sensor's local coordinate system
    vec2 dz = absoluteRotation.apply_back( food.getPos() - absoluteRotation );
    //calculate responce
    
    return sensibility(dz.x, dz.y, innerRadius, radius);
}
