#include "matrix_brain.h"
#include "body.h"
#include <boost/numeric/ublas/operation.hpp>
using namespace boost::numeric::ublas;


MatrixBrain::MatrixBrain( int nSensors, int nStates, int nIntermediate, int nOutputs)
    :X(nSensors+nStates),
     Y(nIntermediate),
     Z(nStates+nOutputs),
     A(nIntermediate, nSensors+nStates),
     B(nStates+nOutputs, nIntermediate),
     numSensors( nSensors),
     numStates( nStates ),
     numIntermediateValues( nIntermediate ),
     numOutputs( nOutputs )
{
}
void MatrixBrain::simulate( Body & mob, ftype dt)
{
    //put the sensor values to the X
    for (int i = 0; i<numSensors; ++i){
	X[i] = mob.getSensor( i );
    }
    //calculate intermediate veector
    axpy_prod(A, X, Y, true); //Y = AX
    //nonlinear transofrm of a vector
    for(int i =0; i<numIntermediateValues; ++i){
	Y[i] = nlf( Y[i] );
    }
    //calculate output + state differential vector
    axpy_prod(B, Y, Z);
    //integrate the output derivatives with saturation
    for( int i=0; i<numStates; ++i){
	int iX = i+numSensors;
	int iZ = i+numOutputs;
	X[iX] = limit(X[iX]+dt*Z[iZ], -saturationLimit, +saturationLimit);//integrate with saturation
    }
    //write output values to the motors
    for( int i =0; i<numOutputs; ++i){
	mob.setMotor( i, Z[i]>0? 1 : 0 );//binary motor control
    }
}

/**Non-linear fucntion for neuron computations*/
ftype MatrixBrain::nlf( ftype x)
{
    if (x>0){
	return 1-(1/(1-x));
    }else{
	return -nlf(-x);
    }
}
