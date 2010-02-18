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

#include "mobile.h"
#include "world.h"
Mobile::Mobile( const vec2 & v, ftype angle=0)
:Located( v ), rot(angle), speed( 0, 0), rotationSpeed(0)
{
	mass = 1;
	inertion = 1;
	movementFriction = 0.1;
	rotationFriction = 0.1;
	world = NULL;
}
void Mobile::simulate( ftype dt )
{
	//simulate movement
	pos += speed*dt;
	rot.add( dt*rotationSpeed );
	applyLimits();
	//Simulate forces
	simMotors();
	//friction
	simFriction( dt );
}
void Mobile::simFriction( ftype dt )
{
	ftype movementFrictionForce = 1/speed.norm2()*movementFriction*world->getViskosity();
	ftype rotationFrictionForce = 1/sqr(rotationSpeed)*rotationFriction*world->getViskosity();

}
void Mobile::applyForceA( ftype dt, const vec2& force, const vec2& applyAt)
{
	vec2 l = applyAt - pos;
	speed += force* (1/mass);
	ftype rotationForce = psprod( l, force );
	rotationSpeed.add( rotationForce/inertion );
}

