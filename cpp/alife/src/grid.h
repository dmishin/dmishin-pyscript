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
#include <pseudo_generator.hpp>

class ItemAcceptor;

class Located{
	protected:
		vec2 pos;
	public:
		explicit Located( const vec2& p ): pos(p){};
		Located (ftype x, ftype y):pos(x,y){};
		Located(){};

		vec2& getPos(){
			return pos;
		}
		const vec2& getPos()const{
			return pos;
		}
};

class Grid{
	private:
		ftype scale_x, scale_y;
		int num_cols, num_rows;
		ftype width, height;
		ftype cellWidth, cellHeight;

	public:
		class Cell{
		public:
			typedef std::set< Located * > CellItems;	
			CellItems items;
				
			void add( Located *item){
				items.insert( item );
			}
			void remove( Located * item){
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
		
		Grid(): cells(0) {};
		
		Cell unallocated;
		//typedef std::vector<Cell> Cells;
		//Cells cells;
		Cell* cells;

		void setGeometry( int cols, int rows, ftype width, ftype height);
		
		/**dimension of the grid*/
		vec2 size()const{ return vec2(width, height);};
		/**Center of the grid, in real points*/
		vec2 center()const{ return vec2(width/2, height/2);};

		
		void putItem( Located * item);
		void removeItem( Located * item);

		/**update positions of all Located items in cells*/
		void update();

		/**Utility class for enumerating items in a rectangle*/
		/**Pseudo-generator, returning items inside a rectangle*/
		struct rectangle_generator: public pseudo_generator<Located*>{
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
			Located* operator()();
		};
		typedef generator_iterator<rectangle_generator> rectangle_iterator;
		Grid::rectangle_iterator genItemsInRect(const vec2& p0, const vec2& p1){
			return rectangle_iterator(*this, p0.x, p0.y, p1.x, p1.y);
		}
		Grid::rectangle_iterator genItemsInRect(ftype x0, ftype y0, ftype x1, ftype y1){
			return rectangle_iterator(*this, x0, x1, y0, y1);
		};

		struct circular_generator: public pseudo_generator<Located*>{
			BEGIN_GENERATOR_STATES
				state1
			END_GENERATOR_STATES;
			vec2 center;
			ftype r;
			Grid* grid;
			//state
			ftype max_cell_radius;
			int ix0, ix1, iy0, iy1, ix, iy;
			Grid::Cell::CellItems::iterator iItem;
			Grid::Cell* curCell;
			
			circular_generator(){};//must be default-constructible
			circular_generator( Grid& g, const vec2& _center, ftype radius)
				:grid(&g), center(_center), r(radius){};

			Located* operator()();
		};
		typedef generator_iterator<circular_generator> circular_iterator;
		circular_iterator genItemsInCircle(const vec2& center, ftype r){
			return circular_iterator( *this, center, r);
		}
	private:
		void updateCell( Cell & cell);
		
		int index_x( ftype x )const{
			return (int)floor(x* scale_x);
		}
		int index_y( ftype y )const{
			return (int)floor(y* scale_y);
		}
		
		Cell& cellRef(int ix, int iy){
			if (ix<0 || iy<0 || ix>=num_cols || iy>=num_rows){
				return unallocated;
			}else{
				return cells[ ix*num_cols+iy ];
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
		void itemsInRect(ftype x0, ftype y0, ftype x1, ftype y1, ItemAcceptor & acceptor);
};

#endif