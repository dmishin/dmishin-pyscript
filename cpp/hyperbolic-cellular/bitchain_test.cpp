#include "bitchain.h"

#include <iostream>
int main( int argc, char* argv[])
{
    bitchain bc;
    bc.append( 0 );
    bc.append( 2 );
    bc.append( 1 );
    for (int i =0; i<bc.size(); ++i ){
	std::cout<<bc[i]<<std::endl;
    }
    return 0;
}
