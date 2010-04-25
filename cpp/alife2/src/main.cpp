#include <iostream>

#include "point.hpp"
#include "rotation.hpp"
#include "grid.hpp"
#include "grid_item.hpp"
#include "mobile.hpp"

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
    
    Grid grd;
    grd.initGrid( 10, 10, 2 );
    cout<< "Created grid with "<<grd.getNumCols()<<" columns and"<<grd.getNumRows()<<"rows\n";
    cout<<"Grid size:"<<grd.getSize()<<endl;
    cout<<"Putting items to grid\n";
    
    FOR_RANGE( i, 0, 100 ){
	GridItem * item = new Mobile();
	item->setLocation( vec2(randf(0,10), randf(0,10) ) );
	grd.putItem( item );
    }
    grd.update();

    cout<<"========================\n";
    cout<< grd.toString() <<endl;
    cout<<"========================\n";

    return 0;
}
