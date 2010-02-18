/***************************************************************************
 *            vec2.h
 *
 *  Sun Jan 31 00:45:29 2010
 *  Copyright  2010  dim
 *  <dim@<host>>
 ****************************************************************************/

#ifndef VEC2_INCLUDED
#define VEC2_INCLUDED

#include <math.h>
//definitions for the base floating-point type.
#include "ftype.h"

class vec2
{
	public:
		ftype x,y;

		vec2( ftype nx, ftype ny): x(nx), y(ny) {};
		vec2() {};

		vec2& operator +=( const vec2& v){
			x+= v.x;
			y+= v.y;
			return *this;
		};
		vec2& operator -=( const vec2& v){
			x-= v.x;
			y-= v.y;
			return *this;
		};
		vec2 operator +( const vec2& v)const{
			return vec2( x+v.x, y+v.y);
		};
		vec2 operator -( const vec2& v)const{
			return vec2( x-v.x, y-v.y);
		};

		vec2& operator *=( ftype k){
			x*=k;
			y*=k;
			return *this;
		};
		vec2 operator *( ftype k)const{
			return vec2(x*k, y*k);
		};
		ftype norm2() const{
			return sqr(x)+sqr(y);
		};
		ftype norm() const{
			return (ftype)sqrt( norm2() );
		};
		vec2& normalize(){
			return (*this) *= (1/norm());
		};
};

//Some helper geometry fucntions
inline
bool in_rect(const vec2& v, ftype x0, ftype y0, ftype x1, ftype y1)
{
	return v.x >= x0 && v.x<=x1 && v.y>=y0 && v.y<=y1;
};
inline
bool in_rect(const vec2& v, const vec2& p0, const vec2& p1)
{
	return in_rect(v, p0.x, p0.y, p1.x, p1.y);
};
inline
ftype dist2(const vec2& v1, const vec2& v2)
{
	return (v1-v2).norm2();
}
inline
ftype dist(const vec2& v1, const vec2& v2)
{
	return (v1-v2).norm();
}
#endif
