#include "grid.h"
#include <sstream>
#include <math.h>

Grid::Grid()
{
    //some default values, just to avoid uninitialized ones
    cells = NULL;
    size = vec2(0,0);
    cellSize = vec2(0,0);
    cellScale = vec2(1,1);
  
    numCols = 0;
    numRows = 0;

}
Grid::~Grid()
{
    delete[] cells;
    cells = 0;
}
void Grid::setGeometry( int cols, int rows, ftype width, ftype height)
{
    if (rows<=0 || cols <=0)
	throw std::logic_error("Bad grid size");
  
    size = vec2( width, height);

    delete[] cells;
    cells = new Cell[ rows* cols ];  
	  
    numCols = cols;
    numRows = rows;
		
    cellScale = vec2(numCols/width, numRows/height);
    cellSize = vec2(width/numCols, height/numRows);
}

void Grid::putItem( Located * item)
{
    const vec2 & location = item->getPos();
    Cell& cell = cellRef( index_x( location.x ), index_y( location.y ) );
    cell.add( item );
}

void Grid::removeItem( Located * item)
{
    const vec2 & location = item->getPos();
    Cell& cell = cellRef( index_x( location.x ), index_y( location.y ) );
    cell.remove( item );
}


void Grid::update()
{
    Cell * i = cells, *e = cells+numCols*numRows;
    for (;i!=e; ++i){
	updateCell( *i );
    }
    updateCell( unallocated );
}


/**Remove all items from the cell and put them back*/
void Grid::updateCell( Grid::Cell & cell)
{
    Grid::Cell::CellItems::iterator i=cell.items.begin(), e = cell.items.end();
    while( i!= e ){
	Cell & mustBelongTo = cellRef( **i );
	if (mustBelongTo != cell){//this item should belong to a different cell
	    Located* item = *i;
	    cell.items.erase( i++ ); //erase current item and got o the next one
	    mustBelongTo.add ( item );
	}else{
	    ++i;
	}
    }
}

std::string Grid::toStr()const
{
    std::ostringstream os;
    os<<"Grid:{"<<std::endl;
    os<<numCols<<"X"<<numRows<<";\n";
    for (int y = 0;y<numRows; ++y){
	os<<"[ ";
	for(int x = 0; x<numCols; ++x){
	    os<<"["<<cellRef( x, y).items.size()<<"]";
	}
	os<<" ]\n";
    }
    os<<"}";
    return os.str();
}
Located* Grid::findNearestItem( const vec2& p, ftype maxDist)
{
    Grid::circular_generator gen(*this, p, maxDist);
    Located* best = 0;
    ftype bestD = maxDist;
    for ( Located* mob=0; gen( mob ); ){
	ftype d = dist( mob->getPos(), p);
	if (d<= bestD){
	    bestD = d;
	    best = mob;
	}
    }
}

///Rectangle generator
Located* Grid::rectangle_generator::operator()()
{
    //Generator preamble
    BEGIN_RESTORE_STATE;
    RESTORE_STATE(state1);
    END_RESTORE_STATE;

    //Generator body
    BEGIN_GENERATOR;
	
    ix0 = limit( (int)floor(x0*grid->cellScale.x), 0, grid->numCols );
    iy0 = limit( (int)floor(y0*grid->cellScale.y), 0, grid->numRows );

    ix1 = limit( (int)ceil(x1*grid->cellScale.x), 0, grid->numCols );
    iy1 = limit( (int)ceil(y1*grid->cellScale.y), 0, grid->numRows );
    //note that local variable are not used, because we are inside pseudo-gnerator
    for (y=iy0; y<=iy1;++y){
	for (x=ix0; x<=ix1;++x){
	    cur_cell = &(grid->cellRef(x,y));
	    i = cur_cell->items.begin();
	    while ( i!=cur_cell->items.end() ){
		if ( in_rect((*i)->getPos(), x0, y0, x1, y1) ){
		    YIELD( *i, state1 );
		}
		++i;
	    }
	}
    }
    END_GENERATOR;
    return 0; //dummy return, just to exit the generator
}

//Circular generator
Located* Grid::circular_generator::operator()()
{
    //TODO Possible optimization: instead of skipping the cells that are too far,
    //     enumerate only cells that are near enough. This would require more 
    //     computations though.
    BEGIN_RESTORE_STATE;
    RESTORE_STATE( state1 );
    END_RESTORE_STATE;

    BEGIN_GENERATOR;
    //first determine rectangle, where to search for
    ix0 = limit( (int)floor((center.x-r)*grid->cellScale.x), 0, grid->numCols );
    iy0 = limit( (int)floor((center.y-r)*grid->cellScale.y), 0, grid->numRows );

    ix1 = limit( (int)ceil((center.x+r)*grid->cellScale.x), 0, grid->numCols );
    iy1 = limit( (int)ceil((center.y+r)*grid->cellScale.y), 0, grid->numRows );

    max_cell_radius = grid->cellSize.norm()*0.5; //maxi
	
    //now enumerate all cells inside this rectangular area
    for ( iy = iy0; iy<iy1; ++iy ){
	for ( ix = ix0; ix < ix1; ++ix ){
	    //determine, whether this cell can be displayed at all
		{
		    vec2 c((ix+ftype(0.5))*grid->cellSize.x,
			   (iy+ftype(0.5))*grid->cellSize.y);
		    //distance from the cell center to the circle center
		    ftype d = (c - center).norm();
		    if ( d > r + max_cell_radius )
			continue;//skip this cell - it is too far from center.
		}
		//iterate items inside cell
		curCell = &(grid->cellRef( ix, iy ));
		iItem = curCell->items.begin();
		iItemEnd = curCell->items.end();
		while (iItem != iItemEnd){
//		for ( iItem = curCell->items.begin(); iItem != curCell->items.end(); ++iItem){
		    //determine, whether this located item lays inside the circle
		    if ( dist((*iItem)->getPos(), center) <= r ){
			YIELD( *iItem, state1 );
		    }
		    ++iItem;
		}
	}
    }
	
    END_GENERATOR;
    return 0;//Dummy
}
