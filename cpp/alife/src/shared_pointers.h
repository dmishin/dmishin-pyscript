#pragma once
#ifndef _SHARED_POINTERS_H_
#define _SHARED_POINTERS_H_
#include <boost/shared_ptr.hpp>

class Brain;
class Mobile;
class AbstractSimulator;

typedef boost::shared_ptr<Mobile> MobilePtr;
typedef boost::shared_ptr<Brain> BrainPtr;
typedef boost::shared_ptr<AbstractSimulator> SimulatorPtr;

#endif
