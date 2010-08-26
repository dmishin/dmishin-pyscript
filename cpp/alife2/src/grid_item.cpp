#include "grid_item.hpp"
#include "grid.hpp"
#include <stdlib.h>
#include <cassert>

using namespace alife2;
GridItem::GridItem( const vec2& pos )
    :Located( pos ),
     pOwnerCell( NULL ),
     pOwner( NULL )
{
};

GridItem::GridItem():
    pOwnerCell( NULL ),
    pOwner( NULL )
{
}

GridCell* GridItem::getOwnerCell()
{
    return pOwnerCell;
}
Grid* GridItem::getOwner()
{
    return pOwner;
}
void GridItem::setOwner( Grid* pNewOwner )
{
    pOwner = pNewOwner;
}

void GridItem::setOwnerCell( GridCell* pCell )
{
    pOwnerCell = pCell;
}
    
/**Update cell the item belongs to*/
void GridItem::updateCell()
{
    assert( pOwner );//Owner must be defined
    //TODO: use cell.is_inside instead
    GridCell * cell = pOwner->findCell( getLocation() );
    if ( cell != pOwnerCell ){
	//Moved to anouther cell
	if ( pOwnerCell ){
	    pOwnerCell->removeItem( this );
	}
	pOwnerCell = cell;
	if ( cell ){
	    cell->addItem( this );
	}
    }
}


GridItem::~GridItem()
{
    if (pOwnerCell ){
	pOwnerCell->removeItem( this );
	pOwnerCell = NULL;
    }
}
