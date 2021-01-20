#pragma once
template <class T>
class Link
{
	friend class List<T>;
	Link* _next, *_prev;
	Link() : _next(this), _prev(this) {}
};

