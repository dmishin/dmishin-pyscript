#pragma once
#ifndef _GRID_H_
#define _GRID_H_
#include <vector>
#include "point.hpp"

namespace alife2{
    class Grid;
    class GridCell;

    /**Cell is one element of the grid*/
    class GridCell{
    public:
	//Data fields
	int  col, row; //position of the cell in the grid
	Grid * grid;//owner
	/**This method is used instead of constructor, because array of cells would be created*/
	void init( int col_, int row_, Grid* owner_ ){ 
	    col = col_; row = row_; grid = owner_;
	};
	    
    };
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

	GridCell & getCell( int col, int row );
	GridCell & getLastCell();
	
	/**Find the cell by the object coordinates*/
	GridCell & findCell( const vec2 & vec );
    public:
	/**Create cells*/
	void initGrid( float width, float height, float cellSize);
	int getNumCols()const{ return numCols; };
	int getNumRows()const{ return numRows; };
	vec2 getSize()const{ return vec2( width, height ); };
	vec2 getCenter()const{ return getSize() * 0.5f; };
	vec2 getCellSize()const{ return vec2( cellWidth, cellHeight ); };
    };
};

#endif /* _GRID_H_ */
