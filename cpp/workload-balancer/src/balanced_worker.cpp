#include <cassert>
#include <algorithm>
#include "balanced_worker.hpp"
#include "simulated.hpp"

BalancedWorker::BalancedWorker()
{
    time = 0;
    stopTime = 0;
    timerSet = false; //if true, simulation will stop, when time reaches stopTime
    stopRequested = false; //set to true to finish the loop
}

BalancedWorker::~BalancedWorker()
{
}

//main loop
BalancedWorker::TimeType BalancedWorker::run()
{
    TimeType startTime = time;
    while( true ){
	if ( stopRequested || timerSet && time == stopTime )
	    break;
	simulate();
	time ++;
    }
    return time - startTime;
}

//Perform one step of all tasks in the queue
void BalancedWorker::simulate()
{
    for( Queue::iterator i = queue.begin(); i != queue.end(); ++i){
	if ( ! (*i)->simulate() ){
	    //This item has finished its life
	    remove( i );
	}
    }
}

//Remove one element from the queue 
//By replacng it with the last one and shrinking the queue.
void BalancedWorker::remove( BalancedWorker::Queue::iterator pos )
{
    *pos = *queue.rbegin(); //put the last element here
    queue.pop_back();
}

void BalancedWorker::add( Simulated* task )        //Add task to the queue (synchronous)
{
    assert( task );
    //TODO: lock
    queue.push_back( task );
}

bool BalancedWorker::remove( Simulated * task )    //Slow method for manual task removal. True if successes. synchronous.
{
    assert( task );
    //TODO: lock
    Queue::iterator pos = std::find( queue.begin(), queue.end(), task );
    if ( pos == queue.end() )
	return false;
    remove( pos );
    return true;
}
