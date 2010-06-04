#include <cassert>

#include <algorithm>
#include <numeric> 
#include <functional>
#include <math.h>

#include <boost/bind.hpp>

#include "balance_method.hpp"

#define TRACE_BALANCER

#ifdef TRACE_BALANCER
#include <iostream>
#define TRACE( X ) {std::cout <<"[BAL]"<< X <<std::endl; std::cout.flush(); }
#else
#define TRACE( X ) ;
#endif
//////////////////////////////////////////////////////////////////////////////////
//Utilities
///
struct const_multiplier: public std::unary_function<double, double>
{
    double k;
    const_multiplier( double k_ ): k(k_){};
    double operator()( double x ){ return x*k; };
};

/**Given a set of floating point numbers such that their sum is integer,
   rounds each of them so that the sum became the same
*/
struct PairComparatorMore{
    typedef std::pair<double, int> arg_type;
    bool operator()( const arg_type &x0, const arg_type &x1 ){
	return x0.first > x1.first;
    }
};

/**Round several values, trying to make minimal change to their sum and to their individual values
   |sum(x)-sum(r)| < 0.5
   If the sum of the values is integer, sum of the output is the same as the sum of the input.
   elements are modified by minimal possible value
*/
void optimalRound( const std::vector<double> &x, std::vector<int> &r )
{
    typedef std::vector< std::pair<double, int> > indexed_vector;//vector of coubles with indices
    indexed_vector remainders(x.size()); //remainders and their indices
    assert( x.size() == r.size() );
    //step 1: round to the lower bound
    double sx = 0.0;
    int sr = 0.0;
    int N = x.size();
    for( int i = 0; i<N; ++i ){
	double xr = floor(x[i]);
	r[i] = static_cast<int>(xr);
	remainders[i] = std::make_pair( x[i] - xr, i );
	sx += x[i];
	sr += xr;
    }
    double defect = sx - sr; //how much is left
    //sort remainders and their indices to get the biggest remainders first
    std::sort( remainders.begin(), remainders.end(), PairComparatorMore() );
    //now add ones to the rounded values with highest remainders. When total defect become less than 0.5 by absolute value, then algorithm is stopped
    indexed_vector::const_iterator 
	i=remainders.begin(), e=remainders.end();
    while( fabs( defect ) > 0.5 && i != e ){
	r[ i->second ] ++; //second contains index of the element
	defect -= 1.0;
	++i;
    }
    assert( fabs( defect ) <= 0.5 );
}



//////////////////////////////////////////////////////////////////////////////////
///  BM_Simple
//////////////////////////////////////////////////////////////////////////////////
BM_Simple::BM_Simple( TimeType catchTime )
{
    catchTimeInterval = catchTime;
}
//First call
void BM_Simple::first( const SizesVector & N0, const TimesVector & T0)
{
    assert( N0.size() == T0.size() );
    updateSize( N0.size() );
}

//called to calculate balance
SizesVector & BM_Simple::calculate( const SizesVector &N0, const TimesVector T0, 
				    const SizesVector &N1, const TimesVector T1 )
{
    int N = N0.size();
    //perform balancing
    //calculate the Tmax - moment of time, when allworkers should hypotetically synchronize
    //It must be later. Find the last time
    TimeType Tmax = *std::max_element( T1.begin(), T1.end() );
    TimeType Tmin = *std::min_element( T1.begin(), T1.end() ); 
    
    TRACE( "Tmax="<<Tmax<<" Tmin="<<Tmin<<" Gap="<<(Tmax-Tmin) );
    //Now calculate T0. it must be after the T1 by some value, probably, several times more than difference between tmax and Tmin
    TimeType Tend = Tmax + catchTimeInterval;

    //Now calculate approximated throughput of every worker (non-normalized)
    // Q/T = (N1+N2)(T2-T1)/(T0 - T2)
    double sumQT = 0;//sum of the throughput values
    for( int i = 0; i<N; ++i){
	double Q_T = ((N0[i]+N1[i]) * static_cast<int>(T1[i]-T0[i]))/
	    static_cast<double>( Tend - T1[i] );
	throughput[i] = Q_T; // Q/T
	sumQT += Q_T;
    }

    int sumN = std::accumulate(N1.begin(), N1.end(), 0);
    if ( sumN > 0 && sumQT > 1e-12 ){ //TODO: eps? not really importan though.
	//Now each updated quantity N' can be calculated as:
	// N_i' = sumN * (QT[i] / sumQT)
	double k = sumN / sumQT;
	std::transform( throughput.begin(), throughput.end(), exactSizes.begin(), const_multiplier( k ) );
	optimalRound( exactSizes, N2 );
	//OK, new sizes calculated!
	//Now check the results.
	assert( std::accumulate( N2.begin(), N2.end(), 0) == sumN ); //except for the division by zero cases, it must be 0
    }

    return N2;
}

void BM_Simple::updateSize( size_t N )
{
    N2.resize( N );
    throughput.resize( N );
    exactSizes.resize( N );
}
