#pragma once
#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_
#include "simulated.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <set>
#include <vector>

namespace alife2{
    class Simulator{
    private:
	int lastWorkerIndex;
	volatile bool stopRequested;
	volatile bool isRunning;
	int simulationChunkSteps;//how many steps to run before sync
	boost::barrier simulationSyncBarrier;//barrier, used to synchronize several simulation threads
    public:
	class Worker;
	typedef std::vector< Worker* > Workers;
	Workers workers;


	class Worker{
	public:
	    int iterations;
	    int index;//worker index
	    typedef std::set<Simulated*> WorkQueue;
	    Simulator * owner;
	    boost::thread thread;
	    WorkQueue queue;

	    boost::mutex queueAccessMutex;

	    Worker( Simulator *owner, int index_);
	    void add( Simulated * item );
	    void remove( Simulated * item );
	    void operator()(); //main function
	    void run();
	};
	
	Simulator( int nWorkers, int simulateBy );
	virtual ~Simulator();

	/**Add the item to the one of the workers*/
	void add( Simulated * item );
	/**Create worker threads and run simulation for N steps*/
	void run( int numSteps );
	/**Wait until all workers finish*/
	void waitAll();
	/**Request all simulators to stop. After this, call waitAll() to wit, until all simulator threads finish*/
	void requestStop();
    };
};


#endif /* _SIMULATOR_H_ */
