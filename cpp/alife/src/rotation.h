#pragma once
#ifndef _ROTATION_H_INCLUDED_
#define _ROTATION_H_INCLUDED_
#include "vec2.h"
#include <math.h>
class rotation{
	private:
		rotation( ftype c, ftype s): v(c, s){};
		
	public:
  ftype sin()const{ return v.y; };
  ftype cos()const{ return v.x; };
		vec2 v; //con_sin vector

		explicit rotation( ftype alpha ): v( cos(alpha), sin( alpha ) ) {};
		
		rotation(){};

		ftype angle()const{
			return (ftype)atan2(v.x, v.y);
		}

		void normalize(){
			v.normalize();
		}

		void set( ftype alpha ){
			v = vec2( cos(alpha), sin(alpha));
		}

		void setSmall( ftype alpha){
			v = vec2( 1-sqr(alpha)*(ftype)0.5, alpha);
		}

		rotation operator +( const rotation &r)const{
			return rotation( v.x * r.v.x - v.y * r.v.y, v.x * r.v.y + v.y * r.v.x );
		}
		
		rotation operator -( const rotation &r)const{
			return rotation( v.x*r.v.x+v.y*r.v.y, -v.x*r.v.y+v.y*r.v.x );
		}

		vec2 apply( const vec2 &vec)const{
			return vec2( vec.x*v.x - vec.y*v.y, vec.x*v.y + vec.y*v.x);
		}
};

#endif
