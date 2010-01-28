#include "grid.h"


Grid::cell & Grid::get_cell( int ix, int iy)
{
    if (ix <0 || ix >= w || iy<0 || iy>=h)
        return outer_cell;
    else
    {
        int idx = ix + iy*w;
        return cells[idx];
    }
}

void Grid::set_geometry(int cols, int rows, ftype width, ftype height)
{
    int num_cells = cols*rows;
    
}
Grid::item_iterator::item_iterator( Grid & g, vec2 c, ftype r)
    :grid(g), center(c), radius(r)
{

}
Located & operator*()const;
void go_next();
bool is_finished()const;
