#ifndef _BALANCED_WORKER_H_
#define _BALANCED_WORKER_H_
/**Worker thread for the load balancer
   Its main purpose if to execute infinitely a list of tasks
 */
#include <vector>
#include<boost/thread.hpp>
#include "balancer_types.hpp"

class Simulated;
class Balancer;
class BalancedWorker{
public:
    //Complex types
    typedef std::vector< Simulated* > Queue;
private:
    //data members
    Balancer & parent;
    Queue queue; //lsit of tasks
    volatile TimeType time;
    volatile TimeType stopTime;
    volatile bool timerSet; //if true, simulation will stop, when time reaches stopTime
    volatile bool stopRequested; //set to true to finish the loop
    volatile int requestedRebalancing; //If positive, worker must get some additional tasks to increase load. If negative, worker must return sume tasks to the pool

    int idleSleepTime; //sleep time in ms to sleep between idle cycles

public:
    BalancedWorker( Balancer& parent_ );
    ~BalancedWorker();

    TimeType run();     //main simulation loop
    void requestStop(){ stopRequested = true; }; //

    void add( Simulated* task );        //Add task to the queue (synchronous)
    bool remove( Simulated * task );    //Slow method for manual task removal. True if successes. synchronous.
    int size()const{ return queue.size(); };                  //Size of the queue
    bool empty()const{ return queue.empty(); }; //is queue empty or not
    
    TimeType getTime()const        { return time; };
    void setTime( TimeType time_ ) { time = time_; };
    
    //Stop time - a moment in time, when stop requested.
    void setTimer( TimeType stime ) { stopTime = stime; timerSet = true; }; 
    void setTimerRelative( TimeType dt ) { stopTime = time + dt; timerSet = true; };
    void disableTimer(){ timerSet = false; };
    TimeType getTimer() const { return stopTime; };
    bool isTimerSet()const { return timerSet; };

    //Balancing. Called by balancer to request workload change
    void requestRebalance( int workloadChange );
private:
    //Internal operation details
    void simulate(); 
    //Remove task from the queue, non-sync.
    void remove( Queue::iterator pos );
    /**Pop one task from the queue. Probably, popping random task may be effective, but I am not sure yet.*/
    Simulated * popOneTask();
    //Check, whether rebalance request is pending, and perform rebalancing.
    void updateBalance(); 
private:
    boost::thread workerThread;// the thread, executing the worker code.
public:
   /**Wait, until thread finish its work
     It may be good idea to request stop before calling this, or waiting can become infinite.
    */
    void wait();
    /*Start the thread*/
    void runThread();
};

#endif /* _BALANCED_WORKER_H_ */
