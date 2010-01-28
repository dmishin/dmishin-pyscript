#include "vec2.h"

#include "located.h"

class Grid
{
    private:
        typedef object_list std::vector< Located *>;

        class cell
        {
            objects_list items;


        };

        std::vector<grid::cell> cells;
        cell outer_cell;

        int rows, columns;
        ftype width, height;
        
        typedef std::pair<int, int> int_pair;

        cell & get_cell( int i, int j);

        /**Set the grometry of the field*/
        void set_geometry( int cols, int rows, ftype width, ftype height);
        

        class item_iterator
        {//circular enumerator of the items
            private:
                vec2 center;
                ftype radius;
                object_list::iterator current_item;
                int cur_row, cur_column;
                Grid & grid;
            public:
                item_iterator( Grid & g, vec2 center, ftype radius);
                Located & operator*()const;
                void go_next();
                bool is_finished()const;
                item_iterator & operator++(){ go_next(); return *this;};

        };

        item_enumerator find_items( vec2 center, ftype radius)


};



