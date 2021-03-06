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

#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_
/**Simulator performs simulation of the world*/
#include <list>
#include "ftype.h"
#include "abstract_simulator.h"

class World;

class Simulator: public AbstractSimulator
{
public:
    Simulator();
    void setWorld( World& w){ world = & w; };
    void setDt( ftype dt1 ){ dt = dt1; };
    //execution control
    void simulate();
    void requestStop(){ stopRequest = true; };
    //get statistics
    int getSimulatedSteps()const{ return simulatedSteps; };
    double getSimulatedTime()const{ return simulatedTime; };

    /**abstrac simulator implementation*/
    virtual void prepareSimulation( World & w);
    virtual void onNewBot( MobilePtr mob );
    virtual void stopSimulation() { requestStop(); };
    virtual void setDelay( int timeMs );
protected:

private:
//    typedef std::set< MobilePtr > Mobiles;
	
    typedef std::list< MobilePtr > Mobiles;
    Mobiles mobiles;
    World * world;


    ftype dt;
    int simulatedSteps;
    double simulatedTime;
    int simulationDelay;

    volatile bool stopRequest;
private:
    void simulateStep( ftype dt );
    bool isStopRequested();
};

#endif // _SIMULATOR_H_
