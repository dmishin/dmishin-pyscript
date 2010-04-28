#include <iostream>

#include "point.hpp"
#include "rotation.hpp"
#include "grid.hpp"
#include "grid_item.hpp"
#include "mobile.hpp"
#include "glut_view.hpp"
#include "world.hpp"

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
    
    World world;

    FOR_RANGE( i, 0, 100 ){
	Mobile * item = new Mobile();
	item->setLocation( vec2(randf(0,10), randf(0,10) ) );
	world.add( item );
    }

    cout<<"========================\n";


    GlutView view;
    view.setWorld( &world );
    view.activate();
    view.runLoop();
    return 0;
}
