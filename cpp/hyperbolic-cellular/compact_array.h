#pragma once
#ifndef __COMPACT_ARRAY__
#define __COMPACT_ARRAY__

/*********************************************************************************
 *  Memory-efficient, fixed size container for values with small number of bits.
 *  Usable, when you need a compact array of values, which are shorter than byte. (1-7 bits).
 * 
 *
 *  This container implements efficient packing of short values into an array-like container
 *  Item size in bits must not be more than size of integer value (for wider values, compact
 *  storage is not a consideration). For the sake of speed, this class does not splits one 
 *  value on two words, thus, for 3-bit values, only 30 bits of every word are used on 32-bit machine.
 *
 * Dmishin. This code is free domain.
 ***********************************************************************************/


#include <cassert>

const int BYTE_SIZE = 8;//oh well.

//convenience macro for integer divisin with rounding up. Used for constant initialization.
#define CARRAY_DIVIDE_UP(a,b) ((a)%(b)==0?((a)/(b)):((a)/(b)+1))

template<int num_items, int item_bits>
class compact_array
{
public:
    typedef unsigned int piece_type;

    typedef piece_type value_type; //for STL compatibility

    static const int items_in_piece = (BYTE_SIZE*sizeof(piece_type))/item_bits;
    static const int num_pieces = CARRAY_DIVIDE_UP( num_items, items_in_piece );
    static const piece_type item_mask = (piece_type(1)<<item_bits)-1;

    static const int used_bits = item_bits*items_in_piece;
    static const piece_type used_bits_mask = ~((~piece_type(0))<<used_bits);
    /**Maximal possible value(inclusive)*/
    static const value_type max_value = item_mask;

    piece_type data[num_pieces];

    int size()const{
	return num_items;
    };

    piece_type get(int idx)const{
	assert(idx >= 0 && idx < num_items);

	int piece_idx = idx/items_in_piece;
	int offset = item_bits*(idx%items_in_piece);
	return (data[piece_idx]>>offset)&item_mask;
    };

    void set(int idx, piece_type val){
	assert(idx >= 0 && idx < num_items);
	assert( (val & ~item_mask) == 0); //value has no bits outside mask
	int piece_idx = idx/items_in_piece;
	int offset = item_bits*(idx%items_in_piece);
	data[piece_idx] = (data[piece_idx] & ~(item_mask<<offset)) | (val<<offset);
    };

    /** clear with zeros */
    void zero(){
	for(int i = 0;i<num_pieces;++i){ data[i]=0; };
    }

    /*Proxy object for implementing the [] operator*/
    struct item_setter{
	int idx;
	compact_array &array;

	item_setter(compact_array &arr, int _idx)
	    :idx(_idx),array(arr)
	    {};
	
	operator piece_type()const{
	    return array.get( idx );
	};
	item_setter& operator=(const piece_type& v){
	    array.set( idx, v);
	    return *this;
	};
    };

    item_setter operator[](int idx){ //return special proxy object, imitating reference
	return item_setter(*this, idx);
    };

    piece_type operator[](int idx)const{
	return get(idx);
    };

    //TODO: implement an STL iterator interface?
};

#endif
