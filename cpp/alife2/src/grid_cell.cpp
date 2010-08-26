#include <assert.h>
#include "grid.hpp"
using namespace alife2;

void GridCell::init( int col_, int row_, Grid* owner_ )
{
    col = col_; row = row_; grid = owner_;
    vec2 size = grid->getCellSize();
    topLeft = vec2( size.x*col, size.y*row );
};

//Check, whether the cell contains the vector
bool GridCell::contains( const vec2 &v )const
{
    vec2 p0 = getTopLeft();
    vec2 p1 = getBottomRight();
    return (p0.x < v.x && p0.y < v.y && p1.x >= v.x && p1.y >= v.y);
}

//Operations on grid items
bool GridCell::hasItem( const GridItem * itm ) const
{
    assert( itm );
    ReadLockType( cellAccessMutex );
    return items.find( const_cast<GridItem*>(itm) ) != items.end();
}
//put item to the cell
void GridCell::addItem( GridItem * itm )
{
    assert( ! hasItem( itm ) );
    //Create lock
    WriteLockType lock( cellAccessMutex );
    items.insert( itm );
}
//remove item from grid
void GridCell::removeItem( GridItem *itm )
{
    assert( itm );
    WriteLockType lock( cellAccessMutex );
    int num_erased = items.erase( itm );
    assert( num_erased == 1 );//Failed to erase. Item not found?
}
