#ifndef _BALANCED_WORKER_H_
#define _BALANCED_WORKER_H_
/**Worker thread for the load balancer
   Its main purpose if to execute infinitely a list of tasks
 */
#include <vector>
#include<boost/thread.hpp>

class Simulated;
class BalancedWorker{
public:
    //Complex types
    typedef std::vector< Simulated* > Queue;
    typedef unsigned long TimeType;
private:
    //data members
    Queue queue; //lsit of tasks
    volatile TimeType time;
    volatile TimeType stopTime;
    volatile bool timerSet; //if true, simulation will stop, when time reaches stopTime
    volatile bool stopRequested; //set to true to finish the loop
public:
    BalancedWorker();
    ~BalancedWorker();

    BalancedWorker::TimeType run();     //main simulation loop
    void requestStop(){ stopRequested = true; }; //

    void add( Simulated* task );        //Add task to the queue (synchronous)
    bool remove( Simulated * task );    //Slow method for manual task removal. True if successes. synchronous.
    
    BalancedWorker::TimeType getTime()const        { return time; };
    void setTime( BalancedWorker::TimeType time_ ) { time = time_; };
    
    //Stop time - a moment in time, when stop requested.
    void setTimer( BalancedWorker::TimeType stime ) { stopTime = stime; timerSet = true; }; 
    void setTimerRelative( BalancedWorker::TimeType dt ) { stopTime = time + dt; timerSet = true; };
    void disableTimer(){ timerSet = false; };
    BalancedWorker::TimeType getTimer() const { return stopTime; };
    bool isTimerSet()const { return timerSet; };

    int size()const{ return queue.size(); };                  //Size of the queue

private:
    //Internal operation details
    void simulate(); 
    //Remove task from the queue, non-sync.
    void remove( Queue::iterator pos );
    //Thread machinery
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
/**Compare two time marks, 
   Taking in account that overflow of the time counter can occur*
   assumed, that both time marks are somewhere near the t0
*/
struct TimeLess{
    BalancedWorker::TimeType t0;
    TimeLess( BalancedWorker::TimeType t0_ ): t0(t0_){};
    bool operator()(BalancedWorker::TimeType t1, BalancedWorker::TimeType t2)const{
	return static_cast<int>(t1-t0) < static_cast<int>(t2-t0);
    };
};


#endif /* _BALANCED_WORKER_H_ */
