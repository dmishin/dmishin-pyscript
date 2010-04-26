#pragma once
#ifndef _LOCATED_H_
#define _LOCATED_H_
#include "point.hpp"

namespace alife2{
    class ILocated{
    public:
	virtual vec2 getLocation()const = 0;
	virtual void setLocation( const vec2& v ) = 0;
    };
	
    class Located :public ILocated{
    protected:
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
