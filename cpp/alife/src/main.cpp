/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.cc
 * Copyright (C) Dmitry 2010 <>
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

#include <iostream>
#include "vec2.h"
#include "grid.h"
#include "alife_glut.h"
#include "limits.h"
#include "stdlib.h"
#include "mobile.h"
#include "world.h"
#include "glut_viewport.h"
#include "simulator.h"
#include "random_brain.h"
#include "matrix_brain.h"

std::ostream& operator <<( std::ostream & s, const vec2 &v)
{
    s<<"{"<<v.x<<";"<<v.y<<"}";
    return s;
}


//Callable for running the simulator
struct simulator_runner
{
    Simulator* sim;
    simulator_runner( Simulator& s): sim(&s){};
    void operator()(){ 
	std::cout<<"Started simulation\n";
	std::cout.flush();
	sim->simulate();
	std::cout<<"Finished simulation\n";
	std::cout.flush();
    };
};

#include <boost/thread.hpp>

int main( int argc, char* argv[])
{
    World w( vec2( 100, 100), 1);

    for(int i =0; i<100; ++i){
	MobilePtr mob(new Mobile( w.center(), 0 ));
	Brain* brn = new RandomBrain();
	//MatrixBrain * brn = new MatrixBrain( mob->getNumSensors(), 5/*satates*/, 10/*intermediate*/, mob->getNumMotors());
	//brn->randomInit();
	mob->setBrain( *brn );
	w.addMobile( mob );
    }
    //fill world with food
    for( int i = 0;i<1000; ++i)
    {
	FoodPtr f(new Food( vec2( frnd(0, 100), frnd(0, 100)) ));
	w.addFood( f );
    }
    //Simulator for processing the data

    Simulator simulator;
    simulator.setWorld( w );
    simulator.setDt( 0.001 );

    boost::thread simThread = boost::thread( simulator_runner( simulator ));
	
    GlutGuiViewport vp( w, vec2(50,50), 12 );
    vp.setActive();
	
    GlutGuiViewport::init( argc, argv );
    GlutGuiViewport::startLoop();
		
    std::cout<<"Finished execution."<<std::endl;
    return 0;
}
