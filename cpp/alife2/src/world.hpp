#pragma once
#ifndef _WORLD_H_
#define _WORLD_H_
#include "grid.hpp"
#include "simulated.hpp"
#include "point.hpp"

namespace alife2{
    class Mobile;
    class Food;

    class World: public Simulated{
	Grid gridMobiles;
	Grid gridFood;
	float timeStep; //simulation step
	float friction; //movement friction
	float rotationFriction; //friciton koefficient for rotation

	rectangle bounds;

	//Grid initialization code
	void initGrids( float width, float height, float cellSize );
	void initParameters();

    public:
	World();
	//Get the walue of the simulation time step
	float getSimulationStep()const { return timeStep; };
	float getFriction()const {return friction; };
	float getRotFriction() const { return rotationFriction; };

	//world geometry
	const rectangle& getBounds()const {return bounds; };

	void add( Mobile * mobile );
	void add( Food* food );

	//Implementation of the Simulated
	virtual void simulate();

	//Apply world geometry to the mobile: force it to stay inside the world bounds
	void applyBounds( Mobile & mob );
    };
};


#endif /* _WORLD_H_ */
