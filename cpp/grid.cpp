#include "grid.h"


grid::cell & grid::get_cell( int ix, int iy)
{
    if (ix <0 || ix >= w || iy<0 || iy>=h)
        return outer_cell;
    else
    {
        int idx = ix + iy*w;
        return cells[idx];
    }
}

