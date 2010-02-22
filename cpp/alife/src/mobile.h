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

#ifndef _MOBILE_H_
#define _MOBILE_H_
#include "grid.h"
#include "rotation.h"
#include "vec2.h"
#include "oriented.h"


/**Additionally to the Located, Mobile can move*/

class World;
class Motor;

class Mobile: public Oriented
{
public:
    Mobile( const vec2 & v, ftype angle=0);

    void simulate( ftype dt);//simulate Mobile movement for a given interval of time
    void setWorld( World& w){ world = &w; };
protected:
    vec2 speed;
    ftype rotationSpeed;
	
    ftype mass;
    ftype inertion;

    ftype rotationFriction;
    ftype movementFriction;

    World* world;
	Motor **motors;
	int numMotors;

    void addForce();
    //Apply force at specified location.
    //Location is in the absolute coordinates
    void applyForceA( ftype dt, const vec2& force, const vec2& applyAt);
	void applyForceR( ftype dt, const vec2& force, const vec2& applyAt);


    void simMotors( ftype dt );
    void simFriction( ftype dt);
	void simBrain( ftype dt );
    void applyLimits();
private:

};

#endif // _MOBILE_H_
