#include "ftype.h"
#iclude "stdlib.h"

ftype frnd( )
{
    return static_cast<ftype>(rand()/static_cast<double>(INT_MAX));
}
ftype frnd (ftype low, ftype high)
{
    return low+(high-low)*frnd();
}
