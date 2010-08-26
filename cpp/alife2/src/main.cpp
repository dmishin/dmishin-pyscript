#include <iostream>

#include "point.hpp"
#include "rotation.hpp"
#include "grid.hpp"
#include "grid_item.hpp"
#include "mobile.hpp"
#include "glut_view.hpp"
#include "world.hpp"
#include "simulator.hpp"


std::ostream & operator << (std::ostream &os, const alife2::vec2 &v)
{
    return os<<"("<<v.x<<","<<v.y<<")";
}


int main(int argc, char *argv[])
{
    using namespace std;
    using namespace alife2;
    
    vec2 v(10,10);
    cout << v << std::endl;
    rotation rot( 0.5 );
    cout << rot.apply( v ) << " | " <<(rot - rot).apply( v ) <<std::endl;
    
    cout << "== Testing grid ==\n";
    
    World world( 100, 100, 4 );
    world.setSimulationStep( 0.05f );
    Simulator sim( 4 );//simulator with 4 threads; 1-not used
    sim.add( &world );//world is a simulated item, add it to the simultaro

    FOR_RANGE( i, 0, 1000 ){
	Mobile * item = new Mobile();
	item->setLocation( vec2(randf(0,100), randf(0,100) ) );
	item->setAngle( randf( 0, 2*M_PI ) );
	item->setSpeed( vec2( randf(-1,1), randf(-1, 1) ) );
	item->setAngleSpeed( randf( -5, 5 ) );
	world.add( item );
	sim.add( item );//add mobile to the simultaro too...
    }

    cout<<world.gridMobiles.toString();
    cout<<"========================\n";


    GlutView view;
    view.setWorld( &world );
    view.activate();
    view.setZoom( 4 );
//starting simulator for infinite number of iterations
    
    sim.run( -1 );
    view.runLoop();
    std::cout<<"Glut loop finished, closing simultaion..."<<std::endl;

    sim.requestStop();
    sim.waitAll();
    std::cout<<"All simulation threads finished"<<std::endl;
    return 0;
}
