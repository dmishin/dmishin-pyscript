#pragma once
#ifndef __ORIENTED_H_INCLUDED__
#define __ORIENTED_H_INCLUDED__

#include "located.h"

class Oriented: public Located
{
public:
    Oriented( const vec2& pos, ftype angle):
	Located(pos), rot( angle )
	{};
    Oriented(){};//fast default constructor
    const rotation& getRotation()const{ return rot; };
    ftype getAngle()const{ return rot.angle(); };
protected:
    rotation rot;
};
#endif
