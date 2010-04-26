#include "mobile.hpp"
#include "world.hpp"
#include <assert.h>

using namespace alife2;
Mobile::Mobile()
    :GridItem( vec2(0,0) ),
     angle( 0 ),
     speed( 0, 0),
     angleSpeed( 0 )
{
}
Mobile::Mobile( const vec2 &pos, float angle_ )
    :GridItem( pos ),
     angle( angle_ ),
     angleSpeed( 0 ),
     speed( 0, 0 )
{
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
    
}

void Mobile::setWorld( World * w )
{
    world = w;
}

Mobile::~Mobile()
{
}
