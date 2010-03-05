#pragma once
#ifndef __BREEDER_H_INCLUDED__
#define __BREEDER_H_INCLUDED__

/**Object, that is responsible for controlling world population
   It produces new children, when it is required
*/
class World;

class Breeder
{
public:
    /**Called from the simulator periodically*/
    virtual void onIdle( World& w) = 0;
};

#endif
