#include "random_choise.h"
#include <stdlib.h>
#include <limits.h>
#include <cassert>

void RandomChoise::add(double probability, int value)
{
    assert(probability > 0);
    sumP += probability;
    choises.push_back(Choise(sumP, value));
}
int RandomChoise::get()const
{
    assert( numChoises() > 0);
    double val = rand()/(double)INT_MAX * sumP;
    return search(val, 0, numChoises() );
}
/**binary search. */
int RandomChoise::search(double v, int begin, int end)const
{
    if (end-begin == 1){//already found
	return choises[begin].second;
    }else{
	int center = (begin+end)/2;
	double vc = choises[center].first;
	if (vc < v){
	    return search(v, center+1, end);
	}else{
	    return search(v, begin, center+1);
	}
    }
}
