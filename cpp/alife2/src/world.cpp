#include "world.hpp"
#include "mobile.hpp"


using namespace alife2;
void World::initParameters()
{
    //initialize world parameters
    timeStep = 0.1f;
    friction = 0.1f;
    rotationFriction = 0.1f;
}

World::World()
{
    initParameters();
    //initialize grids
    initGrids( 100, 100, 1 );
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
}
/*
void World::add( Food* food )
{
    gridFood.putItem( food );
}
*/
