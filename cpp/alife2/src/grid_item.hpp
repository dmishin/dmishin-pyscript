#pragma once
#ifndef _GRID_ITEM_H_
#define _GRID_ITEM_H_

#include "located.hpp"

namespace alife2{
    class Grid;
    class GridCell;

    /**Objects, that are located inthe grid, should be children of this class*/
    class GridItem: public Located{
      protected:
	GridCell *pOwnerCell;
      public:
	GridItem();
	GridItem( const vec2& pos );
	
	virtual ~GridItem(){};
	virtual GridCell* getOwnerCell();
	virtual void setOwnerCell( GridCell* pCell );
    };
};


#endif /* _GRID_ITEM_H_ */
