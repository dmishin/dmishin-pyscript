#ifndef VEC_2_INCLUDED
#define VEC_2_INCLUDED

#include "common.h"
//Lightweight class for a 2d vector
class vec2
{
public:
    ftype x,y;

    vec2( ftype x_, ftype y_): x(x_), y(y_){};
    vec2(){};
    
    vec2& set( ftype x_, ftype y_){ x=x_;y=y_;return *this;};

    ftype norm2()const{return x*x+y*y; };
    ftype norm()const{return sqrt(norm2());};

    ftype dist2( const vec2 &v)const;
    ftype dist( const vec2 &v)const{ return sqrt(dist2(v)); };

    vec2& operator += (const vec2 &v)
    {
        x+=v.x;
        y+=v.y;
        return *this;
    }
    
    vec2& operator -= (const vec2 &v)
    {
        x-=v.x;
        y-=v.y;
        return *this;
    }

    vec2& operator *= (ftype f)
    {
        x*=f;
        f*=f;
    }

    vec2 operator +( const vec2& v)const
    {
        return vec2( x+v.x, y+v.y);
    }

    vec2 operator -(const vec2& v)const
    {
        return vec2( x-v.x, y-v.y);
    }

    vec2 operator *( ftype k)const
    {
        return vec2( x*k,y*k);
    }

    vec2& normalize ()
    {
        (*this)*=(1/norm());
        return *this;
    }
    vec2 normalized()const
    {
        ftype n = 1/norm();
        return vec2( x*n, y*n);
    }
};
#endif
