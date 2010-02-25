#include "compact_array.h"
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout<<"Testing array\n";
    compact_array<10, 13> array;
    compact_array<12,3> array_anouther;
    for (int i =0; i<array.size(); ++i){
//	array.set(i, i%3);
	array_anouther[i] = 3;
	array[i] = i%3;
    }

    compact_array<10,13> array1;
    array1 = array;

    std::cout<<"Array item mask:"<<array.item_mask<<std::endl;
    std::cout<<"Array num_pieces:"<<array.num_pieces<<std::endl;
    std::cout<<"Items in piece:"<<array.items_in_piece<<std::endl;
    for(int i=0; i<array1.size(); ++i){
	std::cout<<"["<<array1.get(i)<<"]";
    }
    std::cout<<std::endl;
    std::cout<<"done"<<std::endl;
    return 0;
}
