#include "located.hpp"

using namespace alife2;

vec2 Located::getLocation()const
{
    return position;
}
void Located::setLocation( const vec2& v )
{
    position = v;
}
