#include <iostream>
#include <stdlib.h>
#include <limits.h>

#include "balanced_worker.hpp"
#include "balancer.hpp"
#include "simulated.hpp"

class DummyTask: public Simulated
{
public:
    int complexity;
    int age;
    DummyTask( int complexity_ ): complexity( complexity_ ), age(0){};
    virtual ~DummyTask(){};
    virtual bool simulate();
};

bool DummyTask::simulate()
{
    //generate random values in loop
    int sum = 0;
    for ( int i = 0; i<complexity; ++i ){
	for( int j = 0; j<10; ++ j){
	    int x = rand();
	    sum += x;
	}
    }
    age += 1;
    return true; //simulate infinitely
}


int main(int argc, char *argv[])
{
    using namespace std;

    cout<<"balance loader test application"<<endl;

    int N = 4; //number of workers
    int Ntasks = 1000;//number of tasks for each balancer;
    int NtaskRandomization = 400;

    cout<<"Creating balance loader with "<<N<<" workers"<<endl;
    Balancer balancer( N );

    //putting wasks to the workers
    for( int i = 0; i < N; ++i ){
	int n = Ntasks+(rand() % NtaskRandomization) - NtaskRandomization/2;
	for( int j = 0; j < n ; ++j){
	    balancer.getWorker( i ).add( new DummyTask( rand()%10+10 ) );
	}
    }

    cout<<"Starting simulation threads ..."<<endl;
    
    balancer.start();

    return 0;
}
