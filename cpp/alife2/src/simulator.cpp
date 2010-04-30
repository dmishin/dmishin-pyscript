#include "simulator.hpp"
#include "util.hpp"
#include <assert.h>

using namespace alife2;
using namespace boost;


//	class Worker{
Simulator::Worker::Worker( Simulator *owner_ )
{
    assert( owner_ );
    owner = owner_;
}
void Simulator::Worker::add( Simulated * item )
{
    assert( item );
    assert( queue.find( item ) == queue.end() );
    queue.insert( item );
}
void Simulator::Worker::remove( Simulated * item )
{
    assert( item );
    int removed_cnt = queue.erase( item );
    assert( removed_cnt == 1 );
}
void Simulator::Worker::operator()()
{
    //TODO: simulation code
    while (! owner->stopRequested ){
	WorkQueue::iterator i = queue.begin();
	while ( i!= queue.end()){
	    (*i)->simulate();
	    ++i;
	}
    }
}
void Simulator::Worker::run()
{
    //create ans start the thread
    thread = boost::thread( *this );
}
	
Simulator::Simulator( int nWorkers, int simulateBy )
    :workers( nWorkers )
     
{
    stopRequested = false;
    lastWorkerIndex = 0;
    FOR_RANGE( i, 0, nWorkers ){
	workers.push_back( new Worker( this ) );
    }
}

Simulator::~Simulator()
{
    for( Workers::iterator i = workers.begin(); i!= workers.end(); ++i){
	//TODO: stop thread
	delete *i;
	*i = NULL;
    }
    workers.empty();
}

void Simulator::add( Simulated * item )
{
    assert( workers.size() >0 ); //add a task to the next worker
    lastWorkerIndex = (lastWorkerIndex + 1)%workers.size();
    workers[ lastWorkerIndex ] -> add( item );
}
/**Create worker threads and run simulation for N steps*/
void Simulator::run( int numSteps )
{
    //for each worker, create a thread
    stopRequested = false;
    for( Workers::iterator i = workers.begin(); i!=workers.end(); ++i ){
	(*i)->run();
    }
}

