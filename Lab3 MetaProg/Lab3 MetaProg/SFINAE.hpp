#pragma once
#include <type_traits>

//template <bool V, class T>
//struct Enable_if 
//{};
//
//template <class T>
//struct Enable_if<true, T>
//{
//	using type = T;
//};


//template <class T,class =  Enable_if< (sizeof(T) <= 4),nullptr_t>::type>
//bool NoThrowCopyConstructible(T)
//{
//	return true;
//}
//
//template <class T, class =  Enable_if<(sizeof(T) > 4), nullptr_t>::type>
//bool NoThrowCopyConstructible(const T&) // måste vara annan parameter annars klagar den på att metoden redan är definerad
//{
//	return false;
//}


template <typename T, class = std::enable_if_t<std::is_nothrow_copy_constructible<T>::value>>
bool NoThrowCopyConstructible(T x)
{
	return true;
}
template <typename T, class = std::enable_if_t<!std::is_nothrow_copy_constructible<T>::value>>
bool NoThrowCopyConstructible(T& x) 
{
	return false;
}

