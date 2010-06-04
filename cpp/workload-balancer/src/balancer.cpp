#include <cassert>
#include <algorithm>

#include <boost/thread/thread_time.hpp>

#include "simulated.hpp"
#include "balanced_worker.hpp"
#include "balancer.hpp"
#include "balance_method.hpp"

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
	workers.push_back( new BalancedWorker( *this ) );
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
void Balancer::setTimer( TimeType stopTime )
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
TimeType Balancer::averageTime()const
{
    //This code is written in order to correctly handle situations, when time counters are close to overflow.
    //It should work, if the gap between them is less than integer diapason
    assert( workers.size() > 0 );
    Workers::const_iterator i = workers.begin();
    TimeType t0 = (*i)->getTime();
    int sum_dt = 0; //can be positive and negative
    for( ++i; i != workers.end(); ++i ){
	int dt = static_cast<int>( (*i)->getTime() - t0 );
	sum_dt += dt;
    }
    return t0 + static_cast<TimeType>( sum_dt / workers.size() );
}

int Balancer::timeGap()const
{
    //This code is written in order to correctly handle situations, when time counters are close to overflow.
    //It should work, if the gap between them is less than integer diapason
    assert( workers.size() > 0 );
    Workers::const_iterator i = workers.begin();
    TimeType t0 = (*i)->getTime();
    int dt_min = 0;
    int dt_max = 0;
    for( ++i; i != workers.end(); ++i ){
	int dt = static_cast<int>( (*i)->getTime() - t0 );
	if ( dt < dt_min )     dt_min = dt;
	else if( dt > dt_max ) dt_max = dt;
    }
    return dt_max - dt_min;
}
void Balancer::setMethod( std::auto_ptr<BalanceMethod> method_ )
{
    assert( ! running );
    method = method_;
}
//Main loop of the load balancer thread.
void Balancer::balanceLoop()
{
    assert( method.get() );//method must be set
    int N = workers.size();
    assert( N > 0 );

    //Values of Ni and Ti on this sten and on previous step.
    TimesVector times0( N );
    SizesVector sizes0( N );
    TimesVector times1( N );
    SizesVector sizes1( N );

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
	    method->first( sizes0, times0 );
	}else{
	    //perform balancing
	    SizesVector & sizesNew = method->calculate( sizes0, times0, sizes1, times1 );
	    //and perform the task shuffling
	    rebalanceTasks( sizes1, sizesNew );
	}
	//store current values for the next step
	times1.swap( times0 );
	sizes1.swap( sizes0 );
	//do a pause before next iteratoin
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

void Balancer::rebalanceTasks( const SizesVector &oldSizes, const SizesVector &newSizes)
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
    for( size_t i = 0; i < oldSizes.size(); ++ i){
	workers[i]->requestRebalance( newSizes[i] - oldSizes[i] );
    };
}


////////////// Putter and getter ////////////////
Balancer::QueueAccessor::QueueAccessor( Balancer &parent_)
    :parent( parent_ )
{
    //acquire lock
    parent.taskPoolMutex.lock();
}
Balancer::QueueAccessor::~QueueAccessor()
{
    //release lock
    parent.taskPoolMutex.unlock();
}
void Balancer::QueuePutter::operator()( Simulated * task )
{
    assert( task );
    parent.taskPool.push_back( task );
}
bool Balancer::QueueGetter::empty()const
{
    return parent.taskPool.empty();
}
Simulated * Balancer::QueueGetter::operator()()
{
    if (!empty()){
	Simulated* rval = parent.taskPool.back();
	parent.taskPool.pop_back();
	return rval;
    }else{
	return NULL;
    }
}
