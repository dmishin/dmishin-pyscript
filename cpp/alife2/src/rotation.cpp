#include "rotation.hpp"

namespace alife2{
    // Sum of rotations
    rotation operator + ( const rotation &a, const rotation& b ){
	return rotation( a.cos()*b.cos() - a.sin()*b.sin(),
			 a.sin()*b.cos() + a.cos()*b.sin() );
    };
    
    rotation operator - ( const rotation &a, const rotation& b){
	return rotation( a.cos()*b.cos() + a.sin()*b.sin(),
			 a.sin()*b.cos() - a.cos()*b.sin() );
    };
   
};
