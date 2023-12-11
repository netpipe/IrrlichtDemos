#ifdef SYNCSYS
#pragma once

#ifndef DONT_INCLUDE_HEADERS
#include "Portable.h"
#include <list>
#endif


namespace helpers
{
//!threadsafe (but not exceptionsafe!) std::list
template <class T> class SecureList : private std::list<T>
{
	MY_CRITICALSECTION Critic;

	void operator = (const SecureList&)		{ }
public:
	typedef typename std::list<T>		p;
	typedef typename p::iterator		iterator;
	typedef typename p::const_iterator	citerator;
	typedef typename p::size_type		size_type;

	SecureList(const p &c)			        { p::insert(begin(true), c.begin(), c.end()); unlock(); }
	SecureList(const SecureList &c) : p()	{ p::insert(begin(true), c.begin(true), c.end()); c.unlock(); unlock(); }
	SecureList(void)				        { }
	~SecureList()					        { }

	//! calls lock of the criticalsection /mutex
	void lock(void) const						{ Critic.Lock(); }
	//! calls unlock of the criticalsection /mutex
	void unlock(void) const						{ Critic.Unlock(); }


	//! calls lock, std::list::begin and returns the result
	citerator begin(bool lockit) const 					{ if(lockit) lock(); return p::begin(); }
	//!calls std::list::end and returns the result
	citerator end(void) const							{ return p::end(); }
	//! calls lock, std::list::begin and returns the result
	iterator begin(bool lockit)							{ if(lockit) lock(); return p::begin(); }
	//!calls std::list::end and returns the result
	iterator end(void)									{ return p::end(); }
	//!calls std::list::erase. Container should be locked before
	iterator erase(iterator it) 						{ return p::erase(it); }
	//! calls lock, std::list::clear and unlock
	void clear(bool lockit)								{ if(lockit) lock(); p::clear();										if(lockit) unlock(); }
	//! calls lock, std::list::push_back and unlock
	void push_back(const T& e, bool lockit)				{ if(lockit) lock(); p::push_back(e);									if(lockit) unlock(); }
	//! calls lock, std::list::push_front and unlock
	void push_front(const T& e, bool lockit)			{ if(lockit) lock(); p::push_front(e);									if(lockit) unlock(); }
	//! calls lock, std::list::remove and unlock
	void remove(const T& e, bool lockit)				{ if(lockit) lock(); p::remove(e);										if(lockit) unlock(); }
	//! calls lock, std::list::sort and unlock
	void sort(bool lockit)								{ if(lockit) lock(); p::sort(); 										if(lockit) unlock(); }
	//! calls lock, std::list::unique and unlock
	void unique(bool lockit)							{ if(lockit) lock(); p::unique(); 										if(lockit) unlock(); }
	//! calls lock, std::list::splice and unlock
	void spliceIn(p &list, bool lockit)					{ if(lockit) lock(); list.splice(list.begin(), *this);					if(lockit) unlock(); }
	//! calls lock, std::list::merge and unlock
	void merge(p &list, bool lockit)					{ if(lockit) lock(); p::merge(list);									if(lockit) unlock(); }
	//! calls lock for both, std::list::splice and unlock
	void splice(SecureList<T> &list, bool lockit)		{ if(lockit) { lock(); list.lock(); } p::splice(begin(false), list);	if(lockit) { list.unlock(); unlock(); } }
	//! calls lock for both, std::list::merge and unlock
	void merge(SecureList<T> &list, bool lockit)		{ if(lockit) { lock(); list.lock(); } p::merge(list);					if(lockit) { list.unlock(); unlock(); } }
	//! calls std::list::size and returns the result
	size_type size(void) const			                { return p::size(); }
	//! calls std::list::empty and returns the result
	bool empty(void) const								{ return p::empty(); }


	//template<class _Iter> void insert(iterator _Where, _Iter _First, _Iter _Last)
	//{
	//	std::list<T>::insert(_Where, _First, _Last);
	//}
	//! calls lock, std::list::insert and returns the result
	iterator insert(iterator _Where, const T& _Val, bool lockit)
	{
		if(lockit) lock();
		return p::insert(_Where, _Val);
	}
};

};
#endif
