#pragma once
#ifndef _ITEM_ENUMERATOR_H_
#define _ITEM_ENUMERATOR_H_

namespace alife2{
    class GridItem;
    /**Abstract class, used for enumerating items in the grid, as callback object*/
   class ItemEnumerator{
    public:
	virtual void enumerate( GridItem & item ) = 0;
    };
}

#endif /* _ITEM_ENUMERATOR_H_ */
