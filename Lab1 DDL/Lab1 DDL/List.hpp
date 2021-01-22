#pragma once
template <class T>
class List
{
	class Node;		// forward decleration

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

	template<class X>   // kan det vara samma om på rad 2?
	class ListIter
	{
		friend class List<T>;
		Link *_ptr;
	public:

		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = X & ;
		using pointer = X * ;

		ListIter(Node<T> *p);
		ListIter();
		ListIter(const ListIter& other);
		ListIter& operatror=(const ListIter& other);
		T& operator*()
		{
			return _ptr->_data;
		}
		T* operator->();
		ListIter& operator++();
		ListIter& operator--();
		ListIter operator++(int);
		ListIter operator--(int);
		friend bool operator == (const ListIter& lhs, const ListIter& rhs);
		friend bool operator != (const ListIter& lhs, const ListIter& rhs);

	};

	Link _head; 
	size_t _size;

public:
	size_t Count() const
	{
		size_t count = 0;
	/*	for (Link i = &_head; i != &_head; i = i._next)
		{
			count++;
		}*/
		return count;
	}

	List() 
	{
		_size = 0;
		_head._next = &_head;
		_head._prev = &_head;
	}

	bool Invariant() const
	{
		size_t i = 0;

		return true;

	}

};
