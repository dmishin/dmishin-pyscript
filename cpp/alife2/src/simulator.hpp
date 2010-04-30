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
	bool stopRequested;
    public:
	class Worker;
	typedef std::vector< Worker* > Workers;
	Workers workers;


	class Worker{
	public:
	    typedef std::set<Simulated*> WorkQueue;
	    Simulator * owner;
	    boost::thread thread;
	    WorkQueue queue;

	    Worker( Simulator *owner );
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
    };
};


#endif /* _SIMULATOR_H_ */
