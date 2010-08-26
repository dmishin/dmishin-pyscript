#pragma once
#ifndef _MOBILE_H_
#define _MOBILE_H_
#include "located.hpp"
#include "grid_item.hpp"
#include "simulated.hpp"
#include "rotation.hpp"

namespace alife2{
    class World;
    class Mobile: public GridItem, public Simulated{
    private:
	/*Common initialization of miscelanneous parameters*/
	void initParameters();
	/**Simulation of the separate parts ob the mobile*/
	void simulateMotors();
	void simulateBrain();
	   
    public:
	//inherits:
	//position - from Located
	vec2 speed;       //movement speed
	rotation angle;   //rotation angle
	float angleSpeed; //rotation speed

	//Physical parameters
	float mass;
	float inertionMoment;

	//Back reference to thw World instance, the mobile belongs to.
	World * world;

	//COnstructing...
	Mobile();
	Mobile( const vec2 &pos, float angle );
	virtual ~Mobile();
	/**Implementation of the SImulated*/
	virtual bool simulate();
	
	//Mobile parameters access
	void setWorld( World* w);

	const vec2& getSpeed() const { return speed; };
	void setSpeed( const vec2& s ) { speed = s; };
	const rotation& getAngle() const { return angle; };
	void setAngle( float angle_ ) { angle = rotation( angle_ ) ; };
	float getAngleSpeed() const { return angleSpeed; };
	void setAngleSpeed( float aSpeed) { angleSpeed = aSpeed; };
    };
};


#endif /* _MOBILE_H_ */
