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
#include <stdlib.h>
#include <cassert>
#include "ftype.h"
#include "mobile.h"
#include "world.h"
#include "motor.h"
#include "brain.h"

Mobile::Mobile( const vec2 & v, ftype angle)
 :Oriented( v, angle ), speed( 0, 0), rotationSpeed(0)
{
    mass = 1;
    inertion = 1;
    movementFriction = 0.1;
    rotationFriction = 0.1;
    energy = 1;

	initMotors();
	
    world = NULL;
    brain = NULL;
}
void Mobile::initMotors()
{
	/*Two motors at the top, <> and tvo at the bottom*/
	motors[0] = Motor( vec2(1, -1), vec2(0, -1));
	motors[1] = Motor( vec2(-1, -1), vec2(0, -1));
	motors[2] = Motor( vec2( 0, 1), vec2(1, 0));
	motors[3] = Motor( vec2( 0, 1), vec2(-1, 0));
}
void Mobile::initSensors()
{
	/*Two food sensors, left and right*/
	foodSensors[0] = Sensor( vec2( ));
	foodSensors[0].setParameters( 1, 3 );

	foodSensors[1] = Sensor();
	foodSensors[1].setParameters( 1, 3 );
}
void Mobile::simulate( ftype dt )
{
	//world perception
	simSensors( dt );
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
void Mobile::simSensors( ftype dt )
{
	//update values of the sensors which need this
	//TODO: add some minimal sensor update time? Sensors can be computationally hard
	assert( world );
	for (int i = 0; i< NUM_FOOD_SENSORS; ++i){
		foodSensors[i].update( *this, *world, dt );
	}
}
void Mobile::simBrain( ftype dt )
{
    if (! brain ) return;
    brain->simulate( *this, dt);
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
    if (! motors ) return;
    for( int i = 0; i<NUM_MOTORS; ++i){
	Motor & m = motors[i];
	applyForceR( dt, m.getForce(), m.getPos());
	energy -= dt * abs(m.getForceValue()) * world->getEnergyConsumptionRate();
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
    if (pos.x<0){
	pos.x = -pos.x;
	speed.x = -speed.x;
    }else if( pos.x>world->getSize().x){
	pos.x = 2*world->getSize().x - pos.x;
	speed.x = -speed.x;
    }
    if (pos.y<0){
	pos.y = -pos.y;
	speed.y = -speed.y;
    }else if( pos.y>world->getSize().y){
	pos.y = 2*world->getSize().y - pos.y;
	speed.y = -speed.y;
    }
}

void Mobile::setMotor( int idx, ftype value)
{
    assert( idx >= 0 && idx < NUM_MOTORS );
    motors[idx].setForceValue( value );
}
ftype Mobile::getSensor( int idx)const
{
    assert( idx >= 0 && idx< NUM_SENSORS );
	switch( idx ){
		case 0: return foodSensors[0].getValue();
		case 1: return foodSensors[1].getValue();
		case 3: return energy; //energy sensor
		default:
			assert( false );
	};
    return 0;
}
