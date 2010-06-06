#include <boost/bind.hpp>
#include <algorithm>
#include <cassert>
#include "concurrent_runner.hpp"
#include "simulated.hpp"


ConcurrentRunner::ConcurrentRunner( size_t numThreads )
{
    time = 0;
    stopRequested = false;
    running = false;
    numWorkers = numThreads;
    cleanupIntervalMs = 500;//clean-up every 500 ms
}
ConcurrentRunner::~ConcurrentRunner()
{
    if (running){
	stopThreads();
    }
}
 //add task to the queue
void ConcurrentRunner::add( Simulated * task )
{
    //locking the queue exclusively
    boost::unique_lock<boost::shared_mutex> lock( queueMutex );
    queue.push_back( Task( task ) ); //create new task and lock for it and push it back to the queue
}
//remove task from the queue (slow)
bool ConcurrentRunner::remove( Simulated * task)
{
    //locking the queue exclusively
    boost::lock_guard<boost::shared_mutex> lock( queueMutex );
    TaskQueue::iterator iItem = std::find_if( queue.begin(), queue.end(), 
					      boost::bind( &ConcurrentRunner::Task::equals, _1, task));
    if ( iItem != queue.end() ){
	remove( iItem );
	return true;
    }else
	return false;
}

//ConcurentWorker callbacks
void ConcurrentRunner::increaseTimer()
{
    //Todo: is locking needed?
    //todo: maybe, use atomic operations, if available?
    boost::lock_guard<boost::mutex> lock( runnerDataMutex );
    time ++;
}
    

//Create and start all threads
void ConcurrentRunner::runThreads( int N )
{
    assert( !running );
    assert( workers.size() == 0 );
    for ( int i = 0; i < N; ++i ){
	workers.push_back( new Worker( *this, i) );
    }
    //run all workers
    std::for_each( workers.begin(), workers.end(), 
		   boost::bind( &ConcurrentRunner::Worker::start, _1 ) );
    running = true;
}
//Send stop request to the threads and wait until all of them finish
void ConcurrentRunner::stopThreads()
{
    if (! running )
	return; //nothing to stop
    stopRequested = true; //set the stop requested flag and wait for all threads
    for( Workers::iterator i = workers.begin(); i!=workers.end(); ++i ){
	(*i)->thread.join();
	delete (*i);
	*i = NULL; //just to be sure...
    }
    workers.erase( workers.begin(), workers.end() );
    running = false;
}
//quick remove task by iterator, un-synced
void ConcurrentRunner::remove( TaskQueue::iterator iTask )
{
    //put the value from the last item to the removed one, and remove tha last
    *iTask = queue.back();
    queue.pop_back();
}

void ConcurrentRunner::removeDeadTasks()
{
    //gain read-write access to the queue
    boost::unique_lock<boost::shared_mutex> lock( queueMutex );
    TaskQueue::iterator iCurrent = queue.begin();
    TaskQueue::iterator iLast = queue.end(); --iLast;
    while ( iCurrent != iLast ){
	//go back until iLast point to some alive task. This task will be used as sumstiture for the empty task
	while ( iLast != iCurrent && (*iLast).task == NULL ){
	    iLast --;
	}
	//now iLast either points to some alive task, or points to dead and equal to iCurrent
	if ( iLast == iCurrent )
	    break;
	//now search for some empty task
	while ( iCurrent != iLast && iCurrent->task != NULL ){
	    ++ iCurrent;
	}
	if ( iCurrent == iLast )
	    break; //nothing to replace.
	//empty place inside and free task found
	*iCurrent = *iLast;
    }
    //Now iCurrent may either point to empty task or to alive task.
    if ( iCurrent->task != NULL ){
	++iCurrent;
    }
    //OK, iCurrent points to the new end.
    //Cut the queue.
    std::for_each( iCurrent, queue.end(), 
		   boost::bind( &ConcurrentRunner::Task::deleteMutex, _1 ) ); //delete mutexes for the deleted tasks
    queue.erase( iCurrent, queue.end() );
}

/**Runner runs this loop to clean up dead tasks from the queue periodically
 */
void ConcurrentRunner::cleanupLoop()
{
    while (! stopRequested ){
	//do some pause...
	boost::this_thread::sleep( boost::posix_time::milliseconds( cleanupIntervalMs ) );//sleep one second.
	//perform the cleanup action
	removeDeadTasks();
    }
}

void ConcurrentRunner::run()
{
    runThreads( numWorkers );
    cleanupLoop(); //loop is finished, when stop is requested
    stopThreads();
}
void ConcurrentRunner::requestStop()
{
    stopRequested = true;
}

/*********************
 * WOrker definitions
 **********************/
ConcurrentRunner::Worker::Worker( ConcurrentRunner & owner_, int index_ )
    :owner( owner_ ),
     index( index_ )
{
    assert( index >= 0 );
}
ConcurrentRunner::Worker::~Worker()
{
    if (running){
	thread.join();//wait for the end of the worker thread
    }
}
void ConcurrentRunner::Worker::mainLoop()
{
    assert( ! running );
    size_t N = index + 1;
    
    running = true;
    while (! owner.stopRequested ){
	//TODO count removed tasks
	//In the beginning of the cycle, gain read access to the queue
	boost::shared_lock<boost::shared_mutex> lockQueue( owner.queueMutex );
	for( size_t i0 = 0; i0 < N; ++i0 ){//i0 is the index of the first task
	    for ( size_t i = i0; i <= owner.queue.size(); i += N){
		Task & task = owner.queue[i];
		boost::lock_guard<boost::mutex> lockTask( *task.mutex );//acquire mutex of the individual task
		if (! task.task ) //task slot is empty - just skip it.
		    continue;
		bool do_continue = task.task -> simulate();
		if (! do_continue ){ //item finished its simulation.
		    task.task = NULL;
		}
	    }
	}
	owner.increaseTimer();
    }
    running = false;
}
void ConcurrentRunner::Worker::start()
{
    assert( ! running );
    //create thread and run main loop
    thread = boost::thread( 
	boost::bind( &ConcurrentRunner::Worker::mainLoop,
		     this ) );
}
 


/*****************************************
 *  Task
 *****************************************/
