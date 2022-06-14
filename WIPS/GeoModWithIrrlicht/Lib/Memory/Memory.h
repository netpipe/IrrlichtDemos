/*
===============================================================================
	File:	Memory.h
	Desc:	Memory management.
===============================================================================
*/

#ifndef __MX_MEMORY_H__
#define __MX_MEMORY_H__

#include <Lib/Memory/Pool/Pool.h>
#include <Lib/Memory/FreeList/FreeList.h>

namespace mix {

//
//	mxMemory
//
class mxMemory {
public:
	virtual		~mxMemory() {}

	virtual void *	Allocate( size_t  Num ) = 0;
	virtual void *	Reallocate( void* old, size_t  Num ) = 0;
	virtual void	Free( void* mem ) = 0;
};

}//end of namespace mix

#endif /* ! __MX_MEMORY_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
