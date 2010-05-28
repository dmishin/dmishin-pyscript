#ifndef _BALANCED_WORKER_H_
#define _BALANCED_WORKER_H_
/**Worker thread for the load balancer
   Its main purpose if to execute infinitely a list of tasks
 */
#include <vector>
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

    BalancedWorker::TimeType run();     //start simulation
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
};

#endif /* _BALANCED_WORKER_H_ */
