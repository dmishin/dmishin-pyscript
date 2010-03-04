#pragma once
#ifndef __MATRIX_BRAIN_INCLUDED__
#defien __MATRIX_BRAIN_INCLUDED__

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "brain.h"


class MatrixBrain
{
public:
    typedef boost::numeric::ublas::matrix<ftype> Matrix;
    typedef boost::numeric::ublas::vector<ftype> Vector;
    int numSensors, numStates, numIntermediateValues;
    MatrixBrain( int nSensors, int nStates, int nIntermediate);
    Vector X;//input signals+states
    Vector Y;//intermediate input signals
    Matrix A;
    Matrix B;
    virtual void simulate( Body & mob, ftype dt);
};

#endif
