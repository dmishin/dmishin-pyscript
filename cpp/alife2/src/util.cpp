#include "util.hpp"
#include <stdlib.h>
#include <limits.h>

using namespace alife2;

float alife2::randf()
{
    return float(rand())/float(INT_MAX);
}
float alife2::randf( float x0, float x1)
{
    return (x1-x0)*alife2::randf()+x0;
}
