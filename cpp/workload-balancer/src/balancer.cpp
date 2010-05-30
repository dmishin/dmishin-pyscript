#include <cassert>
#include <math.h>
#include <boost/bind.hpp>
#include <boost/thread/thread_time.hpp>
#include <algorithm>
#include <numeric> 
#include "simulated.hpp"
#include "balanced_worker.hpp"
#include "balancer.hpp"

#define TRACE_BALANCER

#ifdef TRACE_BALANCER
#include <iostream>
#include <iomanip>
#define TRACE( x ) { std::cout << "BLR:" << x << std::endl; std::cout.flush(); }
#else
#define TRACE( x ) {}
#endif

Balancer::Balancer( int numWorkers )
{
    running = false;
    stopRequested = false;
    createWorkers( numWorkers );
    reachingTimeInterval = 1000;//workers should converge in 1000 steps
    rebalanceIntervalMs = 1000;
    TRACE( "Started balancer" );
}

Balancer::~Balancer()
{
    if (isRunning() ){
	TRACE( "Balancer is running, waiting for all threads to stop..." );
	requestStop();
	waitAll();
	TRACE( "Done." );
    }
    for (Workers::iterator i = workers.begin(); i != workers.end(); ++i ){
	delete *i;
	*i = NULL;
    }
    TRACE( "Balancer destroyed" );
}

void Balancer::createWorkers( int numWorkers )
{
    assert( workers.size() == 0 );
    assert( numWorkers > 0 );
    for( int i = 0; i < numWorkers; ++i ){
	workers.push_back( new BalancedWorker() );
    }
}

void Balancer::requestStop()
{
    assert( running );
    for( Workers::iterator i = workers.begin(); i < workers.end(); ++ i){
	(*i)->requestStop();
    }
    stopRequested = true;
    TRACE( "Stop requested" );
}

void Balancer::waitAll()
{
    assert( running );
    for( Workers::iterator i = workers.begin(); i < workers.end(); ++ i){
	(*i)->wait();
    }
}

//Stop timer manipulation
void Balancer::setTimer( BalancedWorker::TimeType stopTime )
{
    for( Workers::iterator i = workers.begin(); i < workers.end(); ++ i){
	(*i)->setTimer( stopTime );
    }
}
void Balancer::disableTimer()
{
    for( Workers::iterator i = workers.begin(); i < workers.end(); ++ i){
	(*i)->disableTimer();
    }
}
    
//Return mean time value for all workers (each worker has its own time)
BalancedWorker::TimeType Balancer::averageTime()const
{
    //This code is written in order to correctly handle situations, when time counters are close to overflow.
    //It should work, if the gap between them is less than integer diapason
    assert( workers.size() > 0 );
    Workers::const_iterator i = workers.begin();
    BalancedWorker::TimeType t0 = (*i)->getTime();
    int sum_dt = 0; //can be positive and negative
    for( ++i; i != workers.end(); ++i ){
	int dt = static_cast<int>( (*i)->getTime() - t0 );
	sum_dt += dt;
    }
    return t0 + static_cast<BalancedWorker::TimeType>( sum_dt / workers.size() );
}

