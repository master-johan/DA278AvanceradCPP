#pragma once

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
		void InsertFirst(Node* toInsert)
		{
			toInsert->_next = _next;
			toInsert->_prev = this;
			_next->_prev = toInsert;
			_next = toInsert; 
		}

		void InsertLast(Node* toInsert)
		{
			toInsert->_next = this;
			toInsert->_prev = _prev;
			_prev->_next = toInsert;
			_prev = toInsert;

		}

		//Node* DeleteAfter()
		//{
		//	Link* temp = _next;
		//	_next = next->_next;
		//	return static_cast<Node*>(temp);
		//}

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
		Node* _ptr;
	public:

		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = X&;
		using pointer = X*;

		ListIter()
		{
			_ptr = nullptr;
		}

		ListIter(const Link* p = nullptr) : _ptr(static_cast<Node*>(const_cast<Link*>(p))){}

		ListIter(const ListIter& other)
		{
			_ptr = other._ptr;
		}
		//ListIter& operatror=(const ListIter& other);
		X& operator*()
		{
			return _ptr->_data;
		}
		X* operator->()
		{
			return &_ptr->data;
		}
		ListIter& operator++()
		{
			_ptr = static_cast<Node*>(_ptr->_next);
			return *this;
		}
		//ListIter& operator--();
		//ListIter operator++(int);
		//ListIter operator--(int);

		friend bool operator == (const ListIter& lhs, const ListIter& rhs) = default;

		friend bool operator != (const ListIter& lhs, const ListIter& rhs) = default;

	};

	Link _head;


public:
	using iterator = ListIter<T>;
	using const_iterator = ListIter<const T>;

	//~Link()
	//{

	//}
	List()
	{
		_head._next = &_head;
		_head._prev = &_head;
		CHECK
	}
	//List(const List& other);
	List(const char* other) : List()
	{
	
		const char* ptr = other;
		while (*ptr != '\0')
		{
			push_back(*ptr);
			++ptr;
		}
		//while (*ptr != '\0')
		//	++ptr;
		//--ptr;
		//for (; ptr >= other; --ptr)
		//	push_front(*ptr);
		CHECK
	}


	//const T& front() const;
	//T& front();

	//const T& back() const;
	//T& back();

	//iterator begin() noexcept;
	//iterator end() noexcept;

	//bool empty() const noexcept;
	//size_t size() const noexcept;

	//iterator insert(iterator post, const T& value);
	//iterator eratse(const iterator& pos);

	void push_back(const T& toInsert)
	{
		Node* node = new Node(toInsert);
		_head.InsertLast(node);

	}
	void push_front(const T& toInsert)
	{
		Node* node = new Node(toInsert);
		_head.InsertFirst(node);
	}

	//void pop_back();
	//void pop_front();

	//operator=(const List& other); // VG
	//void swap(List<T>&rhs); // VG
	//void splice(const_iterator pos, List& other, const_iterator first, const_iterator last); // VG
	//friend void swap(List<T>& lhs, List < T& rhs); // VG

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
	//friend bool operator< (const List& lhs, const List& other);

	//friend std::ostream& operator<<(std::ostream& cout, const List& other);

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
	iterator end() { return iterator(&_head); }
	const_iterator end() const { return const_iterator(&_head); }

};
