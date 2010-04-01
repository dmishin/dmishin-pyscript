#pragma once 
#ifndef __MATRIX_BREEDER_INCLUDED__
#define __MATRIX_BREEDER_INCLUDED__
#include "breeder.h"

class MatrixBreeder:public Breeder
{
public:
    
    MatrixBreeder();
    MatrixBreeder( int minBots );
    virtual void onIdle( World& world);
private:
    int minimalBots;
    int numBrainStates;
    int numBrainIntermediate;
    void makeNewBot( World& w);
    bool createClone(World& w, bool mutate=true);
    bool createChild(World& w);
    bool createOrphan(World& w);
};

#endif
