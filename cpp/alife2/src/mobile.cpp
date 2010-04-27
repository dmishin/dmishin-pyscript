#include "mobile.hpp"
#include "world.hpp"
#include <assert.h>
#include <math.h>

using namespace alife2;
Mobile::Mobile()
    :GridItem( vec2(0,0) ),
     angle( 0 ),
     speed( 0, 0),
     angleSpeed( 0 )
{
    initParameters();
}
Mobile::Mobile( const vec2 &pos, float angle_ )
    :GridItem( pos ),
     angle( angle_ ),
     angleSpeed( 0 ),
     speed( 0, 0 )
{
    initParameters();
}
void Mobile::initParameters()
{
    //Initialize various mobile parameters
    mass = 1.0f;
    inertionMoment = 1.0f;
}
/**Implementation of the SImulated*/
void Mobile::simulate()
{
    assert( world );
    float dt = world->getSimulationStep();//TODO: get the time step from the world
    //SImulate movement
    position += speed * dt;
    angle += rotation( angleSpeed * dt );//TODO: use fast rotation?
    
    //simulate friction
    //1) Movement liquid friction
    float mvFrictionForce = speed.len2() * world->getFriction(); //TODO: not very optimal calculation
    speed -= speed/speed.len() * mvFrictionForce * dt / mass;
    //2) Rotation liquid friction
    float rotFrictionForce = sqr(angleSpeed) * world->getRotFriction();
    angleSpeed -= signum( angleSpeed ) * rotFrictionForce * dt / inertionMoment;
}

void Mobile::setWorld( World * w )
{
    world = w;
}

Mobile::~Mobile()
{
}
