#pragma once
#include <cstdlib>
#include <ostream>
#include <iostream>
#include <cassert>
#define CHECK assert(Invariant());
template <class T>
class List
{
	class Node;		// forward decleration


	class Link
	{
		friend class List<T>;
		Link* _next;
		Link* _prev;
		Link() : _next(this), _prev(this) {}
	public:
		void InsertBefore(Node* toInsert)
		{
			toInsert->_next = _next;
			toInsert->_prev = this;
			_next->_prev = toInsert;
			_next = toInsert;
		}

		void InsertAfter(Node* toInsert)
		{
			toInsert->_next = this;
			toInsert->_prev = _prev;
			_prev->_next = toInsert;
			_prev = toInsert;

		}

		Node* DeleteFirst()
		{
			Link* temp = _next;
			_next = _next->_next;
			_next->_prev = this;
			return static_cast<Node*>(temp);
		}

		Node* DeleteLast()
		{
			Link* temp = _prev;
			_prev = _prev->_prev;
			_prev->_next = this;
			return static_cast<Node*>(temp);
		}

		void PointTowardsEachOther(Link* second)
		{
			this->_next = second;
			second->_prev = this;
		}

	};

	class Node : public Link
	{
		friend class List;
		T _data;

	public:
		Node(const T& data) : _data(data) {};

	};

	template<class X>   
	class ListIter
	{
		friend class List<T>;
		Node* _ptr;
	public:

		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = X&;
		using pointer = X*;

		ListIter(const Link* p = nullptr) : _ptr(static_cast<Node*>(const_cast<Link*>(p))) {}

		ListIter(const ListIter&) = default;
	
		ListIter& operator= (const ListIter & other) = default;
	
		
		X& operator*()
		{
			return _ptr->_data;
		}
		X* operator->()
		{
			return &_ptr->_data;
		}
		ListIter& operator++()
		{
			_ptr = static_cast<Node*>(_ptr->_next);
			return *this;
		}
		ListIter& operator--()
		{
			_ptr = static_cast<Node*>(_ptr->_prev);
			return *this;
		}
		ListIter operator++(int)
		{
			auto temp(*this);
			operator++();
			return temp;
		}
		ListIter operator--(int)
		{
			auto temp(*this);
			operator--();
			return temp;
		}

		friend bool operator == (const ListIter& lhs, const ListIter& rhs) = default;

		friend bool operator != (const ListIter& lhs, const ListIter& rhs) = default;

	};

	Link _head;


public:
	using iterator = ListIter<T>;
	using const_iterator = ListIter<const T>;

	~List()
	{
		while (_head._next != &_head)
		{
			pop_front();
		}
		CHECK
	}

	List()
	{
		_head._next = &_head;
		_head._prev = &_head;
		CHECK
	}

	List(const List& other) : List()
	{
		if (other.Count() > 0)
		{
			Link* ptr = other._head._next;
			while (ptr != &other._head)
			{
				push_back(static_cast<Node*>(ptr)->_data);
				ptr = ptr->_next;
			}
		}
		CHECK
	}

	List(const char* other) : List()
	{
		const char* ptr = other;
		while (*ptr != '\0')
		{
			push_back(*ptr);
			++ptr;
		}
		CHECK
	}

	T& front()
	{
		return static_cast<Node*>(_head._next)->_data;
	}
	const T& front() const
	{
		return static_cast<Node*>(_head._next)->_data;
	}

	T& back()
	{
		return static_cast<Node*>(_head._prev)->_data;
	}
	const T& back() const
	{
		return static_cast<Node*>(_head._prev)->_data;
	}

	bool empty() const noexcept
	{
		return begin() == end();
	}

	size_t size() const noexcept 
	{
		return std::distance(cbegin(), cend());
	}

	iterator insert(iterator pos, const T& value)
	{
		pos._ptr->InsertAfter(new Node(value));
		return pos._ptr->_prev;
	}
	
	iterator erase(const iterator& pos) 
	{
		iterator temp = pos;
		++temp;

		pos._ptr->_prev->_next = pos._ptr->_next;
		pos._ptr->_next->_prev = pos._ptr->_prev;


		delete pos._ptr;
		CHECK
		return temp;
	}

	void push_back(const T& toInsert)
	{
		Node* node = new Node(toInsert);
		_head.InsertAfter(node);

	}
	void push_front(const T& toInsert)
	{
		Node* node = new Node(toInsert);
		_head.InsertBefore(node);
	}

