#pragma once
#ifndef _GRID_H_
#define _GRID_H_
#include <vector>
#include <set>
#include "point.hpp"
#include "grid_item.hpp"
#include "item_enumerator.hpp"
#include <string>

namespace alife2{
    class Grid;
    class GridCell;
    class GridItem;

    /**This class facilitates fast access to the object lists by coordinates
     * Idea: use hexagonal cells? This may reduce time for the seach, but does not really much.
     */
    class Grid{
    private:
	typedef std::vector< float > CellBorders;

	CellBorders horizontalCellBorders;
	CellBorders verticalCellBorders;

	float width, height;//real width and height of the grid;
	float cellWidth, cellHeight;
	int numCols, numRows;//number of cells

	typedef std::vector< GridCell > Cells;
	Cells cells; //cell data
	int population;

	GridCell & getCell( int col, int row );
	const GridCell & getCell( int col, int row )const{ return const_cast<Grid*>(this)->getCell( col, row ); };
	GridCell & getLastCell();
	const GridCell & getLastCell()const{ return const_cast<Grid*>(this)->getLastCell(); };
	
	/**Find the cell by the object coordinates*/
	GridCell & findCell( const vec2 & vec );
    public:
	Grid();
	Grid( float w, float h, float cellSize);

	/**Create cells*/
	void initGrid( float width, float height, float cellSize);
	int getNumCols()const{ return numCols; };
	int getNumRows()const{ return numRows; };
	vec2 getSize()const{ return vec2( width, height ); };
	vec2 getCenter()const{ return getSize() * 0.5f; };
	vec2 getCellSize()const{ return vec2( cellWidth, cellHeight ); };
	
	/**Updating items in cells*/
	void update();
	void putItem( GridItem * item );//Put item to the grid;
	void removeItem( GridItem * item );//Remove item from the grid;

	/**Getting information about items*/
	int getPopulation()const { return population; };

	/**Getting items in area*/
	int enumerateInRectangle( const rectangle & r, ItemEnumerator & enumr );
	int enumerateInCircle( const circle & c, ItemEnumerator & enumr );

	/**Utility functions*/
	std::string toString()const;
    };

    /**Cell is one element of the grid*/
    class GridCell{
    public:
	//Data fields
	int  col, row; //position of the cell in the grid
	Grid * grid;//owner

	//Grid items
	typedef std::set< GridItem* > Items;
	Items items;

	/**This method is used instead of constructor, because array of cells would be created*/
	void init( int col_, int row_, Grid* owner_ );
	void initOutside( Grid* owner_ );

	/**Outside cell is a special cell, used for storing items outside the main grid*/
	bool isOutisde()const{ return col == -1 || row == -1; };
	
	const Grid* getGrid()const{ return grid; };
	Grid* getGrid(){ return grid; };

	//Cell geometry
	vec2 getSize()const{ return grid->getCellSize(); };
	vec2 getCenter()const{ return getTopLeft() + 0.5f * getSize(); };
	vec2 getTopLeft()const{ 
	    vec2 size = grid->getCellSize();
	    return vec2( size.x*col, size.y*row );
	};
	vec2 getBottomRight() const{
	    vec2 size = grid->getCellSize();
	    return vec2( size.x*(col + 1), size.y*(row + 1) );
	};
	//Check, whether the cell contains the vector
	bool contains( const vec2 &v )const;

	//Operations on grid items
	bool hasItem( const GridItem * itm ) const;
	void addItem( GridItem * itm );//put item to the cell
	void removeItem( GridItem *itm );//remove item from grid
	int getPopulation()const{ return items.size(); };
    };
};

#endif /* _GRID_H_ */
