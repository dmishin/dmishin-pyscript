#pragma once
#ifndef _TICKER_H_
#define _TICKER_H_
#include "ftype.h"

class Ticker
{
private:
    ftype time;
    ftype period;

public:
    explicit Ticker( ftype _period):time(0), period(_period){};
    //add time step and return, how many tcks passed
    int step( ftype dt ){
	time += dt;
	int rval = 0;
	while( time >= period){ //Note: integer division seems more correct way, but the usage supposes that in majority of cases, rval is 0 and only sometimes it is 1. So, this way of doing it may be more appropriate;
	    rval ++;
	    time -= period;
	}
	return rval;
    }
    void reset(){ time = 0;};
    ftype getPeriod()const{ return period; };
    ftype getTime()const {return time; };
};
#endif
