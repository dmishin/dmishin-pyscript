#pragma once
#ifndef __MATRIX_BRAIN_INCLUDED__
#define __MATRIX_BRAIN_INCLUDED__

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "brain.h"
#include "ftype.h"


class MatrixBrain
{
private:
    static ftype nlf( ftype x );
    static const ftype saturationLimit = 10;
public:
    typedef boost::numeric::ublas::matrix<ftype> Matrix;
    typedef boost::numeric::ublas::vector<ftype> Vector;
    int numSensors, numStates, numIntermediateValues, numOutputs;
    MatrixBrain( int nSensors, int nStates, int nIntermediate, int nOutputs);
    Vector X;//input signals+states
    Vector Y;//intermediate input signals
    Vector Z;//Output+state derivatives
    Matrix A;
    Matrix B;
    virtual void simulate( Body & mob, ftype dt);

    /**Completely random initialization*/
    void randomInit();
    /**single mutation*/
    void mutate();
    /**Create child from two parents*/
    void makeChild(const MatrixBrain &a, const MatrixBrain &b);
    /**Set integrator state to zero*/
    void resetState();
};

#endif
