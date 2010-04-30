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
void Simulator::Worker::add( Simulated * item )
{
    assert( item );
//    std::cout<<"Add item to the worker"<<index<<std::endl; std::cout.flush();
    boost::mutex::scoped_lock lock( queueAccessMutex );
    assert( queue.find( item ) == queue.end() );
    queue.insert( item );
}
void Simulator::Worker::remove( Simulated * item )
{
    assert( item );
    boost::mutex::scoped_lock lock( queueAccessMutex );
    int removed_cnt = queue.erase( item );
    assert( removed_cnt == 1 );
}
void Simulator::Worker::operator()()
{
    while (! owner->stopRequested ){
	//TODO: repeat simulation round several times?
	//TODO: load balancing.

	//ensure that while simulating, nobody will corrupt the queue
	{
	    boost::mutex::scoped_lock lock( queueAccessMutex );
	    FOR_RANGE( i, 0, owner->simulationChunkSteps ){
		WorkQueue::iterator i = queue.begin();
		while ( i!= queue.end()){
		    (*i)->simulate();
		    ++i;
		}
	    }
	}
	iterations += 1;
	//After performing the simulation, wait for other threads on the barrier
	owner->simulationSyncBarrier.wait();
    }
}
void Simulator::Worker::run()
{
    //create ans start the thread
    thread = boost::thread( boost::ref(*this) ); //reference to the callable in the boost thread is copies, so use reference.
}
	
Simulator::Simulator( int nWorkers, int simulateBy )
    :simulationSyncBarrier( nWorkers) //number of threads, that would be waiting on barrier
     
{
    isRunning = false;
    stopRequested = false;
    lastWorkerIndex = 0;
    simulationChunkSteps = simulateBy;

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
//    std::cout<<"Add "<<long(item)<<" num workers:"<<workers.size()<<" worker idx="<<lastWorkerIndex<<std::endl;
    assert( workers.size() > 0 ); //add a task to the next worker
    lastWorkerIndex = (lastWorkerIndex + 1)%workers.size();
    assert( lastWorkerIndex >= 0);
    Worker & worker = *workers[ lastWorkerIndex ];
    worker.add( item );
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

