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

#ifndef _WORLD_H_
#define _WORLD_H_
#include <vector>
#include "mobile.h"
#include "grid.h"
#include "food.h"

/**Utility helper functions*/
template<class function_class, class argument_type>
ftype calcGridFunction( Grid& grid, const vec2& center, ftype radius, function_class& func)
{
    Grid::circular_generator g( grid, center, radius);
    ftype s = 0;
    for(Located *mob = 0; g(mob); ){
	s += func( static_cast<argument_type&>(*mob) );
    }
    return s;
};


class World
{
public:
    //some global parameters
    ftype viskosity;
    ftype energyConsumptionRate;
    ftype getViskosity()const{ return viskosity; };
    ftype getEnergyConsumptionRate()const{ return energyConsumptionRate; };
public:
    typedef std::vector<Mobile*> Mobiles; 

    World( vec2 size, ftype cellSize );
    const vec2& getSize()const{ return size; };
	
    //Basic operation on world
    Mobile * findNearestMobile( const vec2& p, ftype maxDist);

    /**Accumulate function value. Fucntion must take const Mobile& */
    template<class function_class>
    ftype calcMobFunction(const vec2 & p, ftype r, function_class & func){
	return calcGridFunction<function_class, Mobile>( gridMobiles, p, r, func);
    };

    template<class function_class>
    ftype calcFoodFunction(const vec2 & p, ftype r, function_class & func){
	return calcGridFunction<function_class, Food>(gridFood, p, r, func);
    };

/** Get positions of the all bots inside given area*/
    typedef std::vector<Oriented> MobilesSnapshot;
    typedef std::vector<Food> FoodSnapshot;

    void getMobilesSnapshot( const vec2& ptTopLeft, const vec2& ptBottomRight, World::MobilesSnapshot&buffer)const;
    void getFoodSnapshot( const vec2& ptTopLeft, const vec2& ptBottomRight, World::FoodSnapshot& buffer)const;

	int getNumBots()const{ return bots.size();};
    void addMobile( Mobile* mob );
    const World::Mobiles& getMobiles()const{ return mobiles;};
 
/** Receive messages from bots*/
	void reportDeadBot( Mobile& mob); //called, when mobile is dead
protected:

private:
    //Grids, used for quick access by coordinates.
    vec2 size;
    Grid gridMobiles;
    Grid gridFood;
    //arrays of mobiles
    Mobiles mobiles;

};



#endif // _WORLD_H_
