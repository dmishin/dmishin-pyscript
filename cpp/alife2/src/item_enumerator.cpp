#include "item_enumerator.hpp"
using namespace alife2;

bool ItemCollector::enumerate( GridItem & item )
{
    items.push_back( &item );
    return true;
}
bool FirstItemGetter::enumerate( GridItem & item_ )
{
    item = &item_;
    return false;//do not continue
}
