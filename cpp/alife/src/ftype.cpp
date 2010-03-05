#include "ftype.h"
#include <stdlib.h>
#include <limits.h>
ftype frnd( )
{
    return static_cast<ftype>(rand()/static_cast<double>(INT_MAX));
}
ftype frnd (ftype low, ftype high)
{
    return low+(high-low)*frnd();
}
