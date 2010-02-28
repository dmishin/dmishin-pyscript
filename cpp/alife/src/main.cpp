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

std::ostream& operator <<( std::ostream & s, const vec2 &v)
{
	s<<"{"<<v.x<<";"<<v.y<<"}";
	return s;
}

ftype frnd( )
{
	return static_cast<ftype>(rand())/static_cast<ftype>(INT_MAX);
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
	World w( vec2( 10, 10), 1);

	for(int i =0; i<100; ++i){
//	  Mobile* mob = new Mobile( vec2(frnd()*10, frnd()*10), frnd()*2*3.1415 );
		Mobile* mob = new Mobile( vec2(5,5), 0 );
	  Brain* brn = new RandomBrain();
	  mob->setBrain( *brn );
	  w.addMobile( mob );
//	  mob->setSpeed( vec2(frnd()*2-1, frnd()*2-1));
//	  mob->setRotationSpeed( frnd()*2-1 );
	}
	//Simulator for processing the data

	Simulator simulator;
	simulator.setWorld( w );
	simulator.setDt( 0.001 );

	boost::thread simThread = boost::thread( simulator_runner( simulator ));
	
	GlutGuiViewport vp( w, vec2(5,5), 30 );
	vp.setActive();
	
	GlutGuiViewport::init( argc, argv );
	GlutGuiViewport::startLoop();
		
	std::cout<<"Finished execution."<<std::endl;
	return 0;
}
