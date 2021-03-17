#pragma once
#include <cstdlib>
#include <ostream>
#include <iostream>
#include <cassert>
#include <new>
#include "Dalloc.hpp"
#define CHECK assert(Invariant());



template<class T>
class Vector
{

	T* _pointer;
	size_t _size;
	size_t _capacity;
	Dalloc<T> _allocator;
#pragma region IteratorClass
	template <class X, int DIR>
	class VectorItt
	{
		friend class Vector<T>;
		T* _ptr;

	public:
#pragma region TypeDefs
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = X&;
		using pointer = X*;
#pragma endregion TypeDefs

		VectorItt()
		{
			_ptr = nullptr;
		}

		VectorItt(X* p) : _ptr(static_cast<T*>(const_cast<T*>(p))) {}


		VectorItt(const VectorItt& other)
		{
			_ptr = other._ptr;
		}

		VectorItt& operator= (const VectorItt& other)
		{
			_ptr = other._ptr;
			return *this;
		}


		template<class Y, class = std::enable_if_t <!std::is_const_v <Y>>> //const_iterator(iterator&)
		VectorItt(const VectorItt<Y, DIR>& rhs) : _ptr(rhs._ptr) {}

		template<class Y, class = std::enable_if_t <!std::is_const_v <Y>>>	//const_iterator& operator=(iterator&)
		VectorItt& operator= (const VectorItt& other) { _ptr = other._ptr; return *this; }

#pragma region IttOperator

		X& operator*() const
		{
			return *_ptr;
		}

		X* operator->()
		{
			return _ptr;
		}

		X& operator[](size_t i)
		{
			return *(_ptr + (i * DIR));
		}

		VectorItt& operator++()
		{
			_ptr += DIR;
			return *this;
		}
		VectorItt& operator--()
		{
			_ptr -= DIR;
			return *this;
		}
		VectorItt operator++(int)
		{
			auto temp(*this);
			operator++();
			return temp;
		}
		VectorItt operator--(int)
		{
			auto temp(*this);
			operator--();
			return temp;
		}

		VectorItt operator+(difference_type i) const
		{
			return _ptr + (i * DIR);
		}
		VectorItt operator-(difference_type i) const
		{
			return _ptr - (i * DIR);
		}
		difference_type operator-(const VectorItt& other)const
		{
			return _ptr - other._ptr;
		}

		VectorItt& operator+= (difference_type i)
		{
			this->_ptr += (i * DIR);
			return *this;
		}



		friend auto operator<=>(const VectorItt& lhs, const VectorItt& rhs)
		{
			if (lhs._ptr < rhs._ptr)
				return -DIR;
			else if (lhs._ptr > rhs._ptr)
				return DIR;
			//else if (lhs._ptr == rhs._ptr)
			return 0;

		}
		friend bool operator==(const VectorItt& lhs, const VectorItt& rhs)
		{
			return (lhs <=> rhs) == 0;
		}
		friend bool operator!= (const VectorItt& lhs, const VectorItt& rhs)
		{
			return (lhs <=> rhs) != 0;
		}
#pragma endregion IttOperator

	};
#pragma endregion IteratorClass


public:

#pragma region TypeDefs
	using iterator = VectorItt<T, 1>;
	using const_iterator = VectorItt<const T, 1>;
	using reverse_iterator = VectorItt<T, -1>;
	using const_reverse_iterator = VectorItt<const T, -1>;
	using value_type = T;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
#pragma endregion TypeDefs

#pragma region Constructor

	~Vector() noexcept
	{
		for (size_t i = 0; i < _size; ++i)
		{
			_pointer[i].~T();
		}
		_allocator.deallocate(_pointer, _capacity);
		CHECK

	}

	Vector() noexcept : _size(0), _capacity(0), _pointer(nullptr)
	{
		CHECK
	}

	Vector(const Vector& other) //copy
	{
		_size = 0;
		_capacity = other._size;
		_pointer = _allocator.allocate(_capacity);

		try
		{
			for (; _size < other._size; ++_size)
				new(_pointer + _size) T(other[_size]);
		}
		catch (const std::exception&)
		{
			for (--_size; _size < 0; --_size)
				_pointer[_size]. ~T();

			_allocator.deallocate(_pointer, _capacity);
			throw;
		}

		CHECK
	}

	Vector(Vector&& other) noexcept
	{
		_size = other._size;
		_capacity = other._capacity;
		_pointer = other._pointer;

		other._size = 0;
		other._capacity = 0;
		other._pointer = nullptr;

		CHECK
	}
	Vector(const char* other)
	{
		_size = 0;
		_capacity = std::strlen(other);
		_pointer = _allocator.allocate(_capacity);
		auto ptr = other;
		try
		{
			for (; ptr[_size] != '\0'; ++_size)
				new(_pointer + _size) T(other[_size]);
		}
		catch (const std::exception&)
		{
			for (--_size; _size < 0; --_size)
				_pointer[_size]. ~T();

			_allocator.deallocate(_pointer, _capacity);
			throw;
		}
		CHECK

	}
#pragma endregion Constructor

#pragma region Operator
	Vector& operator= (const Vector& other) // assignment 
	{
		if (this == &other)
		{
			return *this;
		}
		if (_capacity < other._size)
		{
			_capacity = other._capacity;
			delete[] _pointer;
			_pointer = new T[_capacity];
		}
		_size = other._size;

		for (size_t i = 0; i < _size; i++)
		{
			_pointer[i] = other._pointer[i];
		}

		CHECK

			return *this;
	}
	Vector& operator=(Vector&& other) noexcept
	{
		if (this != &other)
		{
			this->~Vector();
			_capacity = other._capacity;
			_size = other._size;
			_pointer = other._pointer;


			other._capacity = 0;
			other._size = 0;
			other._pointer = nullptr;
		}

		CHECK
			return *this;
	}
	T& operator[](size_t i)
	{
		return _pointer[i];
	}
	const T& operator[](size_t i) const
	{
		return  _pointer[i];
	}
	T& at(size_t i)
	{
		if (i < _size)
		{
			return _pointer[i];
		}

		throw std::out_of_range("Index out for rage");

	}
	const T& at(size_t i) const
	{
		if (i < _size)
		{
			return _pointer[i];
		}

		throw std::out_of_range("Index out for rage");
	}
	T* data() noexcept
	{
		return _pointer;
	}
	const T* data() const noexcept
	{
		return _pointer;
	}
#pragma endregion Operator

#pragma region Iterator

