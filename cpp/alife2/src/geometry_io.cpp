#include <iostream>
#include "geometry_io.hpp"
#include "point.hpp"

namespace alife2{

std::ostream & operator << ( std::ostream & os, const alife2::vec2 & x )
{
    os<<"("<<x.x<<";"<<x.y<<")";
    return os;
}
std::ostream & operator << ( std::ostream & os, const alife2::circle & x )
{
    os<<"{c="<<x.center<<";r="<<x.radius<<"}";
    return os;
}
std::ostream & operator << ( std::ostream & os, const alife2::rectangle & x )
{
    os<<"{"<<x.top_left<<";"<<x.bottom_right<<"}:";
    return os;
}


}
