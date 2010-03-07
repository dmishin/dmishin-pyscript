#include "matrix_breeder.h"
#include "world.h"
#include "ftype.h"

MatrixBreeder::MatrixBreeder()
{
    minimalBots = 10;
};

void MatrixBreeder::onIdle( World &world)
{
    if (world.getNumBots() < minimalBots){
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
}
