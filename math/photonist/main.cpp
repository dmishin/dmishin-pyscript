
#include <iostream>
#include <math.h>

class vec
{
public:
    double data[3];

    vec()
    {
        data[0]=0;
        data[1]=0;
        data[2]=0;
    }
    vec(double x, double y, double z)
    {
        data[0]=x;
        data[1]=y;
        data[2]=z;
    }
};

class obj
{
    public:
        vec center;
        double radius;

        bool intersects( const ray& r)const;
    private:
        bool near( const ray &r)const;
    protected:
        virtual bool test_intersect( const ray& r)const=0;
};

class ray
{
    public:
        vec start;
        vec direction;

        double distance( const vec &p)const;
};

bool obj::near( const ray& r)const
{
    return ray.distance( center )<= radius;
}
double ray::distance( const vec&v)const
{
}
int main(int argc, const char *argv[])
{
    return 0;
}


    
    
