#pragma once
#ifndef __FOOD_H_INCLUDED__
#define __FOOD_H_INCLUDED__
#include <boost/shared_ptr.hpp>

#include "located.h"

class Food:public Located
{
    ftype value;
public:
    Food(){};
    Food( const vec2& pos, ftype _value=1):Located(pos),value(_value){};
    ftype getValue()const{return value;}
};
typedef boost::shared_ptr<Food> FoodPtr;
#endif
