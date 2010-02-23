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
#include <math.h>
#include "ftype.h"
#include "mobile.h"
#include "world.h"
#include "motor.h"

Mobile::Mobile( const vec2 & v, ftype angle)
 :Oriented( v, angle ), speed( 0, 0), rotationSpeed(0)
{
    mass = 1;
    inertion = 1;
    movementFriction = 0.1;
    rotationFriction = 0.1;
    world = NULL;
    numMotors = 0;
    motors = NULL;
}
void Mobile::simulate( ftype dt )
{
    //simulate AI of the bot
    simBrain( dt );
    //simulate movement
    pos += speed*dt;
    rot.add( dt*rotationSpeed );
    applyLimits();
    //Simulate forces
    simMotors( dt );

    //friction
    simFriction( dt );

}
void Mobile::simBrain( ftype dt )
{
//TODO
}
void Mobile::simFriction( ftype dt )
{
    ftype movementFrictionForce = speed.norm()*movementFriction*world->getViskosity();
    ftype rotationFrictionForce = sqr(rotationSpeed)*rotationFriction*world->getViskosity();
	//TODO uneffective computation
    //apply friction forces
    //speed -= speed/speed.norm() * movementFrictionForce / mass * dt;
    speed *= ( 1 - movementFrictionForce / mass * dt);
    rotationSpeed -= sign(rotationSpeed)*rotationFrictionForce / inertion * dt;
}
void Mobile::simMotors( ftype dt )
{
    for( int i = 0; i<numMotors; ++i){
	Motor & m = *(motors[i]);
	applyForceR( dt, m.getForce(), m.getPos());
    }
}
/**Apply force at absolute coordinate*/
void Mobile::applyForceA( ftype dt, const vec2& force, const vec2& applyAt)
{
    vec2 l = applyAt - pos;
    speed += force* (1/mass) * dt;

    ftype rotationForce = psprod( l, force );
    rotationSpeed += rotationForce/inertion * dt;
}
/**Apply force at relative coords*/
void Mobile::applyForceR( ftype dt, const vec2& force, const vec2& applyAt)
{
    vec2 l = rot.apply(applyAt);
    speed += rot.apply(force)* (1/mass) * dt;

    ftype rotationForce = psprod( applyAt, force );
    rotationSpeed += rotationForce/inertion * dt;

}

void Mobile::applyLimits()
{
    pos.x = limit( pos.x, ftype(0), world->getSize().x);
    pos.y = limit( pos.y, ftype(0), world->getSize().y);
}
