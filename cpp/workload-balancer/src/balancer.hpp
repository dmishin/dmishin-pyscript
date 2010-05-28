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
public:
    Balancer( int numWorkers );
    ~Balancer();

    //Simulation flow control
    void start();
    bool isRunning()const { return running; };
    void requestStop();
    void waitAll();

    //Stop timer manipulation
    void setTimer( BalancedWorker::TimeType stopTime );
    void disableTimer();
    
    //Retuen mean time value for all workers (each worker has its own time)
    BalancedWorker::TimeType averageTime()const;
    int timeGap()const;
private:
    void createWorkers( int numWorkers );
    //Main loop of the load balancer thread.
    void balanceLoop();
};


#endif /* _BALANCER_H_ */
