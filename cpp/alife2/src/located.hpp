#pragma once
#ifndef _LOCATED_H_
#define _LOCATED_H_
#include "point.hpp"

namespace alife2{
    class Located{
	vec2 position; 
    public:
	Located(){};
	Located( const vec2& v): position( v ) {};
	virtual ~Located(){};
	virtual vec2 getLocation()const;
	virtual void setLocation( const vec2& v );
    };
};

#endif /* _LOCATED_H_ */
