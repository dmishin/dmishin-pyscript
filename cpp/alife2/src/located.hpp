#pragma once
#ifndef _LOCATED_H_
#define _LOCATED_H_
#include "point.hpp"

namespace alife2{
    /**Abstract interface to be implemented by the located items*/
    class Located{
    public:
	virtual vec2 getLocation()const = 0;
	virtual void setLocation( const vec2& v ) = 0;
    };
	
};

#endif /* _LOCATED_H_ */
