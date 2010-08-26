#pragma once
#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_
#include "simulated.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <set>
#include <vector>
#include <deque>

namespace alife2{
    class Simulator{
    private:
	int lastWorkerIndex;
	volatile bool stopRequested;
	volatile bool isRunning;

    public:
	class Worker;
	typedef std::vector< Worker* > Workers;
	Workers workers;

	typedef std::vector< Simulated* > Queue;

	/**Queues with tasks, waiting for simulateion and tasks, returned from simulateors. They are swapped sometimes*/
	Queue putQueue, getQueue;
	boost::mutex putQueueMutex, getQueueMutex;
	typedef boost::unique_lock< boost::mutex > QueueLockT;


	class Worker{
	public:
	    int iterations;
	    int index;//worker number

	    Simulator * owner;
	    boost::thread thread;

	    Worker( Simulator *owner, int index_);
	    void operator()(); //main function
	    void run();
	};
	
	Simulator( int nWorkers );
	virtual ~Simulator();

	/**Add the item to the one of the workers*/
	void add( Simulated * item );
	/**Create worker threads and run simulation for N steps*/
	void run( int numSteps );
	/**Wait until all workers finish*/
	void waitAll();
	/**Request all simulators to stop. After this, call waitAll() to wit, until all simulator threads finish*/
	void requestStop();
    protected:
	/**SYnchronized method for use by workers: get next free task.*/
	Simulated* getNextTask();
	/**Synchronized method for use by workers: return task to the queue*/
	void returnTask( Simulated * task );

    };
};


#endif /* _SIMULATOR_H_ */
