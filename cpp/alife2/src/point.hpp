#pragma once
#ifndef _POINT_H_
#define _POINT_H_
#include "util.hpp"

namespace alife2{
    class vec2;
    
    vec2 operator + (const vec2& a, const vec2& b);
    vec2 operator - (const vec2& a, const vec2& b);
    vec2 operator * (const vec2& v, float k);
    vec2 operator * (float k, const vec2& v);


    class vec2{
    public:
	float x,y;
	vec2( float x_, float y_):x(x_), y(y_){};
	vec2():x(0), y(0){};
	///////// geometry operations //////////
	float dist2( const vec2& v)const{
	    return ((*this)-v).len2();
	};
	float dist( const vec2& v)const{
	    return ((*this) - v ).len();
	};

	vec2& operator += (const vec2& v){
	    x += v.x; y += v.y;
	    return *this;
	};

	vec2& operator -= (const vec2& v){
	    x -= v.x; y -= v.y;
	    return *this;
	};

	vec2& operator *= (float k){
	    x *= k; y *= k;
	    return *this;
	};

	float len2()const{  return sqr(x) + sqr(y); };
	float len() const{  return sqrtf( len2() ); }; 
	
	/**test two vectors for equality with given treshold*/
	bool eqals( const vec2& v, float eps )const{
	    return dist2( v ) <= sqr( eps );
	};
    };        

    inline
    vec2 operator + (const vec2& a, const vec2& b){
	return vec2( a.x+b.x, a.y+b.y);
    };

    inline
    vec2 operator - (const vec2& a, const vec2& b){
	return vec2( a.x+b.x, a.y+b.y);
    };
    
    inline
    vec2 operator * (const vec2& v, float k){
	return vec2(v.x*k,v.y*k);
    };
    
    inline
    vec2 operator * (float k, const vec2& v){
	return vec2(v.x*k,v.y*k);
    };

    /**Scalar product*/
    inline float sprod( const vec2& v1, const vec2 &v2){
	return v1.x*v2.x + v1.y*v2.y;
    };


    class rectangle{
    public:
	vec2 top_left;
	vec2 bottom_right;
	rectangle(): top_left(0,0), bottom_right( 0,0 ){};
	rectangle( float left, float top, float right, float bottom)
	    :top_left( left, top), bottom_right( right, bottom ){};
	rectangle( const vec2& top_left_, const vec2 & bottom_right_)
	    :top_left( top_left_ ), bottom_right( bottom_right_ ){};

	vec2 size()const { return bottom_right - top_left; };
	float left()const { return top_left.x; };
	float right()const { return bottom_right.x; };
	float top()const { return top_left.y; };
	float bottom()const { return bottom_right.y; };

	bool contains( const vec2& v )const { 
	    return v.x >= top_left.x && v.y >= top_left.y &&
		v.x < bottom_right.x && v.y < bottom_right.y;
	}
	rectangle operator + (const vec2 & offset )const {return rectangle( top_left+offset, bottom_right+offset ); };
	rectangle & operator += (const vec2 & offset){ 
	    top_left += offset;
	    bottom_right += offset;
	    return *this;
	}
    };


    class circle{
    public:
	vec2 center;
	float radius;
	
	circle(): center(0,0), radius( 0 ){};
	circle( const vec2& c, float r ): center( c ), radius( r ){};
	circle( float x, float y, float r): center( x, y), radius( r ){};
	
	bool contains( const vec2 & v )const{ return center.dist2( v ) <= sqr( radius ); };
	circle operator + (const vec2 & v)const{ return circle( center + v, radius ); };
	circle & operator += (const vec2 & v) { center += v; return *this; }	    
    };
		
};

#endif /* _POINT_H_ */
