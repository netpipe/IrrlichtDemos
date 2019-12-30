#ifdef SYNCSYS
#pragma once

#ifndef DONT_INCLUDE_HEADERS
#include "Portable.h"
#include <map>
#endif


namespace helpers
{
//!threadsafe (but not exceptionsafe!) std::map
template <class TKey, class TVal> class SecureMap : private std::map<TKey, TVal>
{
	MY_CRITICALSECTION Critic;

public:

	typedef typename std::map<TKey, TVal>::iterator MapIterator;
	typedef typename std::map<TKey, TVal>::const_iterator cMapIterator;
	typedef typename std::map<TKey, TVal>::size_type MapSizeType;
	typedef typename std::map<TKey, TVal>::value_type MapValueType;

	SecureMap(void)		{ }
	~SecureMap()		{ }

	//! calls lock of the criticalsection /mutex
	void lock(void)						{ Critic.Lock(); }
	//! calls unlock of the criticalsection /mutex
	void unlock(void)					{ Critic.Unlock(); }

	//! calls lock, std::map::clear and unlock
	void clear(bool lockit)				{ if(lockit) lock(); std::map<TKey, TVal>::clear();		if(lockit) unlock(); }
	//!calls std::map::empty and returns the result
	bool empty(void) const				{ return std::map<TKey, TVal>::empty(); }
	//!calls std::map::size and returns the result
	MapSizeType size(void) const		{ return std::map<TKey, TVal>::size(); }


	//!calls lock, std::map::begin and returns the result
	cMapIterator begin(bool lockit) const					{ if(lockit) lock(); return std::map<TKey, TVal>::begin(); }
	//!calls lock, std::map::find and returns the result
	cMapIterator find(const TKey& key, bool lockit) const	{ if(lockit) lock(); return std::map<TKey, TVal>::find(key); }
	//!calls std::map::end and returns the result
	cMapIterator end(void) const							{ return std::map<TKey, TVal>::end(); }

	//!calls lock, std::map::begin and returns the result
	MapIterator begin(bool lockit)					{ if(lockit) lock(); return std::map<TKey, TVal>::begin(); }
	//!calls lock, std::map::find and returns the result
	MapIterator find(const TKey& key, bool lockit)	{ if(lockit) lock(); return std::map<TKey, TVal>::find(key); }
	//!calls std::map::end and returns the result
	MapIterator end(void)							{ return std::map<TKey, TVal>::end(); }
	//!calls std::map::erase. Container should be locked before.
	void erase(MapIterator it) 						{ std::map<TKey, TVal>::erase(it); }

	//!calls lock, std::map::end, unlock and returns the result
	std::pair<MapIterator, bool> insert(const MapValueType& e, bool lockit)	{ if(lockit) lock(); std::pair<MapIterator, bool> it = std::map<TKey, TVal>::insert(e); if(lockit) unlock(); return it; }
};

};
#endif
