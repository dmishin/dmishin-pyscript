#include "sequential_simulator.hpp"

using namespace alife2;

////////////////// chunk ////////////////

TaskChunk::TaskChunk()
{
    refCount = 0;
    firstEmptySlot = 0;
    beginUnusedArea = 0;
    freeSlots = CHUNK_SIZE;
    FOR_RANGE( i, 0, CHUNK_SIZE ){
	tasks[ i ] = 0;
    }
}
void TaskChunk::addRef()
{
    refCount ++;
}
void TaskChunk::releaseRef()
{
    refCount --;
    if (refCount == 0){
	delete this;
    }
}

void TaskChunk::put( Simulated * task )
{
    assert( task );
    assert( ! isFull() );
    tasks[ lastUsedSlot ] = task;
    freeSlots --;
    if ( isFull() ){
	//slot became full, nothing to search
	return;
    }
    //slot is not full yet
    if ( firstEmptySlot == beginUnusedArea ){
	//it was no empty spaces
	firstEmptySlot ++;
	beginUnusedArea ++;
	return;
    }

    //There were empty spaces.
    //searching for the next empty space
    int i = lastUsedSlot + 1;
    while ( i < beginUnusedArea ){
	if ( tasks[ i ] == NULL ){//free slot found
	    firstEmptySlot = i;
	    return;
	}
	i++;
    }
    //free slot not found. Chun became compact
    firstEmptySlot = beginUnusedArea;
}
void TaskChunk::run()
{
    FOR_RANGE( i, 0, beginUnusedArea ){
	Simulated * task = tasks[i];
	if (task) task->simulate();
	//TODO: remove dead task
    }
}
void TaskChunk::remove( int index )
{
    assert( index >=0 && index < beginUnusedArea );
    assert( tasks[index] );
    
    tasks[ index ] = NULL;
    freeSlots ++;
    if ( index < firstEmptySlot ){
	firstEmptySlot = index;
    }
    if ( index+1 == beginUnusedArea ){
	//removed the last task in the used area.
	//Shrinking the area
	beginUnusedArea = index;
	while ( beginUnusedArea > firstEmptySlot && 
		tasks[beginUnusedArea] == NULL ){
	    beginUnusedArea --;
	}
    }
}
	
void TaskChunk::remove( Simulated* task)
{
    assert( task );
    FOR_RANGE( i, 0, beginUnusedArea){
	if (tasks[i] == task){
	    remove ( i );
	}
    }
    assert( false );
}

//////////////////////////// simulator /////////////////////


SequentialSimulator::Chunk SequentialSimulator::getNextChunk() //returns next chunk of the work;
{
    queue_lock lock( queueMutex );
    int max_items_left = queue.size() - nextTaskIndex;
    int items_in_chunk = min( max_items_left, chunkSize );
    Chunk rval( 
    nextTaskIndex += items_in_chunk;
    if (nextTaskIndex >= queue.size()){
	nextTaskIndex = 0;
	iterations += 1;
    }
    return 
}


SequentialSimulator::SequentialSimulator( int numThreads, int chunkSize_ )
{
    isRunning = false;
    stopRequested = false;
    nextTaskIndex = 0;
    iterations = 0;
    chunkSize = chunkSize_;
    FOR_RANGE( i, 0, numThreads ){
	workers.push_back( new Worker );
    };
}

SequentialSimulator::~SequentialSimulator()
{
    if (isRunning ){
	requestStop();
	waitAll();
    };
}
void SequentialSimulator::add( Simulated * item )
{
    queue_lock lock( mutex );
    queue.push_back( item );
}

void SequentialSimulator::simulate( int numIters )
{
    assert( ! isRunning );//can not run twice.
    stopRequested = false;
    isRunning = true;
    for( Workers::iterator i = workers.begin(); i!=workers.end(); ++i){
	*i->run();
    }
}

void SequentialSimulator::waitAll()//wait until all workers finish;
{
    for( Workers::iterator i = workers.begin(); i!=workers.end(); ++i){
	(*i)->thread.join();
    }
}

void SequentialSimulator::requestStop()//set the stop request flag;
{
    stopRequested = true;
}

