#pragma once
#ifndef _LOCATED_H_INC_
#define _LOCATED_H_INC_
#include "ftype.h"
#include "vec2.h"
class Located{
	protected:
		vec2 pos;
	public:
		explicit Located( const vec2& p ): pos(p){};
		Located (ftype x, ftype y):pos(x,y){};
		Located(){};

		vec2& getPos(){
			return pos;
		}
		const vec2& getPos()const{
			return pos;
		}
};
#endif
