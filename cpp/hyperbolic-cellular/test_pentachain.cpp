#include <iostream>
#include "pentachain.h"

int main(int argc, char* argv[])
{
    try{
	PentaChain c;
	c.append(PentaChain::Node3_1);
	c.append(PentaChain::Node3_0);
	c.append(PentaChain::Node3_1);

	std::cout<<"Hello\n";
	std::cout<<"{"<<c.toStr()<<"}"<<std::endl;

    }catch(std::logic_error e){
	std::cerr<<e.what()<<std::endl;
    };
    return 0;
}
