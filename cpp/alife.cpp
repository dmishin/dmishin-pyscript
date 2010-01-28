
#include <vector>
#include <math.h>
#include <iostream>

typedef double ftype;

class vec2
{
public:
    ftype x,y;

    vec2( ftype x_, ftype y_): x(x_), y(y_){};
    vec2(){};
    
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

ftype sqr( ftype x )
{
    return x*x;
};

ftype vec2::dist2( const vec2&v)const
{
    return sqr(x-v.x)+sqr(y-v.y);
}
std::ostream & operator <<( std::ostream & s, vec2 v)
{
    s<<"("<<v.x<<";"<<v.y<<")";
    return s;
}

int main( int argc, const char* argv[])
{
    vec2 v(10,11);
    std::cout<<v.normalized()<<std::endl;
    return 0;
}
