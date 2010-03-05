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

void makeRandomMatrix( ftype firstRowIntensity, ftype intensityDecrease, MatrixBrain::Matrix& m)
{
    ftype k = firstRowIntensity;
    int cols = m.size2();
    int rows = m.size1();
    for( int line = 0; line<rows; ++line){//iterate over all matrix lines
	for( int col = 0; col<cols; ++ col){
	    ftype range = k/cols;

	    m(line, col) = frnd( -range, range);
	}
	k = k*intensityDecrease;//next line would be 10% less intensive
    }
}
/**Completely random initialization*/
void MatrixBrain::randomInit()
{
    /*Initialize A matrix so that for input values around 1, 
      output would be around 10 for top values and would
      decrease by 10% for each next line
    */
    makeRandomMatrix( 10, 0.9, A);
    /*Initialize B matrix by the same rules as A
     */
    makeRandomMatrix( 10, 0.9, B);
    /*Set initial values for integrator*/
    resetState();
}
void MatrixBrain::resetState()
{
    for(int i = 0; i<Z.size(); ++i){
	Z[i] = 0;
    };
}
/**single mutation*/
void MatrixBrain::mutate()
{
    static RandomChoise mutationType;
    enum MutationType{
	DELETION, NOISE, PIN_NOISE
    };
	
    if (mutationType.empty()){//init mutation type chooser
	mutationType.add(0.1, DELETION);
	mutationType.add(0.5, NOISE);
	mutationType.add(0.2, PIN_NOISE);
    };
    switch( mutationType.get()){
	case DELETION:
	    //clear some random line
	case NOISE:
	    //add noise to matrices
	case PIN_NOISE:
	    //set random big values
	default:
	    assert( false );
    };
}
/**Create child from two parents*/
void MatrixBrain::makeChild(const MatrixBrain &a, const MatrixBrain &b)
{
}

