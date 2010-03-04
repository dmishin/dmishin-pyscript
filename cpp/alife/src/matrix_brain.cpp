#include "matrix_brain.h"

#include <boost/numeric/ublas/matrix.hpp>

MatrixBrain::MatrixBrain( int nSensors, int nStates, int nIntermediate)
    :X(nSensors+nStates,1),
     A(nIntermediate, nSensors+nStates),
     B(nIntermediate, nStates)
{
}
void MatrixBrain::simulate( Body & mob, ftype dt)
{
    //put the sensor values to the X
    for (int i = 0; i<numSensors; ++i){
	X(i) = mob.getSensor( i );
    }
    //calculate intermediate veector
    Y = 
}
