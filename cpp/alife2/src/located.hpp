#pragma once
#ifndef _LOCATED_H_
#define _LOCATED_H_
#include "point.hpp"

namespace alife2{
    /**Abstract interface to be implemented by the located items*/
    class Located{
    protected:
	vec2 position;
    public:
	Located(): position( 0, 0 ) {};
	Located( const vec2& p ): position( p ) {};
	
	const vec2 & getLocation()const {return position; };
	void setLocation( const vec2& v ) { position = v; };
	
    };
	
};

#endif /* _LOCATED_H_ */
