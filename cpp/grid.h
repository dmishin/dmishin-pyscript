#include "vec2.h"

class grid
{
    private:
        class cell
        {
        };

        std::vector<grid::cell> cells;
        cell outer_cell;

        int w,h;
        
        typedef std::pair<int, int> int_pair;

        cell & get_cell( int i, int j);




};

