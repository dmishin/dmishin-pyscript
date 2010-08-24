#pragma once
#ifndef _UTIL_H_
#define _UTIL_H_

#include <math.h>
#include <algorithm>

namespace alife2{
    //Math utilities for float calculations
    inline
    float sqr( float x ){
	return x*x;
    }
    inline
    float sqrtf( float x ){
	return float(sqrt(x));
    }
    inline
    float sinf( float x ){
	return float(sin(x));
    }
    inline 
    float cosf( float x ){
	return float( cos(x));
    }
    inline
    float atan2f( float x, float y){
	return float( atan2( x, y ) );
    }    

    template<typename T>
    T max( T a, T b){
	if (a > b) 
	    return a;
	else
	    return b;
    };

    template< typename T>
    T min( T a, T b ){
	if (a < b)
	    return a;
	else
	    return b;
    }
    
    /**Limit x with high and low limits*/
    template< typename T >
    T limit( T x, T low_limit, T high_limit ){
	return min( max( x, low_limit ), high_limit );
    }
	

    //TODO: make it of type T? Will it speed up simulation anyhow?
    template< typename T >
    int signum( T x ){
	if ( x > 0 )
	    return 1;
	else if (x < 0 )
	    return -1;
	else
	    return 0;
    };
    
    /**Random floats*/
    float randf();
    float randf( float x0, float x1 );
};

///Some possibly useful macros
#define FOR_RANGE( var, begin, end ) \
    for( int var = (begin); var < (end); ++(var) )

//Useful class
template< class ElementT >
struct simple_array{
    ElementT * pData;
    size_t size_;

    simple_array() { pData = NULL; size_ = 0; };
    simple_array( size_t size ){ _create( size ); };
    size_t size()const{ return size_; };
    ElementT& operator[]( int idx ){ return pData[idx]; };
    const ElementT& operator[]( int idx)const{ return pData[idx]; };
    ~simple_array(){ delete[] pData; };

    //move-constructor
    simple_array( simple_array<ElementT>& arr ){ (*this) = arr; };
    simple_array<ElementT> & operator=( simple_array<ElementT>& arr ){ 
	std::swap( pData, arr.pData ); 
	return *this;
    };

    //Support for the stl-like iterators
    typedef ElementT * iterator;
    typedef const ElementT * const_iterator;
    iterator begin(){ return pData; };
    iterator end(){ return pData + size_; };

    //Managing data in the array
    void create( size_t size_ ){ clear(); _create( size_ ); }
    void clear(){ delete[] pData; pData = NULL; }

private:
    void _create( size_t new_size ){
	size_ = new_size;
	pData = new ElementT[ new_size ];
    };
};


#endif /* _UTIL_H_ */
