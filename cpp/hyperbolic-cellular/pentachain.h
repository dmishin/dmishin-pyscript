#pragma once
#ifndef __PENTACHAIN_INCLUDED__
#define __PENTACHAIN_INCLUDED__
#include <cassert>

#include "compact_array.h"
#include <string>
#include <stdexcept>

class PentaChain{
public:
    static const int MAX_CHAIN=100;

    typedef compact_array<MAX_CHAIN, 3> ArrayType; //type of the internal compact array
    ArrayType chain;
    int chainLen;

    typedef enum{
	Node2_0, Node2_1,
	Node3_0, Node3_1, Node3_2
    } ChainElement;

    typedef enum{
	Node2, Node3, __WRONG_NODE_TYPE
    } NodeType;

    PentaChain()
	: chainLen(0)
    { 
	chain.zero();//zero-init chain for simplier comparision;
    }
    
    
    ChainElement get( int idx )const{
	assert( idx>=0 && idx < chainLen);
	ArrayType::value_type val = chain[idx];
	assert(val <= Node3_2);
	return static_cast<ChainElement>( val  );
    };

    ChainElement get()const{
	return get(size()-1);
    }

    int size()const{
	return chainLen;
    }
    
    /**Returns type of the new node to be created*/
    NodeType getNewNodeType()const{
	if (size()==0) return Node3;//0th noded is always 3-node
	ChainElement last = get();
	if (last == Node2_0 || last == Node3_0) //TODO: check this!
	    return Node2;
	else
	    return Node3;
    }

    /**Get type of the existing node*/
    NodeType getNodeType( int idx )const{
	return nodeType( get(idx) );
    }

    /**Return the node type of the element*/
    static NodeType nodeType( ChainElement elt ){
	if ( elt >= Node2_0 && elt <=Node2_1)
	    return Node2;
	else if(elt >= Node3_0 && elt <=Node3_2)
	    return Node3;
	else
	    assert( false );//Wrong node value. Memory corruption?
	return __WRONG_NODE_TYPE;//dummy for compiler.
    }	

    /** append an element. Note, that setting element inside is almost always impossible!*/
    void append(ChainElement e){
	assert( getNewNodeType() == nodeType( e ) );//node type must be right!
	chain[ chainLen ] = e;
	chainLen ++;
    }

    ChainElement pop(){
	ChainElement rval = get();
	chain.set( chainLen-1, 0);//clear the value, to simplify comparisions
	chainLen--;
	return rval;
    };
    
    std::string toStr()const{
	std::string rval;
	
	for(int i=0;i<size();++i){
	    const char* c="";
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
