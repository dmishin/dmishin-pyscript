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

std::ostream& operator <<( std::ostream & s, const vec2 &v)
{
	s<<"{"<<v.x<<";"<<v.y<<"}";
}
ftype frnd( )
{
	return static_cast<ftype>(rand())/static_cast<ftype>(INT_MAX);
}
int main( int argc, char* argv[])
{
	Grid g;
	g.setGeometry( 10,10, 10, 10);
	for(int i =0; i<1; ++i){
		g.putItem( new Mobile( vec2(frnd()*10, frnd()*10), frnd()*2*3.1415 ));
	}
	std::cout<<g.toStr();

	initialize( argc, argv, g);
	start_glut_loop();
	std::cout<<"Finished executio"<<std::endl;
	return 0;
}
