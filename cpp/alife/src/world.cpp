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

MobilePtr World::findNearestMobile( const vec2& p, ftype maxDist)
{
	
	return boost::static_pointer_cast<Mobile>( gridFood.findNearestItem( p, maxDist ) );
}
FoodPtr World::findNearestFood( const vec2& p, ftype maxDist)
{
	return boost::static_pointer_cast<Food>( gridFood.findNearestItem( p, maxDist ) );
}

World::World( vec2 _size, ftype cellSize )
{
	size = _size;
	//init grids
	int numCols = static_cast<int>(size.x/cellSize);
	int numRows = static_cast<int>(size.y/cellSize);

	gridMobiles.setGeometry( numCols, numRows, size.x, size.y);
	gridFood.setGeometry( numCols, numRows, size.x, size.y);

	//TODO
//set default world parameters
	viskosity = 10.0;
	energyConsumptionRate = 0.01;
	
}

//get all bots inside rectangle
void World::getMobilesSnapshot( const vec2& ptTopLeft, const vec2& ptBottomRight, World::MobilesSnapshot&buffer)const
{
//TODO: get read access to the grid
    buffer.resize(0);
    Grid::rectangle_generator gen( const_cast<Grid&>(gridMobiles), ptTopLeft, ptBottomRight);
    for(GridItemPtr loc; gen( loc );){
	buffer.push_back( boost::static_pointer_cast<Mobile>( loc ) );
    }
}
void World::getFoodSnapshot( const vec2& ptTopLeft, const vec2& ptBottomRight, World::FoodSnapshot& buffer)const
{
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
    mobiles.push_back( mob );
    gridMobiles.putItem( mob );
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