	iterator begin() noexcept
	{
		return iterator(_pointer);
	}
	iterator end() noexcept
	{
		return iterator(_pointer + _size);
	}
	const_iterator begin() const noexcept
	{
		return const_iterator(_pointer);
	}
	const_iterator cbegin() const noexcept
	{
		return const_iterator(_pointer);
	}
	const_iterator end() const noexcept
	{
		return const_iterator(_pointer + _size);
	}
	const_iterator cend() const noexcept
	{
		return const_iterator(_pointer + _size);
	}

#pragma region ReverseIterator

	reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(_pointer + _size - 1);
	}
	reverse_iterator rend() noexcept
	{
		return reverse_iterator(_pointer - 1);
	}
	const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(_pointer + _size - 1);
	}
	const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(_pointer + _size - 1);

	}
	const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(_pointer - 1);

	}
	const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(_pointer - 1);
	}
#pragma endregion ReverseIterator


#pragma endregion Iterator

public:

	size_t size() const noexcept
	{
		return _size;
	}
	size_t capacity() const noexcept
	{
		return _capacity;
	}

	void reserve(size_t n)
	{
		if (n <= _capacity)
		{
			return;
		}
		if (_pointer == nullptr)
		{
			_pointer = _allocator.allocate(n);
		}
		else
		{
			T* temp = _allocator.allocate(n);
			size_t i = 0;
			try
			{
				for (; i < _size; ++i)
				{
					new(temp + i) T(_pointer[i]);
					_pointer[i].~T();
				}
				_allocator.deallocate(_pointer, _capacity);
				_pointer = temp;
			}
			catch (const std::exception&)
			{
				for (; i != 0; --i)
				{
					temp[i].~T();
				}
				_allocator.deallocate(temp, n);
				throw;
			}

		}
		_capacity = n;

		CHECK
	}

	void shrink_to_fit()
	{
		if (_size == _capacity)
		{
			return;
		}

		T* temp = _allocator.allocate(_size);

		for (size_t i = 0; i < _size; ++i)
		{
			new(temp + i) T(_pointer[i]);
		}

		this->~Vector();
		_capacity = _size;
		_pointer = temp;

	}

	void push_back(const T& c)
	{
		if (_size == _capacity)
		{
			reserve(_capacity * 2 + 1);
		}


		new(_pointer + _size) T(c);
		++_size;

		/*try
		{
			new(_pointer + _size) T(c);
			++_size;

		}
		catch (const std::exception&)
		{
			throw;
		}*/
	}

	void push_back(T&& c)
	{
		if (_size == _capacity)
		{
			reserve(_capacity * 2 + 1);
		}

		new(_pointer + _size) T(std::move(c));
		++_size;

		//try
		//{
		//	new(_pointer + _size) T(std::move(c));
		//	++_size;

		//}
		//catch (const std::exception&)
		//{
		//	throw;
		//}
	}

	void resize(size_t n)
	{

		if (n > _capacity)
		{
			reserve(n);
		}

		for (size_t i = _size; i < n; i++)
		{
			new(_pointer + i) T();
		}

		_size = n;

		// Ändrar size() till n, om n>size() så fylls det på med T()
	}
#pragma region GlobalFunctions

	friend auto operator<=> (const Vector& lhs, const Vector& rhs)
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
	friend bool operator ==(const Vector& lhs, const Vector& other)
	{
		if (lhs._size != other._size)
		{
			return false;
		}
		return (lhs <=> other) == 0;
	}
	friend bool operator !=(const Vector& lhs, const Vector& other)
	{
		if (lhs._size != other._size)
		{
			return true;
		}
		return (lhs <=> other) != 0;
	}

	bool Invariant() const
	{
		if (_capacity == 0)
		{
			if (_pointer != nullptr)
			{
				return false;
			}
		}


		if (_size > _capacity)
		{
			return false;
		}

		//for (size_t i = 0; i < _size; i++)
		//{
		//	if (_pointer[i] == nullptr)
		//	{
		//		return false;
		//	}
		//}


		return true;
	}

	friend std::ostream& operator<<(std::ostream& cout, const Vector& other)
	{
		for (size_t i = 0; i < other.size(); ++i)
			cout << other[i];
		return cout;

	}


	friend void swap(Vector <T>& lhs, Vector<T>& rhs)
	{
		size_t sizeTemp = lhs._size;
		size_t capacityTemp = lhs._capacity;
		T* tempPointer = lhs._pointer;

		lhs._size = rhs._size;
		lhs._capacity = rhs._capacity;
		lhs._pointer = rhs._pointer;

		rhs._size = sizeTemp;
		rhs._capacity = capacityTemp;
		rhs._pointer = tempPointer;
	}
#pragma endregion GlobalFunctions


};
