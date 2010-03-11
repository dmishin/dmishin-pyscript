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
#pragma once
#ifndef _MOBILE_H_
#define _MOBILE_H_
#include "grid.h"
#include "rotation.h"
#include "vec2.h"
#include "oriented.h"
#include "body.h"
#include "motor.h"
#include "sensor.h"
#include <boost/shared_ptr.hpp>

/**Additionally to the Located, Mobile can move*/

class World;
class Brain;

class Mobile: public Oriented, public Body
{
public:
    static const int NUM_MOTORS = 4;
    static const int NUM_SENSORS = 3; //2 food + energy
    static const int NUM_FOOD_SENSORS = 2;
    static const ftype FOOD_EATING_RADIUS = 0.1;

    Mobile( const vec2 & v, ftype angle=0);

    void simulate( ftype dt);//simulate Monbile movement for a given interval of time
    void setWorld( World& w);
    void setBrain( Brain& b){ brain = &b; };
    Brain* getBrain() const {return brain;}
    void setSpeed( const vec2& spd){ speed = spd; };
    void setRotationSpeed( ftype b ){ rotationSpeed = b;};


    //Body impelmentation
    virtual int getNumMotors()const {return NUM_MOTORS; };
    virtual int getNumSensors()const {return NUM_SENSORS; };
    virtual void setMotor( int idx, ftype value);
    virtual ftype getSensor( int idx)const;

    //getters for the advanced display
    const Motor& getMotor( int idx )const;
    const Sensor& getFoodSensor( int idx )const;
    ftype getEnergy()const{ return energy; };
    bool isAlive()const{ return energy>0; };

protected:
//Physical state
    vec2 speed;
    ftype rotationSpeed;
	
    ftype mass;
    ftype inertion;

    ftype rotationFriction;
    ftype movementFriction;
    ftype energy;
/*statistics info*/
	ftype birthday;
	int foodEaten;


    World* world;

    Motor motors[NUM_MOTORS];
	Sensor foodSensors[ NUM_FOOD_SENSORS ];

    Brain* brain;

    void addForce();

    //Apply force at specified location.
    //Location is in the absolute coordinates
    void applyForceA( ftype dt, const vec2& force, const vec2& applyAt); //absolute coordinates
    void applyForceR( ftype dt, const vec2& force, const vec2& applyAt);// relative coordinates


    void simMotors( ftype dt );
    void simFriction( ftype dt);
    void simBrain( ftype dt );
    void simSensors( ftype dt );
    void tryEatFood();
	
    void applyLimits();
private:
/**Initialize motor positions and directions*/
    void initMotors();
    void initSensors();

	bool operator<(const Mobile& mob)const{ return false; };
	bool operator==(const Mobile& mob)const{ return false;};
};

typedef boost::shared_ptr<Mobile> MobilePtr;

#endif // _MOBILE_H_
