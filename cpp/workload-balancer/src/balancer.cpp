#include <cassert>
#include "simulated.hpp"
#include "balanced_worker.hpp"
#include "balancer.hpp"

Balancer::Balancer( int numWorkers )
{
    running = false;
    stopRequested = false;
    createWorkers( numWorkers );
}

Balancer::~Balancer()
{
    if (isRunning() ){
	requestStop();
	waitAll();
    }
    for (Workers::iterator i = workers.begin(); i != workers.end(); ++i ){
	delete *i;
	*i = NULL;
    }
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
}

void Balancer::waitAll()
{
    assert( running );
    for( Workers::iterator i = workers.begin(); i < workers.end(); ++ i){
	//TODO
	//(*i)->thread.join();
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
    
//Retuen mean time value for all workers (each worker has its own time)
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
//Main loop of the load balancer thread.
void Balancer::balanceLoop()
{
    running = true;
    while( stopRequested ){
	//store times (and counts?)
	//wait a bit
	//read new times.
	//calculate speeds
	//calculate balanced counts
	//issue task exchange commands to the workers
    };
    waitAll();
    running = false;
}
