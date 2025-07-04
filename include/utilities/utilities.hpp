#pragma once


#define USE_STL_VECTOR 1
#define USE_STL_DEQUE 1

#if USE_STL_VECTOR
#include <vector>
namespace drop::utl
{
	template<typename T>
	using vector = std::vector<T>;
} // namespace drop::utl
#endif

#if USE_STL_DEQUE
#include <deque>
namespace drop::utl
{
	template<typename T>
	using deque = std::deque<T>;
} // namespace drop::utl
#endif

//#define BIT(x) 1 << x