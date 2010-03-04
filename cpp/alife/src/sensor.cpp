#include "sensor.h"
#include "grid.h"
#include "world.h"
#include "mobile.h"


void Sensor::update(Mobile& mob, World& w, ftype dt)
{
    //Updateing absolute position
    absolutePos = mob.getPos() + mob.getRotation().apply( getPos());
    absoluteRotation = mob.getRotation()+getRotation();
    
    value = w.calcFoodFunction( 
	mob.getPos() + 	absoluteRotation.apply( vec2(radius, 0)), //sensibility area is a circle before the sensor.
	radius,
	*this );
}
ftype Sensor:: getValue()const
{
    return value;
}
ftype Sensor::operator()(Food& food)const
{
    //TODO: ineffective computation

    //calculate sensor response to a food item
    //food item in the sensor's local coordinate system
    vec2 dz = absoluteRotation.apply_back( food.getPos() - absolutePos );
    //calculate responce
    
    ftype r2 = dz.norm2(); //distance to the item, squared

    //calculate the sensibility function
    ftype distResp = r2>sqr(innerRadius)? sqr(innerRadius)/r2 : 1;
    ftype angleResp = r2>1e-3? dz.y/sqrt(r2) : 1; // sin(alpha)
    
    return sensibility * distResp * angleResp;	   
}

void Sensor::setParameters( ftype _sensibility, ftype _radius)
{
    sensibility = _sensibility;
    radius = _radius;
    innerRadius = _radius * 0.05;
}
