#pragma once
#ifndef __MATRIX_BRAIN_INCLUDED__
#define __MATRIX_BRAIN_INCLUDED__

//disable some strange checks in the ublas
#define BOOST_UBLAS_NDEBUG

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "brain.h"
#include "ftype.h"


class MatrixBrain: public Brain
{
private:
    static ftype nlf( ftype x );
    static ftype outputFunction( ftype x );
    ftype saturationLimit;
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
    /**Create copy */
    void copy( const MatrixBrain &b ){ *this = b; reset(); };

    
    /**rreset internal state*/
    void reset();
private:
    void assertCompatible( const MatrixBrain &a);
    void mixParents( const MatrixBrain &a, const MatrixBrain &b);
    void blendParents( const MatrixBrain &a, const MatrixBrain &b, ftype k);

};

#endif
