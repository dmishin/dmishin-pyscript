#pragma once
#ifndef __PENTACHAIN_INCLUDED__
#define __PENTACHAIN_INCLUDED__

#include "bitchain.h"
#include <string>
#include <stdexcept>

class PentaChain{
public:
    bitchain chain;
    typedef enum{
	Node2_0, Node2_1,
	Node3_0, Node3_1, Node3_2
    } ChainElement;
    typedef enum{
	Node2, Node3
    } NodeType;

    ChainElement get( int idx)const{
	int val = chain[idx];
	if (getNodeType( idx )== Node2){
	    if (val >=0 && val < 2)
		return static_cast<ChainElement>(Node2_0+val);
	}else{
	    if (val>=0 && val<3)
		return static_cast<ChainElement>(Node3_0+val);
	};
	throw std::logic_error("Failed to get(): chain contains incorrect item");
    };

    int size()const{ return chain.size();};
    void set(int idx, ChainElement v);
    
    NodeType getNodeType( int idx )const{
	if ( idx == 0 || idx == -1)
	    return Node3;//start node is 3-node
	else{
	    //check previous value
	    ChainElement prevElt = get( idx-1 );
	    if ( prevElt==Node2_1 || prevElt == Node3_2)
		return Node2;
	    else
		return Node3;
	}
    }
    NodeType getNodeType()const{ 
	return getNodeType(size()-1); 
    };
    void append(ChainElement e){
	unsigned int val = 0;
	if (getNodeType() == Node2){
	    switch( e ){
		case Node2_0: 
		    val = 0; break;
		case Node2_1:
		    val = 1; break;
		default:
		    throw std::logic_error("Wrong node type");
	    }
	}else{
	    switch( e ){
		case Node3_0: 
		    val = 0; break;
		case Node3_1:
		    val = 1; break;
		case Node3_2:
		    val = 2; break;
		default:
		    throw std::logic_error("Wrong node type");
	    }
	}
	chain.append( val );
    };
    ChainElement pop(){
	ChainElement rval = get(size()-1);
	chain.pop();
	return rval;
    };
    std::string toStr()const{
	std::string rval;
	
	for(int i=0;i<size();++i){
	    char* c="";
	    switch (get(i)){
		case Node2_0:
		    c="[0/2]";break;
		case Node2_1:
		    c="[1/2]";break;
		case Node3_0:
		    c="[0/3]";break;
		case Node3_1:
		    c="[1/3]";break;
		case Node3_2:
		    c="[2/3]";break;
	    };
	    rval += c;
	}
	return rval;
    };
};

#endif
