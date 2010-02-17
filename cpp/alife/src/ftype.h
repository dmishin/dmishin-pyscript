#pragma once
#ifndef _FTYPE_H_INCLUDED_
#define _FTYPE_H_INCLUDED_

#include <math.h>

//basic floating-point type
typedef float ftype;

template <typename T>
T sqr( T x )
{
	return x*x;
};

template <typename T>
T min( T x, T y)
{
	if (x>y) return y;
	else     return x;
}

template <typename T>
T max( T x, T y)
{
	if (x<y) return y;
	else     return x;
}

template <typename T>
T limit( T x, T min_value, T max_value)
{
	return min(max(x, min_value), max_value);
}



#endif
