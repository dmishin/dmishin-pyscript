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

Mobile * World::findNearestMobile( const vec2& p, ftype maxDist)
{
	Grid::circular_generator gen(gridMobiles, p, maxDist);
	Located* best = 0;
	ftype bestD = maxDist;
	for ( Located* mob=0; gen( mob ); ){
		ftype d = dist( mob->getPos(), p);
		if (d<= bestD){
			bestD = d;
			best = mob;
		}
	}
	return static_cast<Mobile*>(best);
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
	viskosity = 100.0;
	energyConsumptionRate = 0.01;
	
}

//get all bots inside rectangle
void World::getMobilesSnapshot( const vec2& ptTopLeft, const vec2& ptBottomRight, World::MobilesSnapshot&buffer)const
{
//TODO: get read access to the grid
    buffer.resize(0);
	Grid::rectangle_generator gen( const_cast<Grid&>(gridMobiles), ptTopLeft, ptBottomRight);
	for(Located* loc; gen( loc );){
		buffer.push_back( static_cast<Oriented&>( *loc ) );
	}
}
void World::getFoodSnapshot( const vec2& ptTopLeft, const vec2& ptBottomRight, World::FoodSnapshot& buffer)const
{
//TODO: get read access to the grid
    buffer.resize(0);
    Grid::rectangle_generator gen( const_cast<Grid&>(gridFood), ptTopLeft, ptBottomRight);
    for(Located* loc; gen( loc );){
	buffer.push_back( static_cast<Food&>( *loc ) );
    }
}

void World::addMobile( Mobile* mob )
{
    mob->setWorld( *this );
    mobiles.push_back( mob );
    gridMobiles.putItem( mob );
}

void World::reportDeadBot( Mobile& mob )
{
	//Called from the dead bot.
	//TODO: remove bot from the grid and from the list
}
