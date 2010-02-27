#include "random_brain.h"

RandomBrain::RandomBrain()
{
  t = 0;
}
void RandomBrain::simulate( Body &body, ftype dt)
{
  t+= dt;
  if (t>= SWITCH_TIME){
    t -= SWITCH_TIME;

  }
}
