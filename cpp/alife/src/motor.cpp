#include "motor.h"

Motor::Motor( const vec2& pos, const vec2& dir)
  :Located( pos ),
   direction( dir ),
   force ( 0 )
{
}

