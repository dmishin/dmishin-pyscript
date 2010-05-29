#ifndef _BALANCER_H_
#define _BALANCER_H_
#include <vector>

class BalancedWorker;
class Simulated;
/**Main class, responsible for the load balancing
 */
class Balancer{
public:
    //Complec data types
    typedef std::vector< BalancedWorker* > Workers;
private:
    //Data fields
    Workers workers;
    volatile bool running;
    volatile bool stopRequested;
    BalancedWorker::TimeType reachingTimeInterval;
    int rebalanceIntervalMs;
public:
    Balancer( int numWorkers );
    ~Balancer();

    //Simulation flow control
    void start();
    bool isRunning()const { return running; };
    void requestStop();
    void waitAll();

    int getRebalanceInterval()const{ return rebalanceIntervalMs; };
    void setRebalanceInterval( int msec ){ rebalanceIntervalMs=msec; };

    //Rebalancer parameters. Characteristic time ( in simulation cycles ) of the balancing process
    BalancedWorker::TimeType getJoinTime()const { return reachingTimeInterval; };
    void setJoinTime( BalancedWorker::TimeType t ){ reachingTimeInterval = t; };

    //Stop timer manipulation
    void setTimer( BalancedWorker::TimeType stopTime );
    void disableTimer();
    
    //Retuen mean time value for all workers (each worker has its own time)
    BalancedWorker::TimeType averageTime()const;
    int timeGap()const;

    //workers access. Deprecated. Use only for testing.
    BalancedWorker & getWorker( int idx ){ return *workers[idx]; };
private:
    void createWorkers( int numWorkers );
    void runWorkers();//start main loop thread in the each worker.
    //Main loop of the load balancer thread.
    void balanceLoop();
    typedef std::vector< int > SizesVector;
    //Performs the tasks shuffing, according to the calculated values.
    void rebalanceTasks( const SizesVector &oldSizes, const SizesVector &newSizes);
};


#endif /* _BALANCER_H_ */
