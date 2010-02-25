#include "compact_array.h"
#include <iostream>

int main(int argc, char* argv[])
{
    compact_array<10, 2> array;
    std::cout<<array.items_in_piece<<std::endl;
    std::cout<<"done"<<std::endl;
    return 0;
}