int Balancer::timeGap()const
{
    //This code is written in order to correctly handle situations, when time counters are close to overflow.
    //It should work, if the gap between them is less than integer diapason
    assert( workers.size() > 0 );
    Workers::const_iterator i = workers.begin();
    BalancedWorker::TimeType t0 = (*i)->getTime();
    int dt_min = 0;
    int dt_max = 0;
    for( ++i; i != workers.end(); ++i ){
	int dt = static_cast<int>( (*i)->getTime() - t0 );
	if ( dt < dt_min )     dt_min = dt;
	else if( dt > dt_max ) dt_max = dt;
    }
    return dt_max - dt_min;
}

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
//Main loop of the load balancer thread.
void Balancer::balanceLoop()
{
    typedef std::vector<BalancedWorker::TimeType> TimesVector;
    int N = workers.size();
    assert( N > 0 );

    //Values of Ni and Ti on this sten and on previous step.
    TimesVector times0( N );
    SizesVector sizes0( N );
    TimesVector times1( N );
    SizesVector sizes1( N );

    std::vector< double > throughput( N );
    std::vector< double > exactSizes( N );
    SizesVector sizesNew( N );//new sizes would be stored here

    bool isFirstStep = true;

    while( !stopRequested ){
	TRACE( "iterate...");
	//store times (and counts?)
	//wait a bit
	//read new times.
	//calculate speeds
	//calculate balanced counts
	//issue task exchange commands to the workers
	// --- //
	//record sizes and times for this step
	std::transform( workers.begin(), workers.end(), times1.begin(), boost::bind( &BalancedWorker::getTime, _1 ) );
	std::transform( workers.begin(), workers.end(), sizes1.begin(), boost::bind( &BalancedWorker::size, _1 ) );

	if( isFirstStep ){
	    isFirstStep = false;
	    TRACE( "Simulation step #1" );
	}else{
	    //perform balancing
	    //calculate the Tmax - moment of time, when allworkers should hypotetically synchronize
	    //It must be later. Find the last time
	    BalancedWorker::TimeType Tmax = *std::max_element( times1.begin(), times1.end() );
	    BalancedWorker::TimeType Tmin = *std::min_element( times1.begin(), times1.end() );
	    TRACE( "Tmax="<<Tmax<<" Tmin="<<Tmin<<" Gap="<<(Tmax-Tmin) );
	    //Now calculate T0. it must be after the T1 by some value, probably, several times more than difference between tmax and Tmin
	    BalancedWorker::TimeType T0 = Tmax + reachingTimeInterval;

	    //Now calculate approximated throughput of every worker (non-normalized)
	    // Q/T = (N1+N2)(T2-T1)/(T0 - T2)
	    double sumQT = 0;//sum of the throughput values
	    for( int i = 0; i<N; ++i){
		double Q_T = ((sizes0[i]+sizes1[i]) * static_cast<int>(times1[i]-times0[i]))/
		    static_cast<double>( T0 - times1[i] );
		throughput[i] = Q_T; // Q/T
		sumQT += Q_T;
	    }
	    int sumN = std::accumulate(sizes1.begin(), sizes1.end(), 0);
	    if ( sumN > 0 && sumQT > 1e-12 ){ //TODO: eps? not really importan though.
		//Now each updated quantity N' can be calculated as:
		// N_i' = sumN * (QT[i] / sumQT)
		double k = sumN / sumQT;
		std::transform( throughput.begin(), throughput.end(), exactSizes.begin(), const_multiplier( k ) );
		optimalRound( exactSizes, sizesNew );
		//OK, new sizes calculated!
		//Now check the results.
#ifdef TRACE_BALANCER
		int sumN1 = std::accumulate( sizesNew.begin(), sizesNew.end(), 0);
		if ( sumN1 != sumN ){//tracing
		    TRACE("LOST TASKS:");
		    TRACE("Catch time:"<<T0);
		    for (int i = 0; i < N; ++ i){
			TRACE( "#"<<i<<"T0="<<times0[i]<<" T1="<<times1[i]<<" N0="<<sizes0[i]<<" N1="<<sizes1[i]<<" Q/T="<<throughput[i] );
		    }
		    TRACE("calculated new sizes:");
		    for (int i = 0; i < N; ++ i){
			TRACE("#"<<i<<" Nnew="<<sizesNew[i] );
		    }
		    TRACE("Defect sum(Nnew)-sum(N) = "<<sumN1-sumN);
		}
#endif
		assert( std::accumulate( sizesNew.begin(), sizesNew.end(), 0) == sumN ); //except for the division by zero cases, it must be 0
		
		//and perform the task shuffling
		rebalanceTasks( sizes1, sizesNew );
	    }
	}
	//store current values for the next step
	times1.swap( times0 );
	sizes1.swap( sizes0 );
	//do a pause before next iteratoin
	//boost::thread::sleep(boost::posix_time::milliseconds( 1000 ) );//sleep one second.
	boost::this_thread::sleep( boost::posix_time::milliseconds( rebalanceIntervalMs ) );//sleep one second.
    };
    TRACE( "End loop" );
}
void Balancer::start()
{
    runWorkers();
    running = true;

    balanceLoop();

    waitAll();
    running = false;
}
void Balancer::runWorkers()
{
    for( Workers::iterator i = workers.begin(); i < workers.end(); ++ i){
	(*i)->runThread();
    }
}
//Performs the tasks shuffing, according to the calculated values.
#ifdef TRACE_BALANCER
#include <sstream>
#endif
void Balancer::rebalanceTasks( const Balancer::SizesVector &oldSizes, const Balancer::SizesVector &newSizes)
{
#ifdef TRACE_BALANCER
    std::stringstream ss;
    ss<<"Rebalance:";
    for( size_t i = 0; i < oldSizes.size(); ++ i){
	if (i != 0 ) ss<<",";
	ss<<"["<<oldSizes[i]<<"-->"<<newSizes[i]<<"]";
    }
    TRACE( ss.str() );
#endif
    //TODO
}