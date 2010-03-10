#pragma once
#ifndef _FOOD_BREEDER_H_
#define _FOOD_BREEDER_H_
#include "breeder.h"

class FoodBreeder: public Breeder
{
private:
    int minFoodItems;
public:
    FoodBreeder( int _numFood);
    virtual ~FoodBreeder(){};
    void onIdle( World& w);
};

#endif
