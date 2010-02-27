#pragma once
#ifndef _MOTOR_H_INCLUDED_
#define _MOTOR_H_INCLUDED_

#include "vec2.h"
#include "ftype.h"
#include "located.h"

class Motor: public Located
{
protected:
    vec2 direction;
    ftype force;

public:
    Motor ( const vec2& pos
	    , const vec2& dir);

    /* misc getters */
    const vec2& getDirection()const{ return direction; };
    vec2 getForce()const{ return direction* force;};
    ftype getForceValue()const{ return force; };

    /*Sets amount of the force, produced by motor*/
    void setForceValue( ftype f ) {force = f;};
};

#endif
