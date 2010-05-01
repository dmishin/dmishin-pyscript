#pragma once
#ifndef _WORLD_H_
#define _WORLD_H_
//#include "grid.hpp"
#include "grid_mt.hpp"
#include "simulated.hpp"
#include "point.hpp"
#include "ticker.hpp"


namespace alife2{
    class Mobile;
    class Food;

    class World: public Simulated{
	float timeStep; //simulation step
	float friction; //movement friction
	float rotationFriction; //friciton koefficient for rotation

	rectangle bounds;


	ticker gridMobilesTicker;
	ticker gridFoodTicker;

	//Grid initialization code
	void initGrids( float width, float height, float cellSize );
	void initParameters();

    public:
	GridMt gridMobiles;
	GridMt gridFood;
	
	World( float width, float height, float cellSize);
	//Get the walue of the simulation time step
	float getSimulationStep()const { return timeStep; };
	float getFriction()const {return friction; };
	float getRotFriction() const { return rotationFriction; };

	

	//world geometry
	const rectangle& getBounds()const {return bounds; };
	vec2 getCenter()const { return bounds.center(); };

	void add( Mobile * mobile );
	void add( Food* food );

	//Implementation of the Simulated
	virtual void simulate();

	//Apply world geometry to the mobile: force it to stay inside the world bounds
	void applyBounds( Mobile & mob );
    };
};


#endif /* _WORLD_H_ */
