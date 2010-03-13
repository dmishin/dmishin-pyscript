#include "matrix_breeder.h"
#include "world.h"
#include "ftype.h"
#include "random_choise.h"
#include "matrix_brain.h"
#include "shared_pointers.h"
#include "parameters.h"
#include <iterator>

MatrixBreeder::MatrixBreeder()
{
    minimalBots = NUM_BOTS;
    numBrainStates = NUM_BRAIN_STATES;
    numBrainIntermediate = NUM_BRAIN_INTERMEDIATE;
};
MatrixBreeder::MatrixBreeder( int minBots )
{
    minimalBots = minBots;
    numBrainStates = NUM_BRAIN_STATES;
    numBrainIntermediate = NUM_BRAIN_INTERMEDIATE;
}

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
	ADD_ORPHAN, ADD_CLONE, ADD_CHILD, ADD_EXACT_CLONE
    };
    
    static RandomChoise choise;
    if (choise.empty()){
	choise.add( 1, ADD_ORPHAN );
	choise.add( 100, ADD_CLONE);
	choise.add( 30, ADD_EXACT_CLONE);
	choise.add( 1000, ADD_CHILD);
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
	case ADD_EXACT_CLONE:
	    if (createClone(w, false))
		break;
	    break;
	default:
	    assert( false );
    };
}

struct BotEnergyPred{
    ftype minEnergy;
    BotEnergyPred( ftype minE ):minEnergy( minE ){};
    bool operator()( MobilePtr bot )const{ 
	return bot->getEnergy()>=minEnergy && bot->getFoodEaten()>2; 
    };
};

bool MatrixBreeder::createClone(World& w, bool mutate)
{
    //random clone of the some bot
    //find at least one bot, with energy above some maximum.
    //TODO: get rid of memory allocation for arrays? Is it possible?

    ftype minimalCloneEnergy = MINIMAL_CLONE_ENERGY;
    std::vector<MobilePtr> foundBots;
    
    std::back_insert_iterator<std::vector<MobilePtr> > iter(foundBots);//otput iterator
    w.findBot( BotEnergyPred(minimalCloneEnergy), iter );
    if (foundBots.size() == 0){
	return false;//nobody to clone
    }
    //get a random one.
    int idx = rand()%foundBots.size();
    MobilePtr mob = foundBots[ idx ];
    
    //create its clone
    MobilePtr clone( new Mobile( mob->getPos(), frnd()*2*3.14 ) );
    MatrixBrain * brn = new MatrixBrain( mob->getNumSensors(), 
					 numBrainStates/*satates*/, 
					 numBrainIntermediate/*intermediate*/, 
					 mob->getNumMotors());
    //clone brain...
    brn->copy( static_cast<MatrixBrain&>(* mob->getBrain()) );
    //mutate
    if (mutate)
	brn->mutate();
    //put it to the world
    clone->setBrain( BrainPtr( brn ) );
    w.addMobile(clone);
    return true;
}
bool MatrixBreeder::createChild(World& w)
{
    //random clone of the some bot
    //find at least one bot, with energy above some maximum.
    //TODO: get rid of memory allocation for arrays? Is it possible?

    ftype minimalCloneEnergy = ftype(0.7);
    std::vector<MobilePtr> foundBots;
    
    std::back_insert_iterator<std::vector<MobilePtr> > iter(foundBots);//otput iterator
    w.findBot( BotEnergyPred(minimalCloneEnergy), iter );
    if (foundBots.size() < 2){
	return false;//nobody to breed
    }
    //get a random one.
    int idx1 = rand()%foundBots.size();
    int idx2 = rand()%foundBots.size();
    MobilePtr mob1 = foundBots[ idx1 ];
    MobilePtr mob2 = foundBots[ idx2 ];
    
    //create its clone
    MobilePtr child( new Mobile( mob1->getPos(), frnd()*2*3.14 ) );
    MatrixBrain * brn = new MatrixBrain( mob1->getNumSensors(), 
					 numBrainStates/*satates*/, 
					 numBrainIntermediate/*intermediate*/, 
					 mob1->getNumMotors());
    //clone brain...
    brn->makeChild( static_cast<MatrixBrain&>(* mob1->getBrain()),
		     static_cast<MatrixBrain&>(* mob2->getBrain()));
    //put it to the world
    child->setBrain( BrainPtr(brn) );
    w.addMobile(child);
    return true;
}
bool MatrixBreeder::createOrphan(World& w)
{
    //create new mobile at the center
    MobilePtr mob( new Mobile( vec2(frnd()*w.getSize().x,
				    frnd()*w.getSize().y ), 
			       frnd()* 2*3.1415 ));

    MatrixBrain * brn = new MatrixBrain( mob->getNumSensors(), 
					 numBrainStates/*satates*/, 
					 numBrainIntermediate/*intermediate*/, 
					 mob->getNumMotors());
    brn->randomInit();
    mob->setBrain( BrainPtr(brn) );
    w.addMobile( mob );
    return true;//always true;
}
