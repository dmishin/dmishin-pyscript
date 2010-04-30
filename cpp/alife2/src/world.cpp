#include "world.hpp"
#include "mobile.hpp"


using namespace alife2;
void World::initParameters()
{
    //initialize world parameters
    timeStep = 0.001f;
    friction = 0.1f;
    rotationFriction = 0.1f;
}

World::World( float width, float height, float cellSize)
{
    initParameters();
    //initialize grids
    initGrids( width, height, cellSize );
}

void World::initGrids( float width, float height, float cellSize )
{
    bounds = rectangle( 0, 0, width, height );
    gridMobiles.initGrid( width, height, cellSize );
    gridFood.initGrid( width, height, cellSize );
}
	//Implementation of the Simulated
void World::simulate()
{
}

void World::add( Mobile * mobile )
{
    gridMobiles.putItem( mobile );
    mobile->setWorld( this );
}
/*
void World::add( Food* food )
{
    gridFood.putItem( food );
}
*/
 //Apply world geometry to the mobile: force it to stay inside the world bounds
void World::applyBounds( Mobile & mob )
{
    //For now, only force mobile to stay within the rectangular area of the World.
    //In future, add some obstacles?
    const vec2 & pos = mob.getLocation();
    if ( bounds.contains( pos ) ){
	//Bot is within the world rectangle
	return;
    }else{
	//Bot is outside the world rectangle
	//force it back
	vec2 pos1 = pos;
	vec2 speed1 = mob.getSpeed();

	if ( pos.x < bounds.left() ){
	    pos1.x  = bounds.left();
	    speed1.x = 0;
	}else if ( pos.x > bounds.right() ) {
	    pos1.x = bounds.right();
	    speed1.x = 0;
	}
	if ( pos.y < bounds.top() ){
	    pos1.y  = bounds.top();
	    speed1.y = 0;
	}else if ( pos.y > bounds.bottom() ) {
	    pos1.y = bounds.bottom();
	    speed1.y = 0;
	}
	//update the speed and position
	mob.setLocation( pos1 );
	mob.setSpeed( speed1 );
    }
}
