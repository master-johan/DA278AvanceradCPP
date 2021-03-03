#pragma once
#include <type_traits>

template <class T>
auto Sum(const T& c)
{
	using type = decltype(*c.begin());
	using type_noref = typename std::remove_reference<type>::type;
	using type_noconst = typename std::remove_const<type_noref>::type;
	

	type_noconst sum{};

	for (auto i : c)
		sum += i;

	return sum;
}