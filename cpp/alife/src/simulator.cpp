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
    World::Mobiles::const_iterator i, e=world->getMobiles().end();
    for( i = world->getMobiles().begin(); i!=e; ++i){
	(*i)->simulate( dt );
    }
}

bool Simulator::isStopRequested()
{
    return stopRequest;
}
