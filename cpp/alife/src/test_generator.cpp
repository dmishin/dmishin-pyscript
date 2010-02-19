#include "pseudo_generator.h"


#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <set>

std::set<int> values;

class my_generator:public pseudo_generator<int>{
public:
    /**Generator declaration*/
    BEGIN_GENERATOR_STATES
    state1, state2, state3, state4
        END_GENERATOR_STATES;

    /**State variables (local variables are not usable in generators*/
    int x;
    int a0, a1;
    std::set<int>::iterator iset;
		
    /*Generator must be default-constructible*/
    my_generator(){};

    /**real constructor*/
    my_generator(int _a0, int _a1){
        a0 = _a0;
        a1 = _a1;
    };
    /**generator body*/
    int operator()(){
        //generator preamble. All states must be mentioned
        BEGIN_RESTORE_STATE
            RESTORE_STATE( state1 )
            RESTORE_STATE( state2 )
            RESTORE_STATE( state3 )
            RESTORE_STATE( state4 )
            END_RESTORE_STATE;
        //main code
			
        BEGIN_GENERATOR;
        iset = values.begin();
        while (iset != values.end()){
            YIELD( *iset , state4);
            ++iset;
        }
        for( x = a0; x<a1; x++){
            YIELD( x, state1);
            YIELD( 1000+x, state2);
        }
        YIELD(-1000, state3);
        END_GENERATOR;
			
        return -1;//dummy value, not used
    };
};

struct printer{
    void operator()( int x){
        std::cout<<x<<std::endl;
    };
};

int main(int argc, char* argv[])
{
    values.insert(5);
    values.insert(7);
    values.insert(17);

    my_generator gener(5, 15);
    ///	
    /*
      do{
      int i = gener();
      if (! gener.stopped() ){
      std::cout<<i<<std::endl;
      }
      }while (! gener.stopped() );
    */
    generator_iterator<my_generator> gen(5, 15), end;
    std::for_each(gen, end, printer() );
    /*
      std::vector<int> v;
      v.insert(v.begin(), gen, end);
	
      for(int i =0; i<v.size(); ++i){
      std::cout<<v[i]<<std::endl;
      }
    */
    return 0;
}
