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
bool Mobile::simulate()
{
    assert( world );

    float dt = world->getSimulationStep();

    simulateMotors();
    simulateBrain();

    //SImulate movement
    position += speed * dt;
    angle += rotation( angleSpeed * dt );//TODO: use fast rotation?
    //Apply limitations, the World has on bot position
    world->applyBounds( *this );

    //simulate friction
    //1) Movement liquid friction

    speed *= max( float(0), (1 - speed.len() * world->getFriction() * dt / mass) ); //do not allow friction to reverse the speed

    //2) Rotation liquid friction
    float rotFrictionForce = sqr(angleSpeed) * world->getRotFriction();
    angleSpeed -= signum( angleSpeed ) * rotFrictionForce * dt / inertionMoment;

    //Mobile is a cell dweller - update reference to the inhabited cell
    //updateCell();

    return true;//TODO: energy > 0
}

void Mobile::simulateMotors()
{
    //TODO
}

void Mobile::simulateBrain()
{
    //TODO
}
   
void Mobile::setWorld( World * w )
{
    world = w;
}

Mobile::~Mobile()
{
    assert( ! getOwnerCell() );//prevenmt attempt of deleting used bot.
    assert( ! world );//check that the bot is removed from the world.
}
