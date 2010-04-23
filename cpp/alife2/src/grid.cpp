#include <assert.h>
#include <algorithm>

#include "grid.hpp"
#include "util.hpp"

using namespace alife2;

void Grid::initGrid( float width_, float height_, float cellSize)
{
    width = width_;
    height = height_;

    numCols = max( 1, int(width_ / cellSize) );
    numRows = max( 1, int(height_ / cellSize) );

    cellWidth = width_ / numCols;
    cellHeight = height_ / numRows;

    //Init cell borders
    verticalCellBorders.resize( numCols );
    for (int col = 0; col < numCols; ++col){
	verticalCellBorders[ col ] = (col + 1)*cellWidth;
    }

    horizontalCellBorders.resize( numCols );
    for (int row = 0; row < numRows; ++row ){
	horizontalCellBorders[ row ] = (row + 1)*cellHeight;
    }

    //Init cells
    cells.resize( numCols* numRows + 1); //add one additional cell
    for (int col = 0; col < numCols; ++col){
	for (int row = 0; row < numRows; ++row ){
	    GridCell & cell = getCell( col, row );
	    cell.init( col, row, this );
	}
    }
}

GridCell & Grid::getCell( int col, int row )
{
    assert( col >=0 && col < numCols );
    assert( row >=0 && row < numRows );
    return cells[ col + row * numCols ];
}

GridCell & Grid::getLastCell()
{
    return cells[ cells.size() - 1 ]; //TODO: some better code?
}


/**Binary search in the array
   for te array {a0, a1, a2, ..., an} 
   and value v
   returns pointer for the element *ai, such that ai>v and a{i-1}<=v
   if not found, returns pointer to the nearest elemnet and flag false
*/

/*   //not really needed - I am too stupid..
template <typename iterator, typename value_type >
std::pair<iterator, bool> find_bound( iterator begin, iterator end, value_type val )
{
    int num_items = end - begin;
    if ( num_items == 0){
	return std::make_pair( begin, false ); //not found
    }
    if ( num_items == 1){
	return std::make_pair( begin, *begin < val );
    }
  
    //else - use dichotomy
    iterator center = begin + (num_items/2);
    if ( *center < val )
	return find_bound( begin, center, val );
    else
	return find_bound( center, end, val );
}
*/

/**Find the cell by the object coordinates*/
GridCell & Grid::findCell( const vec2 & vec )
{
    using namespace std;
    int col = int(floor( vec.x / cellWidth ) );
    int row = int(floor( vec.y / cellHeight ) );
    
    if ( col >=0 && col < numCols && row >= 0 && row < numRows ) {
	return getCell( col, row );
    }else{
	return getLastCell();
    }
}


/////////////////////////////////
/// Grid Cell
/////////////////////////////////

bool GridCell::contains( const vec2 &v )const
{
    vec2 p0 = getTopLeft();
    vec2 p1 = getBottomRight();
    return (p0.x < v.x && p0.y < v.y && p1.x >= v.x && p1.y >= v.y);
}

