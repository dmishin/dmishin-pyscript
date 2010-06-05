#ifndef _BALANCER_TYPES_H_
#define _BALANCER_TYPES_H_

#include <vector>

typedef unsigned long TimeType;
typedef std::vector< TimeType > TimesVector;
typedef std::vector< int > SizesVector;

/**Compare two time marks, 
   Taking in account that overflow of the time counter can occur*
   assumed, that both time marks are somewhere near the t0
*/
struct TimeLess{
    TimeType t0;
    TimeLess( TimeType t0_ ): t0(t0_){};
    bool operator()(TimeType t1, TimeType t2)const{
	return static_cast<int>(t1-t0) < static_cast<int>(t2-t0);
    };
};




#endif /* _BALANCER_TYPES_H_ */
