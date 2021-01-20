#pragma once
template <class T>
class List
{
	class Node;

	class Link
	{
		friend class List<T>;
		Link* _next, *_prev;
		Link() : _next(this), _prev(this){}
	};

	class Node : public Link
	{
		friend class List;
		T _data;

	public:
		Node(const T& data) : _data(data) {};
	};

	template<class T>   // kan det vara samma om på rad 2?
	class ListIter
	{
		friend class List;
		Link *_ptr;
	public:

		ListIter(Node<T> *p);
		ListIter();
		ListIter(const ListIter& other);
		ListIter& operatror = (const ListIter& other);
		T & operator*();
		T* operator->();
		ListIter& operator++();
		ListIter& operator--();
		ListIter operator++(int);
		ListIter operator--(int);
		friend bool operator == (const ListIter& lhs, const ListIter& rhs);
		friend bool operator != (const ListIter& lhs, const ListIter& rhs);

	};

	Link _head; 

};
