#pragma once

#include <iostream>

#include <cassert>

#define CHECK assert(Invariant());

template<class T>
class Flist {
    class Node;

    class Link {
        friend class Flist<T>;

        Link* _next;
        Link() : _next(nullptr) {}
        void InsertAfter(Node* toInsert) {
            toInsert->_next = _next;
            _next = toInsert;
        }
        //Return pointer to node to delete
        Node* DeleteAfter() { //Return pointer to node to delete
            Link* temp = _next;
            _next = _next->_next;
            return static_cast<Node*>(temp);
        }
        //[beforeFirst+1, atLast] is inserted (moved) to after this
        void Splice(Link* beforeFirst, Link* atLast) {
            if (beforeFirst == atLast || beforeFirst->_next == atLast)
                return; //Below do not work for empty insert
            Link* afterSplice = _next;
            _next = beforeFirst->_next;
            beforeFirst->_next = atLast->_next;
            atLast->_next = afterSplice;
            //std::swap(_next, atLast->_next);
            //std::swap(_next, beforeFirst->_next);
        }

    };

    class Node : Link {
        friend class Flist<T>;

        T _data;
    public:
        Node(const T& data) :_data(data) {};
    };

    template<class X> //X is T or const T
    class FlistIter {
        friend class Flist<T>;

        Node* _ptr;
    public:

#pragma region types
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using reference = X&;
        using pointer = X*;
#pragma endregion types

#pragma region Constructors && Assignment
        ~FlistIter() = default;
        FlistIter(const Link* node = nullptr)
            :_ptr(static_cast<Node*>(const_cast<Link*>(node))) {}
        FlistIter(const FlistIter&) = default;
        FlistIter& operator=(const FlistIter&) = default;

        //Detta kommer att tas upp senare på kursen
        template<class Y, class = std::enable_if_t<!std::is_const_v<Y>>>
        FlistIter(const FlistIter<Y>& rhs) : _ptr(rhs._ptr) {}
        template<class Y, class = std::enable_if_t<!std::is_const_v<Y>>>
        FlistIter& operator=(const FlistIter<Y>& rhs) {
            _ptr = rhs._ptr;
        }
#pragma endregion Constructors && Assignment

#pragma region Element access
        X& operator*() {
            return _ptr->_data;
        }
        X* operator->() {
            return &_ptr->_data;
        }
#pragma endregion Element access

#pragma region Modifiers
        FlistIter& operator++() {
            _ptr = static_cast<Node*>(_ptr->_next);
            return *this;
        }
        FlistIter operator++(int) {
            auto temp(*this);
            operator++();
            return temp;
        }
#pragma endregion Modifiers

#pragma region nonmembers
        friend bool operator==(const FlistIter& lhs, const FlistIter& rhs) = default;
        friend bool operator!=(const FlistIter& lhs, const FlistIter& rhs) = default;

