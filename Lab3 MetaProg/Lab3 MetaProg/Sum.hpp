#pragma once
#include <type_traits>

template <class T>
auto Sum(const T& c)
{
	decltype(*std::begin(c)) x = (*std::begin(c));

	return 1;
}