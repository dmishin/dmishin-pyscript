#include <assert.h>
#include <algorithm>
#include <sstream>
#include <boost/tuple/tuple.hpp>

#include "grid.hpp"
#include "util.hpp"

using namespace alife2;
using namespace boost;

Grid::Grid()
{
    population = 0;
    width = 0;
    height = 0;
    numCols = 0;
    numRows = 0;
    
}
Grid::Grid( float w, float h, float cellSize)
{
    population = 0;
    initGrid( w, h, cellSize );
}


void Grid::initGrid( float width_, float height_, float cellSize)
{
    population = 0;

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
    getLastCell().initOutside( this ); //last cell is an outside cell.
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

/**Updating items in cells: 
 *for each item in each cell, 
 * check, whether it belongs to the gight cell, 
 * and if needed - put it to the right one*/
void Grid::update()
{
    typedef std::vector< GridItem* >  ItemVector; //TODO: isn't it performance loss?
    ItemVector repositionedItems;

    Cells::iterator i;
    for( i = cells.begin(); i != cells.end(); ++i ){
	GridCell & cell = *i;
	//iterate items inside cell
	//TODO: Grab cell monitor?
	GridCell::Items::iterator iItem = cell.items.begin();
	while ( iItem != cell.items.end() ){
	    GridItem * item = *iItem;
	    if ( cell.contains( item->getLocation() ) ){
		++iItem;
		continue; //OK, item is inside the cell;
	    }else{
		//Item is outside the cell and must be updated
		cell.items.erase( iItem++ );
		repositionedItems.push_back( item );
	    }
	}
    }
    //Now put the repositioned items back to the grid
    
    for ( ItemVector::iterator i = repositionedItems.begin(); i != repositionedItems.end(); ++i ){
	population --;
	putItem( *i );
    }	
}

void Grid::putItem( GridItem * item )
{
    assert( item );
    //first get the cell reference for this item
    GridCell & cell = findCell( item->getLocation() );
    cell.addItem( item );
    population ++;
}

void Grid::removeItem( GridItem * item )
{
    assert( item ); //item is real 
    assert( item->getOwnerCell() );//and owner cell is right
    assert( item->getOwnerCell()->getGrid() == this );//and item belongs to this grid

    item->getOwnerCell()->removeItem( item );
    item->setOwnerCell( NULL );//item does not belongs to anything anymore.
    population --;
}
#include <iostream>
int Grid::enumerateInRectangle( const rectangle & r, ItemEnumerator & enumerator )
{
    //determine cell bounds
    int count = 0;
    int leftBound, rightBound; 
    tie( leftBound, rightBound ) = hrzCellIndexRange( r.left(), r.right() );
    int topBound, bottomBound;
    tie( topBound, bottomBound ) = vrtCellIndexRange( r.top(), r.bottom() );

//    std::cout<<r.left()<<":"<<r.right()<<"|"<<r.top()<<":"<<r.bottom()<<std::endl;
//    std::cout<<"bounds:"<<leftBound<<":"<<rightBound<<"|"<<topBound<<":"<<bottomBound<<std::endl;
//    std::cout.flush();

    for( int x = leftBound; x <= rightBound; ++x ){
	for( int y = topBound; y <= bottomBound; ++y ){
	    GridCell & cell = getCell( x, y );
	    for( GridCell::Items::iterator i = cell.items.begin(); i != cell.items.end(); ++i){
		assert( *i );//Item must be non-null
		if ( r.contains( (*i)->getLocation() ) ){
		    if (! enumerator.enumerate( **i ) )
			return count;
		    else
			count++;
		}
	    }
	}
    }
    
}

std::pair<int, int> Grid::hrzCellIndexRange( float x0, float x1 )
{
    return std::make_pair( limit( int(floor( x0 / cellWidth)), 0, numCols-1 ),
			   limit( int(ceil( x1 / cellWidth )), 0, numCols-1 ) );
}
std::pair<int, int> Grid::vrtCellIndexRange( float y0, float y1 )
{
    return std::make_pair( limit( int(floor( y0 / cellWidth)), 0, numRows-1 ),
			   limit( int(ceil( y1 / cellWidth )), 0, numRows-1 ) );
}

int Grid::enumerateInCircle( const circle & c, ItemEnumerator & enumerator )
{
    //determine cell bounds
    int count = 0;
    int leftBound, rightBound;
    tie (leftBound, rightBound ) = hrzCellIndexRange( c.center.x - c.radius, c.center.x + c.radius );
    int topBound, bottomBound;
    tie ( topBound, bottomBound ) = vrtCellIndexRange( c.center.y - c.radius, c.center.y + c.radius );

    for( int x = leftBound; x <= rightBound; ++x ){
	for( int y = topBound; y <= bottomBound; ++y ){
	    GridCell & cell = getCell( x, y );
	    for( GridCell::Items::iterator i = cell.items.begin(); i != cell.items.end(); ++i){
		assert( *i );//Item must be non-null
		if ( c.contains( (*i)->getLocation() ) ){
		    if (! enumerator.enumerate( **i ) )
			return count;
		    else
			count++;
		}
	    }
	}
    }
    
}



/**Utility functions*/
std::string Grid::toString()const
{
    std::ostringstream ostream;
    FOR_RANGE(y, 0, numRows){
	ostream<<"[";
	FOR_RANGE(x, 0, numCols){
	    ostream<<"["<<getCell( x, y ).getPopulation()<<"]";
	}
	ostream<<"]\n";
    }
    ostream<<"Outside cells population:"<<getLastCell().getPopulation();
    return ostream.str();
}
