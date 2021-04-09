#pragma once


template <class T>
class SharedPtr
{
	T* _ptr;
	size_t* _counter;
	void add(T* other)
	{
		if (other != nullptr)
		{
			if (_counter == nullptr)
			{
				_counter = new size_t(1);
			}
			else
			{
				++*_counter;
			}
		}
	}

	void remove()
	{
		if (_counter != nullptr)
		{
			--* _counter;
			if (*_counter == 0)
			{
				delete _ptr;
				delete _counter;
			}
		}
		_ptr = nullptr;
		_counter = nullptr;
	}

public:
	SharedPtr() : _ptr(nullptr), _counter(0) {}

	SharedPtr(T* other) : _ptr(other), _counter(nullptr)
	{
		add(_ptr);
		Invariant();
	}

	SharedPtr(const std::nullptr_t) : _ptr(nullptr), _counter(nullptr) {}
	SharedPtr(const SharedPtr& other) : _ptr(nullptr), _counter(nullptr)
	{
		if (this != &other)
		{
			_ptr = other._ptr;
			_counter = other._counter;
			add(_ptr);
		}
		Invariant();
	}
	SharedPtr(SharedPtr&& other) noexcept
	{
		if (this != &other)
		{
			_ptr = other._ptr;
			_counter = other._counter;
			add(_ptr);
			other.remove();
		}
		Invariant();
	}
	~SharedPtr()
	{
		remove();
		Invariant();
	}

	SharedPtr& operator=(const SharedPtr& other) noexcept
	{
		if (this != &other)
		{
			remove();
			_ptr = other._ptr;
			_counter = other._counter;
			add(_ptr);
		}
		Invariant();
		return *this;
	}

	bool operator==(const std::nullptr_t other) const
	{
		return _ptr == other;
	}
	bool operator<(const std::nullptr_t other) const
	{
		return _ptr < other;
	}
	bool operator==(const SharedPtr& other) const
	{
		return _ptr == other.get();
	}
	bool operator<(const SharedPtr& other) const
	{
		return _ptr < other.get();
	}

	T& operator*()
	{
		return *_ptr;
	}
	T* operator->()
	{
		return _ptr;
	}

	operator bool() const
	{
		return _ptr != nullptr;
	}

	T* get()
	{
		return _ptr;
	}	

	const T* get() const
	{
		return _ptr;
	}

	void reset()
	{
		remove();
	}

	size_t use_count()
	{
		if (_counter != nullptr)
		{
			return *_counter;

		}
		return 0;
	}

	bool Invariant()
	{
		if (_ptr == nullptr && _counter == nullptr)
		{
			return true;
		}
		if (_ptr != nullptr && _counter != nullptr)
		{
			return true;
		}

		return false;
	}

	void swap(SharedPtr& rhs) noexcept
	{
		std::swap(_ptr, rhs._ptr);
		std::swap(_counter, rhs._counter);
	}

};

template<class T>
void swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs)
{
	lhs.swap(rhs);
}

template< class T, class... Args >
SharedPtr<T> MakeShared(Args&&...args)
{
	return SharedPtr<T>(new T(std::forward<Args>(args)...));
}
