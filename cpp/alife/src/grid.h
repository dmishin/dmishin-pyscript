/***************************************************************************
 *            grid.h
 *
 *  Sun Jan 31 01:11:16 2010
 *  Copyright  2010  dim
 *  <dim@<host>>
 ****************************************************************************/

#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include <vector>
#include <set>
#include <stdexcept>
#include <string>

#include "vec2.h"
#include "pseudo_generator.h"
#include "located.h"
#include <boost/shared_ptr.hpp>
/**Pointer type for the items in the grid.*/

typedef boost::shared_ptr<Located> GridItemPtr ;

class Grid{
private:  
    int numCols, numRows;
  
    vec2 size;
    vec2 cellSize;
    vec2 cellScale;
public:
    class Cell{
    public:
	typedef std::set< GridItemPtr > CellItems;	
	CellItems items;
				
	void add( GridItemPtr item){
	    items.insert( item );
	}
	void remove( GridItemPtr item){
	    if (items.erase( item ) == 0)
		throw std::logic_error("element not in the cell");//element not found in the set
	}
	bool operator == (const Grid::Cell &c)const{
	    return this == &c;
	}
	bool operator != (const Grid::Cell &c)const{
	    return this != &c;
	}
    };

//Defauult constructor of empty field		
    Grid();
    ~Grid();
		
    Cell unallocated;

    //Cells cells;
    Cell* cells;

    void setGeometry( int cols, int rows, ftype width, ftype height);
		
    /**dimension of the grid*/
    vec2 getSize()const{ return size;};
  
    /**Center of the grid, in real points*/
    vec2 center()const{ return size*(ftype)0.5;};

		
    void putItem( GridItemPtr item);
    void removeItem( GridItemPtr item);
    GridItemPtr findNearestItem( const vec2& p, ftype maxDist);

    /**update positions of all Located items in cells*/
    void update();

    /**Utility class for enumerating items in a rectangle*/
    /**Pseudo-generator, returning items inside a rectangle*/
    struct rectangle_generator: public pseudo_generator<GridItemPtr>{
	//pseudo-generator preamble
	BEGIN_GENERATOR_STATES 
	state1
	END_GENERATOR_STATES;
	//Storage for the state variables.
	ftype x0, y0, x1, y1;
	Grid* grid;

	int ix0, iy0, ix1, iy1;
	int x,y;
	Cell::CellItems::iterator i;
	Cell *cur_cell;
	//empty constructor is required.
	rectangle_generator(){};
	//real constructors
	rectangle_generator(Grid& g, ftype _x0, ftype _y0, ftype _x1, ftype _y1)
	    :x0(_x0), y0(_y0), x1(_x1), y1(_y1),
	     grid(&g)
	    {};
	rectangle_generator(Grid& g, const vec2& p0, const vec2& p1)
	    :x0(p0.x), y0(p0.y), x1(p1.x), y1(p1.y),
	     grid(&g)
	    {};
	//generator body
	GridItemPtr operator()();
    };
  
    typedef generator_iterator<rectangle_generator> rectangle_iterator;
    Grid::rectangle_iterator genItemsInRect(const vec2& p0, const vec2& p1){
	return rectangle_iterator(*this, p0.x, p0.y, p1.x, p1.y);
    }
  
    Grid::rectangle_iterator genItemsInRect(ftype x0, ftype y0, ftype x1, ftype y1){
	return rectangle_iterator(*this, x0, x1, y0, y1);
    };

    struct circular_generator: public pseudo_generator<GridItemPtr>{
	BEGIN_GENERATOR_STATES
	state1
	END_GENERATOR_STATES;
	Grid* grid;
	vec2 center;
	ftype r;
	//state
	ftype max_cell_radius;
	int ix0, ix1, iy0, iy1, ix, iy;
	Grid::Cell::CellItems::iterator iItem, iItemEnd;
	Grid::Cell* curCell;
			
	circular_generator(){};//must be default-constructible
	circular_generator( Grid& g, const vec2& _center, ftype radius)
	    :grid(&g), center(_center), r(radius){};

	GridItemPtr operator()();
    };
    typedef generator_iterator<circular_generator> circular_iterator;
    circular_iterator genItemsInCircle(const vec2& center, ftype r){
	return circular_iterator( *this, center, r);
    }
private:
    void updateCell( Cell & cell);
		
    int index_x( ftype x )const{
	return (int)floor(x* cellScale.x);
    }

    int index_y( ftype y )const{
	return (int)floor(y* cellScale.y);
    }
		
    Cell& cellRef(int ix, int iy){
	if (ix<0 || iy<0 || ix>=numCols || iy>=numRows){
	    return unallocated;
	}else{
	    return cells[ ix+iy*numCols ];
	}
    }
		
    const Cell& cellRef(int ix, int iy)const{
	return const_cast<Grid &>(*this).cellRef(ix, iy); //not a hack, just reusing code
    }

    Cell & cellRef( const Located &l){
	return cellRef( index_x(l.getPos().x), index_y(l.getPos().y) );
    }
    const Cell & cellRef( const Located &l) const{
	return const_cast<Grid&>(*this).cellRef( l );
    }
public:
    std::string toStr()const;
};

#endif
