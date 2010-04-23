#include <iostream>
#include "point.hpp"
#include "rotation.hpp"


std::ostream & operator << (std::ostream &os, const alife2::vec &v)
{
    return os<<"("<<v.x<<","<<v.y<<")";
}


int main(int argc, char *argv[])
{
    using namespace std;
    
    alife2::vec2 v(10,10);
    std::cout << v <<std::endl;
    
    return 0;
}
