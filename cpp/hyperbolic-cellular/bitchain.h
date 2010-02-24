#pragma once
#ifndef __BITCHAIN_INCLUDED__
#define __BITCHAIN_INCLUDED__

const int MAX_PIECES=4;
const int VALUE_BITS=2; //2 bits per one value

class bitchain
{
public:
    typedef unsigned int PieceType32;
    PieceType32 data[MAX_PIECES];
    int numValues;
    bitchain(){
	numValues = 0;
    };
    int size()const{ return numValues; };
    bitchain& append( bitchain::PieceType32 d){
	numValues+=1;
	setValue( numValues-1, d );
	return *this;
    };
    bitchain::PieceType32 pop(){
	PieceType32 v = getValue( numValues - 1);
	numValues -= 1;
	return v;
    };
    bitchain::PieceType32 operator[](int idx)const{
	return getValue( idx );
    };
private:
    static int valuesInPiece(){
	return sizeof(PieceType32)/VALUE_BITS;
    };
    static PieceType32 valueMask(){
	return (PieceType32(1)<<VALUE_BITS)-1;
    };
    static int maxIdx(){
	return valuesInPiece()*MAX_PIECES;
    };
    
    PieceType32 getValue( int idx)const{
	int pieceIdx = idx/valuesInPiece(); 
	int valueIdx = idx%valuesInPiece(); //index of the value in the piece
	int valueOffset = valueIdx*VALUE_BITS;
	return (data[pieceIdx]>>valueOffset)&valueMask();
    };
    PieceType32 setValue( int idx, bitchain::PieceType32 val ){
	int pieceIdx = idx/valuesInPiece(); 
	int valueIdx = idx%valuesInPiece(); //index of the value in the piece
	int valueOffset = valueIdx*VALUE_BITS;
	data[pieceIdx] &= ~(valueMask() << valueOffset);
	data[pieceIdx] |= ((val & valueMask())<<valueOffset);
    };
 
};

#endif
