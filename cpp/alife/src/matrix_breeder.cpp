#include "matrix_breeder.h"
#include "world.h"
#include "ftype.h"
#include "random_choise.h"
#include "matrix_brain.h"

MatrixBreeder::MatrixBreeder()
{
    minimalBots = 100;
    numBrainStates = 10;
    numBrainIntermediate = 15;
};
#include <iostream>
void MatrixBreeder::onIdle( World &world)
{
    int numNewBots = minimalBots - world.getNumBots();
    while (numNewBots-- > 0){
	//it is time to make a new bot!
	makeNewBot( world );
    }
}

void MatrixBreeder::makeNewBot( World &w)
{
    //possibilities:
    // - add orphan
    // - add mutated clone
    // - add child of 2 parents
    enum OrphanType {
	ADD_ORPHAN, ADD_CLONE, ADD_CHILD
    };
    
    static RandomChoise choise;
    if (choise.empty()){
	choise.add( 0.1, ADD_ORPHAN );
	choise.add( 0.1, ADD_CLONE);
	choise.add( 0.8, ADD_CHILD);
    }
    //create a body...
    switch( choise.get() ){
	case ADD_CHILD:
	    //trying to add a child
	    if (createChild(w))
		break;
	    //continue: make a clone
	case ADD_CLONE:
	    if (createClone(w))
		break;
	    //continue to create an orphan
	case ADD_ORPHAN:
	    createOrphan(w);
	    break;
	default:
	    assert( false );
    };
}

bool MatrixBreeder::createClone(World& w)
{
    //random clone of the some bot
    //find at least one bot
    return false;
}
bool MatrixBreeder::createChild(World& w)
{
    return false;
}
bool MatrixBreeder::createOrphan(World& w)
{
    //create new mobile at the center
    MobilePtr mob( new Mobile( vec2( w.center() ), frnd()* 2*3.1415 ));

    MatrixBrain * brn = new MatrixBrain( mob->getNumSensors(), 
					 numBrainStates/*satates*/, 
					 numBrainIntermediate/*intermediate*/, 
					 mob->getNumMotors());
    brn->randomInit();
    mob->setBrain( *brn );
    w.addMobile( mob );
    return true;//always true;
}
