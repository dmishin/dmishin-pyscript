#include <cassert>
#include <algorithm>
#include <boost/bind.hpp>
#include "balanced_worker.hpp"
#include "simulated.hpp"
#include "balancer.hpp"

BalancedWorker::BalancedWorker( Balancer & parent_ )
    :parent( parent_ )
{
    time = 0;
    stopTime = 0;
    timerSet = false; //if true, simulation will stop, when time reaches stopTime
    stopRequested = false; //set to true to finish the loop
    requestedRebalancing = 0;//by default - no rebalancing is needed
}

BalancedWorker::~BalancedWorker()
{
}

//main loop
TimeType BalancedWorker::run()
{
    TimeType startTime = time;
    stopRequested = false;
    while( true ){
	if ( stopRequested || (timerSet && (time == stopTime) ) )
	    break;
	simulate();
	time ++;
	//check, whether there are any rebalance requests
	updateBalance();
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

/**Pop one task from the queue. Probably, popping random task may be effective, but I am not sure yet.
 */
Simulated * BalancedWorker::popOneTask()
{
    if (! empty() ){
	Simulated * rval = queue.back();
	queue.pop_back();
	return rval;
    }else{
	return NULL;
    }
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

//////////////// Thread-related machinery ////////////////
/**Create thread, executing the main loop
 */
void BalancedWorker::runThread()
{
    boost::thread thrd( boost::bind( &BalancedWorker::run, this ) );
    workerThread.swap( thrd ); //move-assignment is not working bu some strange reason. OK, let's be explicit.
}
/**Wait, until thread finish its work
   It may be good idea to request stop before calling this, or waiting can become infinite.
*/
void BalancedWorker::wait()
{
    workerThread.join();
}

void BalancedWorker::updateBalance()
{
    int request = requestedRebalancing;
    requestedRebalancing = 0;

    if ( request > 0){
	//get additional tasks
	Balancer::QueueGetter getter = parent.getter();
	for( ; request > 0 && !getter.empty(); --request ){
	    add( getter() );
	}
    }else if( request < 0){
	//return some tasks
	Balancer::QueuePutter putter = parent.putter();
	for( ; request < 0 && !empty(); ++request ){
	    putter( popOneTask() );
	}
    }
}	
       
void BalancedWorker::requestRebalance( int workloadChange )
{
    requestedRebalancing = workloadChange;
}
