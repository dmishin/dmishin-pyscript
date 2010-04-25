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
    public:
	//inherits:
	//position - from Located
	vec2 speed;
	rotation angle;
	float angleSpeed;
	float mass;

	World * world;

	Mobile();
	Mobile( const vec2 &pos, float angle );
	virtual ~Mobile();

	void setWorld( World* w);

	/**Implementation of the SImulated*/
	virtual void simulate();
	
	const vec2& getSpeed() const { return speed; };
	void setSpeed( const vec2& s ) { speed = s; };
	const rotation& getAngle() const { return angle; };
	void setAngle( float angle_ ) { angle = rotation( angle_ ) ; };
	float getAngleSpeed() const { return angleSpeed; };
	void setAngleSpeed( float aSpeed) { angleSpeed = aSpeed; };
	
	

    private:
	
    };
};


#endif /* _MOBILE_H_ */
