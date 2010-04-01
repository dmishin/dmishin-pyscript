/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * alife-cpp
 * Copyright (C)  2010 <>
 * 
 * alife-cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * alife-cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "world.h"
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>

MobilePtr World::findNearestMobile( const vec2& p, ftype maxDist)
{
	
    return boost::static_pointer_cast<Mobile>( gridFood.findNearestItem( p, maxDist ) );
}
FoodPtr World::findNearestFood( const vec2& p, ftype maxDist)
{
    return boost::static_pointer_cast<Food>( gridFood.findNearestItem( p, maxDist ) );
}

World::World( vec2 _size, ftype cellSize )
    :mobilesGridUpdateTicker( 0.5 ) //update mobiles grid periodically
    ,breedersIdleTicker( 1.0 ) //update breeders
{
	size = _size;
	//init grids
	int numCols = static_cast<int>(size.x/cellSize);
	int numRows = static_cast<int>(size.y/cellSize);

	gridMobiles.setGeometry( numCols, numRows, size.x, size.y);
	gridFood.setGeometry( numCols, numRows, size.x, size.y);

	//set default world parameters
	viskosity = 15.0;
	energyConsumptionRate = 0.0003;
	idleEnergyConsumptionRate = 0.0001;

	time = 0;
}

//get all bots inside rectangle
void World::getMobilesSnapshot( const vec2& ptTopLeft, const vec2& ptBottomRight, World::MobilesSnapshot&buffer)const
{
    boost::lock_guard<boost::mutex> guard( mutex );
//TODO: get read access to the grid
    buffer.resize(0);
    Grid::rectangle_generator gen( const_cast<Grid&>(gridMobiles), ptTopLeft, ptBottomRight);
    for(GridItemPtr loc; gen( loc );){
	buffer.push_back( boost::static_pointer_cast<Mobile>( loc ) );
    }
}
void World::getFoodSnapshot( const vec2& ptTopLeft, const vec2& ptBottomRight, World::FoodSnapshot& buffer)const
{
    boost::lock_guard<boost::mutex> guard( mutex );
//TODO: get read access to the grid
    buffer.resize(0);
    Grid::rectangle_generator gen( const_cast<Grid&>(gridFood), ptTopLeft, ptBottomRight);
    for(GridItemPtr loc; gen( loc );){
	buffer.push_back( static_cast<Food&>( *loc ) );
    }
}

void World::addMobile( MobilePtr mob )
{
    mob->setWorld( *this );
    {
	boost::lock_guard<boost::mutex> guard( mutex );
	gridMobiles.putItem( mob );
    }
	
    if (simulator){//report the bot to the simulator
	simulator->onNewBot( mob );
    }
}
void World::addFood( FoodPtr f )
{
    gridFood.putItem( f );
}

void World::reportDeadBot( Mobile& mob )
{
	//Called from the dead bot.
	//TODO: remove bot from the grid and from the list
}
void World::foodEaten( FoodPtr food, Mobile &mob) //called by mobile, when it eats one food item
{
    gridFood.removeItem( food );
}

void World::setSimulator( boost::shared_ptr<AbstractSimulator> _simulator)
{
    simulator = _simulator;
    if (simulator){
	simulator->prepareSimulation( *this );
	Grid::items_generator gItems( gridMobiles );
	for( GridItemPtr item; gItems( item );){
	    MobilePtr pMob = boost::static_pointer_cast<Mobile>( item );
	    simulator->onNewBot( pMob );
	}
    }
}

struct isBotDead{
    bool operator()( GridItemPtr pBot ){
	MobilePtr p = boost::static_pointer_cast<Mobile>( pBot );
	return ! p->isAlive();
    }
};

/**Must be called by simulator to signal that grids shoudl be updated*/
void World::updateGrids(bool updateMobiles, bool updateFood)
{
    boost::lock_guard<boost::mutex> guard( mutex );
    if(updateMobiles){
	gridMobiles.update();
	//filter out dead mobiles
	gridMobiles.remove_if( isBotDead() );
    }
    if( updateFood )//usually false
		gridFood.update();
}

/**Simulate global parameters*/
void World::simulate( ftype dt )
{
    time += dt;
    if (mobilesGridUpdateTicker.step( dt )){
	updateGrids(true/*mobiles*/, false/*food*/);
    }
    if (breedersIdleTicker.step( dt )){
	for(Breeders::iterator i = breeders.begin(); i!=breeders.end(); ++i){
	    (*i)->onIdle( *this );
	}
    }
}

void World::addBreeder( Breeder *pb )
{
    assert (pb );
    breeders.push_back( pb );
}
bool World::removeBreeder( Breeder * pb )
{
    Breeders::iterator i = std::find( breeders.begin(), breeders.end(), pb );
    if (i != breeders.end()){
	breeders.erase( i );
	return true;
    }else{
	return false;
    }
}