        //friend bool operator==(const FlistIter& lhs, const FlistIter& rhs) {
        //    return lhs._ptr == rhs._ptr;
        //}
        //friend bool operator!=(const FlistIter& lhs, const FlistIter& rhs) {
        //    return lhs._ptr != rhs._ptr;
        //}
#pragma endregion nonmembers

    };

public:
#pragma region Testfunktioner
    bool Invariant() const {
        size_t i = 0;
        for (auto p = &_head; p != nullptr; p = p->_next)
            if (++i == std::numeric_limits<size_t>::max())
                return false;
        return true;
    }
    Flist(const char* str) :Flist() { //debug
        const char* ptr = str;
        while (*ptr != '\0')
            ++ptr;
        for (; ptr >= str; --ptr)
            push_front(*ptr);
        CHECK
    }
    friend static
        std::ostream& operator<<(std::ostream& cout, const Flist& l) {
        for (auto it = l.begin(); it != l.end(); ++it)
        {
            cout << *it << " ";
        }
        cout << std::endl;
        return cout;
    }
#pragma endregion Testfunktioner

private:
    Link _head;

public:
#pragma region typedef //only iterators really neccessary!
    using value_type = T;
    using pointer = T*;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = size_t;
    typedef const T* const_pointer;
    using iterator = FlistIter<T>;
    using const_iterator = FlistIter<const T>;
#pragma endregion typedef

#pragma region Constructors && Assignment
    //Also used as help function and must leave a correct empty list
    ~Flist() {
        while (_head._next != nullptr)
            pop_front();
        CHECK
    }
    Flist() = default;
    Flist(const Flist& other) {
        const Link* source = other._head._next;
        Link** dest = &(_head._next);
        while (source != nullptr) {
            Link* node = new Node(static_cast<const Node*>(source)->_data);
            (*dest) = node;
            dest = &(node->_next);
            source = source->_next;
        }
        //(*dest) = nullptr;
        CHECK
            //(*dest) = nullptr;
    }
#pragma endregion Constructors && Assignment

#pragma region element access
    T& front() {
        return static_cast<Node*>(_head._next)->_data;
    }
    const T& front() const {
        return static_cast<Node*>(_head._next)->_data;
    }
#pragma endregion element access

#pragma region iterators
    iterator begin() { return iterator(_head._next); }
    const_iterator begin() const { return const_iterator(_head._next); }
    const_iterator cbegin() const { return const_iterator(_head._next); }
    iterator end() { return iterator(nullptr); }
    const_iterator end() const { return const_iterator(nullptr); }
    const_iterator cend() const { return const_iterator(nullptr); }
#pragma endregion iterators

#pragma region Capacity
    bool empty() const noexcept { return begin() == end(); }
    size_t size() const noexcept {
        return std::distance(cbegin(), cend());
    }
#pragma endregion 

#pragma region Modifiers
    //returns iterator to new element
    iterator insert_after(const iterator& pos, const T& value) {
        pos._ptr->InsertAfter(new Node(value));
        return pos._ptr->_next;
    }
    //returns iterator to the element after
    iterator erase_after(const iterator& pos) {
        iterator temp = pos;
        ++temp;
        delete pos._ptr;
        return temp;
    }
    void push_front(const T& toInsert) {
        _head.InsertAfter(new Node(toInsert));
    }
    void pop_front() {
        delete (_head.DeleteAfter());
    }
    void swap(Flist& other) {
        std::swap(_head._next, other._head._next);
    }
#pragma endregion Modifiers

#pragma region nonmembers
#if true //C++20
    friend auto operator<=>(const Flist& lhs, const Flist& rhs) {
        auto lIt = lhs.begin(); auto rIt = rhs.begin();
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
    //friend bool operator==(const Flist& lhs, const Flist& rhs) = default;
    friend bool operator==(const Flist& lhs, const Flist& rhs) {
        return (lhs <=> rhs) == 0;
    }
    friend bool operator!=(const Flist& lhs, const Flist& rhs) {
        return (lhs <=> rhs) != 0;
    }

#else //not C++20
    friend bool operator==(const Flist& lhs, const Flist& rhs) {
        auto lIt = lhs.begin(); auto rIt = rhs.begin();
        for (; lIt != lhs.end() && rIt != rhs.end(); ++lIt, ++rIt)
            if (*lIt != *rIt)
                return false;
        return (lIt == lhs.end()) && (rIt == rhs.end());  //both ended => equal
    }
    friend bool operator!=(const Flist& lhs, const Flist& rhs) {
        return !(lhs == rhs);
    }
    friend bool operator<(const Flist& lhs, const Flist& rhs) {
        auto lIt = lhs.begin(); auto rIt = rhs.begin();
        for (; lIt != lhs.end() && rIt != rhs.end(); ++lIt, ++rIt)
            if (*lIt < *rIt)
                return true;
            else if (*lIt > *rIt)
                return false;
        return (rIt != rhs.end());  //if lhs shorter it is less
    }
    friend bool operator>(const Flist& lhs, const Flist& rhs) {
        return (rhs < lhs);
    }
    friend bool operator<=(const Flist& lhs, const Flist& rhs) {
        return !(rhs < lhs);
    }
    friend bool operator>=(const Flist& lhs, const Flist& rhs) {
        return !(lhs < rhs);
    }
#endif

    friend void swap(Flist& lhs, Flist& rhs) {
        lhs.swap(rhs);
    }
#pragma endregion nonmembers

};
