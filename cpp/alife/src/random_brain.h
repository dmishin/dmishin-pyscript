#pragma once
#ifndef __RANDOM_BRAIN_INCLUDED__
#define __RANDOM_BRAIN_INCLUDED__
#include "brain.h"
class RandomBrain: public Brain
{
private:
  ftype t;
public:
  static const ftype SWITCH_TIME=5;
  RandomBrain();
  virtual ~RandomBrain(){};
  virtual void simulate( Body& body, ftype dt);
private:
    /*called to set random motor*/
    void randomSet( Body& b );
};
#endif
