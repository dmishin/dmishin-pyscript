#pragma once
#ifndef _ITEM_ENUMERATOR_H_
#define _ITEM_ENUMERATOR_H_
#include <vector>

namespace alife2{
    class GridItem;
    /**Abstract class, used for enumerating items in the grid, as callback object*/
    class ItemEnumerator{
    public:
	virtual bool enumerate( GridItem & item ) = 0;
    };
    class ItemCollector: public ItemEnumerator{
    public:
	typedef std::vector< GridItem * > Items;
	Items & items;
	ItemCollector( Items & items_ ): items( items_ ){};
	virtual bool enumerate( GridItem & item );
    };

    class FirstItemGetter: public ItemEnumerator{
    public:
	GridItem * item;
	FirstItemGetter(): item( NULL ){};
	virtual bool enumerate( GridItem &item_ );
	bool hasItem()const { return bool(item); };
	GridItem * getItem(){ return item; };
    };
}

#endif /* _ITEM_ENUMERATOR_H_ */
