#include "sensor.h"
#include "grid.h"
#include "world.h"



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
    
    ftype r2 = dz.norm2();
    ftype alpha = 
}
