
#ifndef _CONCURRENT_RUNNER_H_
#define _CONCURRENT_RUNNER_H_

/**Idea:
   Instead of creating several workers, each hawing its own set of tasks,
   and balancing there tasks to ensure that all owrkers are approximately synchronous,

   Make all workers to run sequentially tasks from the same queue.
   
   Even if oune worker is much faster than anouther, it is not a problem:
   that worker would just make more loops than the slow one.
   Time gap between different tasks would no more than N, where N is number of workers.

   The problem:
   1)In most situations, it would be disastrous to attempt to run the same task by two eorkers in the same time.
   ==> locking mechanism to mark task busy must be implemented
       ? use one  mutex and array of booleans?
       ? use one mutex for each task?
   2) Removing and adding tasks to the queue may be problematic.
  
   Good idea: two workers, running approximately at the same speed throught the task list
   may create often collisions.
   To solve this, make runners to go through the task list in the different order:
    runner1 executes tasks in order
    runner2 first executes even, then odd
    runner3 forst exectures ==0 mod 3, then ==1 mode 3 then ==2 mod 3 
    etc.
*/
#include <boost/thread.hpp>
#include <memory>
#include <vector>
typedef unsigned long TimeType;

class Simulated;
class ConcurrentRunner
{
private:
//Task queue definitions
    class Task{
    public:
	Simulated* task;
	boost::mutex * mutex;
	explicit Task( Simulated* task_): task(task_), mutex( new boost::mutex ){};
	~Task(){ delete mutex; };
	bool equals( Simulated * t )const { return t == task; }
    };
    typedef std::vector< Task > TaskQueue;
    TaskQueue queue;

    boost::shared_mutex queueMutex; //mutex for syncing queue modifications
    boost::mutex runnerDataMutex;
//Worker queue
    struct Worker{
	ConcurrentRunner & owner;
	int index;
	boost::thread thread;
	volatile bool running;

	Worker( ConcurrentRunner & owner, int index );
	~Worker();
	void mainLoop();
	void start();
    };
    typedef std::vector< Worker * > Workers;
    Workers workers;

//status fields
//Thread control data
    volatile bool stopRequested;
    volatile bool running;

    volatile TimeType time;
    size_t numWorkers;
    int cleanupIntervalMs;
public:
    ConcurrentRunner( size_t numThreads );
    virtual ~ConcurrentRunner();

    TimeType getTime()const{ return time; };
    void setTime( TimeType time_ ){ time = time_; };

    bool isRunning()const{ return running; };

    void add( Simulated * task ); //add task to the queue
    bool remove( Simulated * task);//remove task from the queue (slow)

//ConcurentWorker callbacks
    void increaseTimer();
//Thread control
    void run();
    void requestStop();
    
private:
    //Create and start all threads
    void runThreads( int N );
    //Send stop request to the threads and wait until all of them finish
    void stopThreads();
    //quick remove task by iterator, unsynced
    void remove( TaskQueue::iterator iTask );
    void removeDeadTasks();
    /**Runner runs this loop to clean up dead tasks from the queue periodically*/
    void cleanupLoop();
};

#endif /* _CONCURRENT_RUNNER_H_ */
