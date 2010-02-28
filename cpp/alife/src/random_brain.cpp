#include "random_brain.h"
#include "body.h"
#include <stdlib.h>
#include <limits.h>

RandomBrain::RandomBrain()
{
  t = 0;
}
void RandomBrain::simulate( Body &body, ftype dt)
{
  t+= dt;
  if (t>= SWITCH_TIME){
    t -= SWITCH_TIME;
    randomSet( body );
  }
}

void RandomBrain::randomSet( Body& b )
{
    int numMotor = b.getNumMotors();
    for (int i = 0; i<numMotor; ++i){
	b.setMotor( i, 0);
    }
    int motorIdx = (int)floor(rand()/double(INT_MAX)*numMotor);
    b.setMotor( motorIdx, 1 );
}
