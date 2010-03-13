#pragma once
#ifndef __BRAIN___INCLUDED__
#define __BRAIN___INCLUDED__
#include "ftype.h"

/**Abstract class, representing instance of an object, controlling the mobile.*/
class Body;
class Brain
{
public:
    Brain();
    virtual ~Brain();
    virtual void simulate( Body & mob, ftype dt)=0;
};

#endif
