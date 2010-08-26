#include "simulator.hpp"
#include "util.hpp"
#include <assert.h>
#include <iostream>

using namespace alife2;
using namespace boost;


//	class Worker{
Simulator::Worker::Worker( Simulator *owner_, int index_ )
{
    assert( owner_ );
    owner = owner_;
    iterations = 0;
    index = index_;
}
void Simulator::Worker::operator()()
{
    while (! owner->stopRequested ){
	Simulated* task = owner->getNextTask();
	if ( task ){
	    if ( task->simulate() ){
		owner->returnTask( task );
	    }
	}else{
	    //TODO: owner has no task: wait?
	}
	iterations += 1;
    }
}
void Simulator::Worker::run()
{
    //create ans start the thread
    thread = boost::thread( boost::ref(*this) ); //reference to the callable in the boost thread is copies, so use reference.
}
	
Simulator::Simulator( int nWorkers )
{
    isRunning = false;
    stopRequested = false;

    workers.reserve( nWorkers );//to avoid unneeded memory re-allocations
    FOR_RANGE( i, 0, nWorkers ){
	workers.push_back( new Worker( this, i ) );
    }
}

Simulator::~Simulator()
{
    if ( isRunning ) {
	stopRequested = true; //request stop and wait.
	waitAll();
    };
    for( Workers::iterator i = workers.begin(); i!= workers.end(); ++i){
	//TODO: stop thread
	delete *i;
	*i = NULL;
    }
    workers.empty();
}

void Simulator::add( Simulated * item )
{
    returnTask( item );
}
/**Create worker threads and run simulation for N steps*/
void Simulator::run( int numSteps )
{
    //for each worker, create a thread
    stopRequested = false;
    isRunning = true;
    for( Workers::iterator i = workers.begin(); i!=workers.end(); ++i ){
	(*i)->run();
    }
}

/**Wait until all workers finish*/
void Simulator::waitAll()
{
    if ( ! isRunning )
	return;//already finished, nothing to wait
    for( Workers::iterator i = workers.begin(); i!= workers.end(); ++i){
	(*i)->thread.join();
    };
    isRunning = false;
}
/**Request all simulators to stop*/
void Simulator::requestStop()
{
    stopRequested = true;
};


/**SYnchronized method for use by workers: get next free task.*/
Simulated* Simulator::getNextTask()
{
    QueueLockT lock ( getQueueMutex );
    if ( getQueue.empty() ){
	//there are no items to get. try to swap queues...
	QueueLockT ( putQueueMutex );
	if ( putQueue.empty() ){
	    //Both queues are empty - nothing to get.
	    return NULL;
	}else{
	    //perform queue swap
	    putQueue.swap( getQueue );
	}
    }
    Simulated* rval = getQueue.back();
    getQueue.pop_back();
    return rval;
}
/**Synchronized method for use by workers: return task to the queue*/
void Simulator::returnTask( Simulated * task )
{
    assert( task );
    //Puttig task to the put queue
    QueueLockT lock( putQueueMutex );
    putQueue.push_back( task );
}
