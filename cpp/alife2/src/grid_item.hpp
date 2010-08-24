#pragma once
#ifndef _GRID_ITEM_H_
#define _GRID_ITEM_H_

#include "located.hpp"

namespace alife2{
    class Grid;
    class GridCell;

    /**Objects, that are located inthe grid, should be children of this class
     * Additionally to its position, grid item knows the cell it belongs to.
     */
    class GridItem: public Located{ //inherits protected poistion
      protected:
	GridCell *pOwnerCell;
	Grid* pOwner;
      public:
	GridItem();
	GridItem( const vec2& pos );
	virtual ~GridItem();
	
	GridCell* getOwnerCell();
	Grid* getOwner();
	void setOwner( Grid* pNewOwner );
	void setOwnerCell( GridCell* pCell );

	void updateCell();
    };
};


#endif /* _GRID_ITEM_H_ */
