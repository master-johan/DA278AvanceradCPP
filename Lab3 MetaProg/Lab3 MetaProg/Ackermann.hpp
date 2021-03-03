#pragma once

template <int m,int n>
struct Ackermann
{
	static const int value = Ackermann<m - 1, Ackermann<m, n - 1>::value>::value;
};

template <int m>
struct Ackermann<m, 0>
{
	static const int value = Ackermann<m - 1, 1>::value;
};

template <int n>
struct Ackermann<0, n>
{
	static const int value = n + 1;
};

template <>
struct Ackermann<0, 0>
{
	static const int value = 1;
};