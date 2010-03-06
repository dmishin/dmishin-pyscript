#pragma once 
#ifndef __MATRIX_BREEDER_INCLUDED__
#define __MATRIX_BREEDER_INCLUDED__
#include "breeder.h"

class MatrixBreeder:public Breeder
{
public:
    MatrixBreeder();
    virtual void onIdle( World& world);
private:
    void makeNewBot( World& w);
};

#endif
