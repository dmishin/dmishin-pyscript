/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * alife-cpp
 * Copyright (C)  2010 <>
 * 
 * alife-cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * alife-cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "world.h"
#include <algorithm>

Mobile * World::findNearestMobile( const vec2& p, ftype maxDist)
{
	Grid::circular_generator gen(gridMobiles, p, maxDist);
	Located* best = 0;
	ftype bestD = maxDist;
	for ( Located* mob=0; gen( mob ); ){
		ftype d = dist( mob->getPos(), p);
		if (d<= bestD){
			bestD = d;
			best = mob;
		}
	}
	return static_cast<Mobile*>(best);
}
