#include "grid.h"
#include <sstream>
#include <math.h>

void Grid::setGeometry( int cols, int rows, ftype width, ftype height)
{
	delete[] cells;

	num_cols = cols;
	num_rows = rows;
	this->width = width;
	this->height = height;
	//cells.resize( num_cols*num_rows );
	cells = new Cell[ num_cols*num_rows ];

	scale_x = num_cols/width;
	scale_y = num_rows/height;

	cellWidth = width/num_cols;
	cellHeight = height/num_rows;
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
	
	//Cells::iterator i=cells.begin(), e=cells.end();
	Cell * i = cells, *e = cells+num_cols*num_rows;
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
	os<<num_cols<<"X"<<num_rows<<";\n";
	for (int y = 0;y<num_rows; ++y){
		os<<"[ ";
		for(int x = 0; x<num_cols; ++x){
			os<<"["<<cellRef( x, y).items.size()<<"]";
		}
		os<<" ]\n";
	}
	os<<"}";
	return os.str();
}


#include "item-acceptor.h"
void Grid::itemsInRect(ftype x0, ftype y0, ftype x1, ftype y1, ItemAcceptor & acceptor)
{
	int ix0 = limit( (int)floor(x0*scale_x), 0, num_cols );
	int iy0 = limit( (int)floor(y0*scale_y), 0, num_rows );

	int ix1 = limit( (int)ceil(x1*scale_x), 0, num_cols );
	int iy1 = limit( (int)ceil(y1*scale_y), 0, num_rows );
	for (int y=iy0; y<=iy1;++y){
		for (int x=ix0; x<=ix1;++x){
			Cell & cell(cellRef(x,y));
			Cell::CellItems::iterator i, e=cell.items.end();
			for ( i = cell.items.begin(); i!=e; ++i){
				const vec2 & pos = (*i)->getPos();
				if (pos.x >= x0 && pos.x <= x1 && pos.y>=y0 && pos.y<=y1){
					acceptor.put( **i );
				}
			}
		}
	}
}


#include <iostream>

///Rectangle generator
Located* Grid::rectangle_generator::operator()()
{
	//Generator preamble
	BEGIN_RESTORE_STATE;
		RESTORE_STATE(state1)
	END_RESTORE_STATE;

	//Generator body
	BEGIN_GENERATOR;
	
	ix0 = limit( (int)floor(x0*grid->scale_x), 0, grid->num_cols );
	iy0 = limit( (int)floor(y0*grid->scale_y), 0, grid->num_rows );

	ix1 = limit( (int)ceil(x1*grid->scale_x), 0, grid->num_cols );
	iy1 = limit( (int)ceil(y1*grid->scale_y), 0, grid->num_rows );
	//note that local variable are not used, because we are inside pseudo-gnerator
	for (y=iy0; y<=iy1;++y){
		for (x=ix0; x<=ix1;++x){
			cur_cell = &(grid->cellRef(x,y));
			//for ( i = cur_cell->items.begin(); i!=cur_cell->items.end(); ++i){
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
	ix0 = limit( (int)floor((center.x-r)*grid->scale_x), 0, grid->num_cols );
	iy0 = limit( (int)floor((center.y-r)*grid->scale_y), 0, grid->num_rows );

	ix1 = limit( (int)ceil((center.x+r)*grid->scale_x), 0, grid->num_cols );
	iy1 = limit( (int)ceil((center.y+r)*grid->scale_y), 0, grid->num_rows );

	max_cell_radius = sqrt( sqr(grid->cellWidth)+ sqr(grid->cellHeight) )*0.5; //maxi
	
	//now enumerate all cells inside this rectangular area
	for ( iy = iy0; iy<iy1; ++iy ){
		for ( ix = ix0; ix < ix1; ++ix ){
			//determine, whether this cell can be displayed at all
			{
				ftype cx = (ix+ftype(0.5))*grid->cellWidth;//center of the cell
				ftype cy = (iy+ftype(0.5))*grid->cellHeight;
				//distance from the cell center to the circle center
				ftype d = sqrt(sqr(cx-center.x)+sqr(cy-center.y));
				if ( d > r + max_cell_radius )
					continue;//skip this cell - it is too far from center.
			}
			//iterate items inside cell
			curCell = &(grid->cellRef( ix, iy ));
			for ( iItem = curCell->items.begin(); iItem != curCell->items.end(); ++iItem){
				//determine, whether this located item lays inside the circle
				if ( dist((*iItem)->getPos(), center) <= r ){
					YIELD( *iItem, state1 );
				}
			}
		}
	}
	
	END_GENERATOR;
	return 0;//Dummy
}