#include "grid_item.hpp"
#include <stdlib.h>

using namespace alife2;
GridItem::GridItem( const vec2& pos )
    :Located( pos ),
     pOwnerCell( NULL )
{
};

GridItem::GridItem():
    pOwnerCell( NULL )
{
}

GridCell* GridItem::getOwnerCell()
{
    return pOwnerCell;
}
void GridItem::setOwnerCell( GridCell* pCell )
{
    pOwnerCell = pCell;
}
    
