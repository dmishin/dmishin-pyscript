#ifndef _ABSTRACT_SIMULATOR_H_
#define _ABSTRACT_SIMULATOR_H_
#include "mobile.h"

class World;
class AbstractSimulator
{
public:
    /**Called before simulation start*/
    virtual void prepareSimulation( World& w)=0;
    /**Called to inform that new bot entered the world*/
    virtual void onNewBot( MobilePtr mob )=0;
    /**Called to request simulation stop*/
    virtual void stopSimulation()=0;
};


#endif /* _ABSTRACT_SIMULATOR_H_ */
