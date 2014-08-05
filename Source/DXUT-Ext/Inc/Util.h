#ifndef _UTIL_H_
#define _UTIL_H_

#include "SmartPtr.h"
#include "Color.h"

// 有用的函数

template< class T > inline T Max(const T A, const T B)
{
	return (A >= B) ? A : B;
}
template< class T > inline T Min(const T A, const T B)
{
	return (A <= B) ? A : B;
}
template< class T > inline T Max3(const T A, const T B, const T C)
{
	return Max(Max(A, B), C);
}
template< class T > inline T Min3(const T A, const T B, const T C)
{
	return Min(Min(A, B), C);
}

#endif