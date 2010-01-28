
#include <vector>
#include <math.h>
#include <iostream>

#include "vec2.h"

class rot
{
    public:
        vec2 v;

        rot(){};
        rot (ftype alpha): { set(alpha); };

        ftype angle()const;

        rot& small_rot( ftype alpha );
        
        ftype get_sin()const{return v.x;};
        ftype get_cos()const{return v.y;};

        vec2 apply( const vec2& v1)const
        {
            return vec2( v1.x*v.x-v1.y*v.y,  v1.x*v.y+v1.y*v.x);
        }
        void apply( const vec2& v1, vec2& b)
        {
            b.set(v1.x*v.x-v1.y*v.y,  v1.x*v.y+v1.y*v.x);
        }
        rot& set( ftype alpha);
}


rot& rot::set( ftype alpha )
{
    v.set(cos(alpha), sin(alpha));
    return *this;
}
rot& rot::small_rot( ftype alpha)
{
    v.set(1-sqr(alpha)*0.5, alpha);
    return *.this;
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
