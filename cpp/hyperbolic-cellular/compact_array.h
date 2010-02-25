#pragma once
#ifndef __COMPACT_ARRAY__
#define __COMPACT_ARRAY__

const int BYTE_SIZE = 8;//oh well.

#define DIVIDE_UP(a,b) ((a)%(b)==0?((a)/(b)):((a)/(b)+1))

template<int num_items, int item_bits>
class compact_array
{
public:
    typedef unsigned int piece_type;

    static const int items_in_piece = sizeof(piece_type)/item_bits;
    static const int num_pieces = DIVIDE_UP( num_items, items_in_piece );
    static const piece_type item_mask = (piece_type(1)<<item_bits-1);

    static const int used_bits = item_bits*items_in_piece;
    static const piece_type used_bits_mask = ~(~piece_type(0)<<used_bits);

    piece_type data[num_pieces];
    piece_type get(int idx){
	int piece_idx = idx/items_in_piece;
	int offset = item_bits*(piece_idx%items_in_piece);
	return (data[piece_idx]>>offset)&item_mask;
    };
    void set(int idx, piece_type val){
	int piece_idx = idx/items_in_piece;
	int offset = item_bits*(piece_idx%items_in_piece);
	data[piece_idx] = (data[piece_idx] & ~(item_mask<<offset)) | (val<<offset);
    };
};

#endif
