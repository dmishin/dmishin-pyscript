#pragma once
#ifndef _TICKER_H_
#define _TICKER_H_

namespace alife2{

    class ticker
    {
    private:
	float time;
	float period;

    public:
	ticker():time(0), period(0){};
	explicit ticker( float _period):time(0), period(_period){};
	//add time step and return, how many tcks passed
	int step( float dt ){
	    if (period <=0)
		return false;
	    time += dt;
	    int rval = 0;
	    while( time >= period){ //Note: integer division seems more correct way, but the usage supposes that in majority of cases, rval is 0 and only sometimes it is 1. So, this way of doing it may be more appropriate;
		rval ++;
		time -= period;
	    }
	    return rval;
	}
	void reset(){ time = 0;};
	float get_period()const{ return period; };
	void set_period( float p ){ period = p; time = 0; };
	float get_time()const {return time; };
    };

};
#endif