	void pop_back()
	{
		delete _head.DeleteLast();
		CHECK
	}
	void pop_front()
	{
		delete _head.DeleteFirst();
		CHECK
	}

	List& operator=(const List& other) // VG
	{
		List newList(other);
		swap(newList);
		return *this;
	}
	void splice(const_iterator pos, List& other, const_iterator first, const_iterator last) // VG
	{
		if (first == last)
		{
			return;
		}
		
		Link* beforePos = pos._ptr->_prev;
		Link* beforeFirst = first._ptr->_prev;
		Link* beforeLast = last._ptr->_prev;

		beforePos->PointTowardsEachOther(first._ptr);
		beforeFirst->PointTowardsEachOther(last._ptr);
		first._ptr->PointTowardsEachOther(pos._ptr);


		CHECK
	}
	void swap(List<T>& rhs)// VG
	{
		bool thisEmpty = empty();
		bool rhsEmpty = rhs.empty();

		if (thisEmpty && rhsEmpty)
		{
			return;
		}
		else if (thisEmpty)
		{
			_head._next = rhs._head._next;
			_head._next->_prev = &_head;
			_head._prev = rhs._head._prev;
			_head._prev->_next = &_head;

			rhs._head._next = &rhs._head;
			rhs._head._prev = &rhs._head;
		}
		else if (rhsEmpty)
		{
			rhs._head._next = _head._next;
			rhs._head._next->_prev = &rhs._head;
			rhs._head._prev = _head._prev;
			rhs._head._prev->_next = &rhs._head;
			_head._next = &_head;
			_head._prev = &_head;
		}
		else
		{
			Link* temp1 = _head._next;
			Link* temp2 = _head._prev;

			_head._next = rhs._head._next;
			_head._next->_prev = &_head;
			_head._prev = rhs._head._prev;
			_head._prev->_next = &_head;

			rhs._head._next = temp1;
			rhs._head._next->_prev = &rhs._head;
			rhs._head._prev = temp2;
			rhs._head._prev->_next = &rhs._head;
		}

		CHECK
	}

	friend void swap(List<T>& lhs, List<T>& rhs) // VG
	{
		lhs.swap(rhs);
	}

	friend auto operator<=> (const List& lhs, const List& rhs)
	{
		auto lIt = lhs.begin();
		auto rIt = rhs.begin();

		for (; lIt != lhs.end() && rIt != rhs.end(); ++lIt, ++rIt)
			if (*lIt < *rIt)
				return std::strong_ordering::less;
			else if (*lIt > *rIt)
				return std::strong_ordering::greater;
		if (lIt == lhs.end())
			if (rIt == rhs.end())
				return std::strong_ordering::equivalent;
			else
				return std::strong_ordering::less;
		else
			return std::strong_ordering::greater;

	}

	friend bool operator==(const List& lhs, const List& rhs)
	{
		return (lhs <=> rhs) == 0;
	}

	friend bool operator!=(const List& lhs, const List& rhs)
	{
		return (lhs <=> rhs) != 0;
	}

	friend std::ostream& operator<<(std::ostream& cout, const List& other)
	{
		for (auto it = other.begin(); it != other.end(); ++it)
		{
			cout << *it << " ";
		}
		cout << std::endl;
		return cout;
	}

	std::ostream& Print(std::ostream& cout)
	{
		for (auto ptr = _head._next; ptr != &_head; ptr = ptr->_next)
		{
			cout << static_cast<Node*>(ptr)->_data << " ";
		}
		cout << std::endl;
		return cout;
	}

	size_t Count() const
	{
		size_t count = 0;
		for (auto ptr = _head._next; ptr != &_head; ptr = ptr->_next)
		{
			count++;
		}
		return count;
	}

	bool Invariant() const {
		size_t i = 0;
		for (auto ptr = _head._next; ptr != &_head; ptr = ptr->_next)
		{
			if (++i == std::numeric_limits<size_t>::max())
				return false;
			if (ptr->_next->_prev != ptr)
				return false;

		}

		return true;
	}

	iterator begin() { return iterator(_head._next); }
	const_iterator begin() const { return const_iterator(_head._next); }
	const_iterator cbegin() const { return const_iterator(_head._next); }
	iterator end() { return iterator(&_head); }
	const_iterator end() const { return const_iterator(&_head); }
	const_iterator cend() const { return const_iterator(&_head); }

};
