#pragma once
#ifndef _SEQUENTIAL_SIMULATOR_H_
#define _SEQUENTIAL_SIMULATOR_H_
#include "simulated.hpp"
#include <boost/thread.hpp>

namespace alife2{
    class TaskChunk;
};

namespace boost{
    inline intrusive_ptr_add_ref( TaskChunk * ptr ){
	ptr->addRef();
    };
    inline intrusive_ptr_release_ref( TaskChunk * ptr ){
	ptr->releaseRef();
    };    
};


namespace alife2{
    /**Simulates the list of task as fast as it can, using the list of tasks*/
    static const int CHUNK_SIZE = 100;

    class TaskChunk{
    public:
	int refCount;
	int firstEmptySlot;
	int freeSlots;
	int beginUnusedArea;;
	Simulated* tasks[ CHUNK_SIZE ];

	TaskChunk();

	/**getting information*/
	bool isFull()const { return freeSlots == 0;};
	bool isEmpty()const{ return freeSlots == CHUNK_SIZE; };
	/**true, is there are no holes*/
	bool isCompact()const{ return firstEmptySlot == beginUnusedArea;};

	/**Modify tasks list*/
	void put( Simulated * task );
	void remove( int index );
	void remove( Simulated* task);
	/*Execute the tasks*/
	void run();
	/**Reference counting facilities*/
	void addRef();
	void releaseRef();
    };

    typedef boost::intrusive_ptr<TaskChunk> TaskChunkPtr;

    class SequentialSimulator{
    private:
	boost::mutex queueMutex;
	bool isRunning;
	bool stopRequested;
	int nextTaskIndex;

	typedef boost::lock<boost::mutex> queue_lock;

	int iterations;
	int chunkSize;

	typedef std::vector< TaskChunkPtr > Queue;

	Queue queue;

	class Worker{
	public:
	    SequentialSimulator * simulator;
	    void operator()();
	    boost::thread thread;
	    void run();
	};

	//Task queue operations;
	struct Chunk{
	    int begin, int end;
	};
	TaskChunkPtr getNextChunk(); //returns next chunk of the work;

	typedef std::vector< Worker* > Workers;
	Workers workers;
    public:
	
	SequentialSimulator( int numThreads, int chunkSize_ );
	~SequentialSimulator();
	void add( Simulated * item );
	void simulate( int numIters );
	void waitAll();//wait until all workers finish;
	void requestStop();//set the stop request flag;
    };
};


#endif /* _SEQUENTIAL_SIMULATOR_H_ */
