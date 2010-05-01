#pragma once
#ifndef _GRID_MT_H_
#define _GRID_MT_H_

#include "grid.hpp"
#include <boost/thread.hpp>

/**Variation of the grid class that synchronizes access*/
namespace alife2{
    class GridMt: public Grid{
	boost::shared_mutex mutex;
	typedef boost::unique_lock< boost::shared_mutex> unique_lock;
	typedef boost::shared_lock< boost::shared_mutex> shared_lock;

    public:
	GridMt(): Grid(){};
	GridMt( float w, float h, float cellSize):Grid( w, h, cellSize){};

	/**Updating items in cells*/
	void update(){ 
	    unique_lock lock( mutex );
	    Grid::update(); 
	};

	void putItem( GridItem * item ){ 
	    unique_lock lock( mutex );
	    Grid::putItem( item );
	}//Put item to the grid;
	void removeItem( GridItem * item ){
	    unique_lock lock( mutex );
	    Grid::removeItem( item); 
	}//Remove item from the grid;

	/**Getting items in area*/
	int enumerateInRectangle( const rectangle & r, ItemEnumerator & enumr ){
	    shared_lock lock( mutex );
	    return Grid::enumerateInRectangle( r, enumr); 
	}
	int enumerateInCircle( const circle & c, ItemEnumerator & enumr ){
	    shared_lock lock( mutex );
	    return Grid::enumerateInCircle( c, enumr );
	}
    };
};


#endif /* _GRID_MT_H_ */
