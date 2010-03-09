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

#include "simulator.h"
#include <stdlib.h>
#include <stdexcept>
#include "world.h"

Simulator::Simulator()
:mobilesGridUpdateTicker( 10 ) //update mobiles every 1.0 unit of time
{
    world = NULL;
    dt = (ftype)1e-5;
    simulatedSteps = 0;
    simulatedTime = 0;
    stopRequest = false;
}

void Simulator::simulate()
{
    if ( !world ){
	throw std::logic_error("Failed to start simulation: world not set");
    }

    while(! isStopRequested() ){
	simulateStep( dt );
	simulatedSteps ++;
	simulatedTime += dt;
    }

    stopRequest = false;//stop request fulfilled
}

void Simulator::simulateStep( ftype dt )
{
    Mobiles::iterator i=mobiles.begin(), e=mobiles.end();
    while( i!=e ){
	(*i)->simulate( dt );

	if (! (*i)->isAlive() ){
	    mobiles.erase( i++ );//first erase then increase
	    //++i;
	}else{
	    ++i;
	}
    }
    if (mobilesGridUpdateTicker.step( dt )){
	world->updateGrids(true/*mobiles*/, false/*food*/);
	//std::cout<<"Updated\n";
	//std::cout.flush();
    }
}

bool Simulator::isStopRequested()
{
    return stopRequest;
}
    /**abstrac simulator implementation*/
void Simulator::prepareSimulation( World & w)
{
    mobiles.clear();
    setWorld( w );
}
void Simulator::onNewBot( MobilePtr mob )
{
    //assert( mobiles.find(mob) == mobiles.end() );
    //mobiles.insert( mob );
    mobiles.push_back( mob );

//    mobiles.erase( mobiles.begin() );
}
