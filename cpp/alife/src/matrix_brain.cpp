#include "matrix_brain.h"
#include "body.h"
#include <boost/numeric/ublas/operation.hpp>
#include <algorithm>
#include "random_choise.h"
using namespace boost::numeric::ublas;


MatrixBrain::MatrixBrain( int nSensors, int nStates, int nIntermediate, int nOutputs)
    :X(nSensors+nStates),
     Y(nIntermediate),
     Z(nStates+nOutputs),
     A(nIntermediate, nSensors+nStates),
     B(nStates+nOutputs, nIntermediate)
{
    numSensors = nSensors;
    numStates= nStates;
    numIntermediateValues = nIntermediate ;
    numOutputs = nOutputs;
    saturationLimit = 10;
//    reset();
}
void MatrixBrain::reset()
{
    for( int i=0; i<numStates; ++i){
	int iX = i+numSensors;
	X[iX] = 0; //integrator state to zero
    }
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
	Y[i] = nlf( Y[i] + 0.1 );//removing symmetry
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
    for(int i = 0; i<numStates; ++i){
	X[i+numSensors] = 0;
    };
}
/**Matrix manipulation utilities*/
void clearRandomLine( MatrixBrain::Matrix & m)
{/**fills some random line with zeros*/
    int lineIdx = rand()%m.size1();
    for(unsigned int i =0; i<m.size2(); ++i){
	m(lineIdx, i) = 0;
    };
}

void addRelativeNoise( MatrixBrain::Matrix & m, ftype amount)
{/**add relative noise to the matrix*/
    //TODO: ineffective code...
    MatrixBrain::Matrix::iterator1 i,e = m.end1();
    for( i = m.begin1(); i!=e; ++i ){
	(*i) = (*i)*(1+frnd(-amount, amount));
    };
}

void addConstantNoise( MatrixBrain::Matrix & m, ftype amount)
{
   //TODO: ineffective code...
    MatrixBrain::Matrix::iterator1 i,e = m.end1();
    for( i = m.begin1(); i!=e; ++i ){
	(*i) = (*i) + frnd(-amount, amount);
    };
}
void addPinNoise( MatrixBrain::Matrix &m, ftype amount, int count = 1)
{
    assert( count >=0);
    int i,j;
    while( count ){
	i = rand()%m.size1();
	j = rand()%m.size2();
	m(i,j) = frnd( -amount, amount);
    }
}
/**single mutation*/
void MatrixBrain::mutate()
{
    const int RELATIVE_NOISE_AMOUNT = 0.1;
    const int CONSTANT_NOISE_AMOUNT = 0.01;
    const int PIN_NOISE_AMOUNT = 5;

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
	    //clear some random line from A and B
	    clearRandomLine( A );
	    clearRandomLine( B );
	    break;
	case NOISE:
	    //add noise to matrices
	    addRelativeNoise( A, frnd(0, RELATIVE_NOISE_AMOUNT));
	    addRelativeNoise( B, frnd(0, RELATIVE_NOISE_AMOUNT));
	    addConstantNoise( A, frnd(0, CONSTANT_NOISE_AMOUNT));
	    addConstantNoise( B, frnd(0, CONSTANT_NOISE_AMOUNT));
	    break;
	case PIN_NOISE:
	    //set random big values
	    addPinNoise( A, PIN_NOISE_AMOUNT);
	    addPinNoise( B, PIN_NOISE_AMOUNT);
	    break;
	default:
	    assert( false );
    };
}
/**Create child from two parents*/
void MatrixBrain::makeChild(const MatrixBrain &a, const MatrixBrain &b)
{
    static RandomChoise childChoise;
    enum ChildType{
	CHILD_BLEND, CHILD_MIX
    };
    if (childChoise.empty()){
	childChoise.add( 0.5, CHILD_BLEND);
	childChoise.add( 0.5, CHILD_MIX);
    };
    switch( childChoise.get() ){
	case CHILD_MIX:
	    //Merge matrices with random koefficient
	    mixParents( a, b);
	    break;
	case CHILD_BLEND:
	    //take some lines from one matrix and other lines from anouther.
	    blendParents( a, b, frnd(0, 1) );
	    break;
	default: assert( false );
    };
}

void MatrixBrain::blendParents( const MatrixBrain &a, const MatrixBrain &b, ftype k)
{
    std::fill(A.begin1(), A.end1(), 0);
    std::fill(B.begin1(), B.end1(), 0);
    A += a.A*(k);
    A += b.A*(1-k);
    B += a.B*(k);
    B += b.B*(1-k);
}

void MatrixBrain::assertCompatible( const MatrixBrain &a)
{
    assert(a.numSensors == numSensors);
    assert(a.numStates == numStates);
    assert(a.numIntermediateValues == numIntermediateValues);
    assert(a.numOutputs == numOutputs);
}
void mixMatrices( const MatrixBrain::Matrix& a, const MatrixBrain::Matrix &b, MatrixBrain::Matrix &c)
{
    for(unsigned int i  = 0; i < a.size1(); ++i){
	//decidem whether this row is got from the 
	const MatrixBrain::Matrix &useMatrix = *(rand()%2 ? &a : &b);
	row(c, i) = row( useMatrix, i);
    }
}
void MatrixBrain::mixParents( const MatrixBrain &a, const MatrixBrain &b)
{
    assertCompatible( a );
    assertCompatible( b );
    mixMatrices( a.A, b.A, A);
    mixMatrices( a.B, b.B, B);
}
