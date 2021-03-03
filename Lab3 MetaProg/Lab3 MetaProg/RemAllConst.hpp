#pragma once

template <class T>
struct RAC
{
	using type = T;
};

template<class T>
using RAC_t = typename RAC<T>::type;

template<class T>
struct RAC<const T>
{
	using type = RAC_t<T>;
};

template<class T>
struct RAC<T&>
{
	using type = RAC_t<T>&;
};

template<class T>
struct RAC<T*>
{
	using type = RAC_t<T>*;
};