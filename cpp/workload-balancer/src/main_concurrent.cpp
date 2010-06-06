#include <iostream>
#include <stdlib.h>
#include <limits.h>

#include "concurrent_runner.hpp"
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
    if ( rand() %1000 == 1 )
	return false;
    age += 1;
    return true; //simulate infinitely
}


int main(int argc, char *argv[])
{
    using namespace std;

    cout<<"COncurrent runner test application"<<endl;

    int N = 4; //number of workers
    int Ntasks = 4000;//number of tasks for each balancer;

    cout<<"Creating balance loader with "<<N<<" workers and "<<Ntasks<<" tasks"<<endl;
    ConcurrentRunner runner( N );

    //putting wasks to the workers
    for( int i = 0; i < Ntasks; ++i ){
	runner.add( new DummyTask( rand()%10+10 ) );
    }

    cout<<"Starting simulation threads ..."<<endl;
    cout.flush();
    
    runner.run();

    return 0;
}
