#ifndef _ROTATION_H_
#define _ROTATION_H_
#include "point.hpp"

namespace alife2{
    class rotation;
    rotation operator + ( const rotation &a, const rotation& b );
    rotation operator - ( const rotation &a, const rotation& b);

    class rotation{      
    public:
	vec2 sincos;
	//Basic constructors
	rotation( float alpha ):sincos( cosf( alpha ), sinf( alpha ) ){};
	rotation ( float cos, float sin): sincos(cos, sin) {};
	rotation():sincos(1,0) {};
	//Fast constructor for small rotations
	static rotation smallAngle( float smallAngle ){
	    return rotation( 1-sqr(smallAngle) * 0.5f, smallAngle );
	}
	
      
	//Retrieve angle information
	float angle()const{ return atan2f( sincos.x, sincos.y ); };
	float sin()const{ return sincos.y; };
	float cos()const{ return sincos.x; };

	//inverse rotation
	rotation inv()const{ return rotation( cos(), -sin() ); };
	rotation operator -()const{ return inv(); };

	//normalization
	void normalize(){
	    float norm = sincos.len();
	    if (norm != 0){
		sincos *= (1/norm);
	    }else{
		sincos = vec2( 1, 0);
	    }
	};

	//Small addition
	rotation & addSmall( float angle ){
	    (*this) = (*this) + smallAngle( angle );
	};
         
	rotation operator += (const rotation &r){ (*this) = (*this) + r; };
	rotation operator -= (const rotation &r){ (*this) = (*this) - r; };

	//Apply rotation to the vector
	vec2 apply( const vec2 &v )const{
	    return vec2( v.x*cos()-v.y*sin(), v.y*cos() + v.x*sin() );
	};
	//Apply rotation in the different direction
	vec2 applyBack( const vec2 &v )const{
	    return vec2( v.x*cos() + v.y*sin(), v.y*cos() - v.x*sin() );
	}; 
    };
 

};

#endif /* _ROTATION_H_ */
