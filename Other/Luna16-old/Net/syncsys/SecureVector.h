#ifdef SYNCSYS
#pragma once

#ifndef DONT_INCLUDE_HEADERS
#include "Portable.h"
#include <vector>
#endif


namespace helpers
{
//!threadsafe (but not exceptionsafe!) std::vector
template <class T> class SecureVector : private std::vector<T>
{
	MY_CRITICALSECTION Critic;

public:
	typedef typename std::vector<T>         p;
	typedef typename p::iterator			iterator;
	typedef typename p::const_iterator		citerator;
	typedef typename p::size_type			size_type;

	SecureVector(void)		{ }
	~SecureVector()			{ }

	//! calls lock of the criticalsection /mutex
	void lock(void)	const						{ Critic.Lock(); }
	//! calls unlock of the criticalsection /mutex
	void unlock(void) const						{ Critic.Unlock(); }


	//! calls lock, std::vector::begin and returns the result
	citerator begin(bool lockit) const 					{ if(lockit) lock(); return p::begin(); }
	//!calls std::vector::end and returns the result
	citerator end(void) const							{ return p::end(); }
	//! calls lock, std::vector::begin and returns the result
	iterator begin(bool lockit)							{ if(lockit) lock(); return p::begin(); }
	//!calls std::vector::end and returns the result
	iterator end(void)									{ return p::end(); }
	//!calls std::vector::erase. Container should be locked before
	iterator erase(iterator it) 						{ return p::erase(it); }
	//! calls lock, std::vector::clear and unlock
	void clear(bool lockit)								{ if(lockit) lock(); p::clear();						if(lockit) unlock(); }
	//! calls lock, std::vector::push_back and unlock
	void push_back(const T& e, bool lockit, bool other)	{ if(lockit) lock(); if(other && p::size() == p::capacity()) p::reserve(2*size() + 512); p::push_back(e);	if(lockit) unlock(); }
	//! calls lock, std::vector::push_front and unlock
	void push_front(const T& e, bool lockit)			{ if(lockit) lock(); p::push_front(e);					if(lockit) unlock(); }
	//! calls lock, std::vector::remove and unlock
	void remove(const T& e, bool lockit)				{ if(lockit) lock(); p::remove(e);						if(lockit) unlock(); }
	//! calls lock, std::vector::swap and unlock
	void swap(p &vector, bool lockit)	            	{ if(lockit) lock(); p::swap(vector);					if(lockit) unlock(); }
	//! calls std::vector::size and returns the result
	size_type size(void) const							{ return p::size(); }
	//! calls std::vector::empty and returns the result
	bool empty(void) const								{ return p::empty(); }


	// calls lock, std::vector::insert and returns the result
	iterator insert(iterator _Where, const T& _Val, bool lockit)
	{
		if(lockit) lock();
		return std::vector<T>::insert(_Where, _Val);
	}
};

};
#endif
