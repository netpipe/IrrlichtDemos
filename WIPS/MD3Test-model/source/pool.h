#ifndef __POOL_H_INCLUDED__
#define __POOL_H_INCLUDED__

#include <irrTypes.h>

namespace irr
{
namespace core
{

class Pool : public virtual IReferenceCounted
{
public:
    Pool(u32 size)
        :Buffer(NULL), Pointer(NULL), Len(size), Pos(0), Avail(Len)
    {
        _IRR_DEBUG_BREAK_IF(Len == 0) // Bad init.
        Buffer = (void *) new c8[Len];
        _IRR_DEBUG_BREAK_IF(Buffer == NULL) // No memory.
        Pointer = Buffer;
    }

    Pool(void * p, u32 size)
        :Buffer(p), Pointer(NULL), Len(size), Pos(0), Avail(Len)
    {
        _IRR_DEBUG_BREAK_IF(Len == 0) // Bad init.
        _IRR_DEBUG_BREAK_IF(Buffer == NULL) // No memory.
        Pointer = Buffer;
    }

    ~Pool()
    {
        if (Buffer)
            delete [] (c8*)Buffer;
    }

    bool seek(u32 finalPos)
    {
		if (finalPos > Len)
			return false;

		Pos = finalPos;
		Pointer = (void *) (((c8 *)Buffer) + Pos);
		Avail = Len - Pos;

		return true;
	}

	bool offset(s32 off) {
	    if (off == 0)
	        return true;

        u32 newpos;
        if (off > 0)
            newpos = Pos + (u32)off;
        else {
            newpos = (u32)((-1)*off);
            if (Pos < newpos)
                return false;
            newpos = Pos - newpos;
        }
        return seek(newpos);
	}

	void * buffer() const { return Buffer; }

	void * pointer() const { return Pointer; }

	u32 size() const { return Len; }

    u32 available() const { return Avail; }

	u32 pos() const { return Pos; }

private:
    void *Buffer;
    void *Pointer;
    u32 Len;
    u32 Pos;
    u32 Avail;
};


} // namespace core

} // namespace irr

#endif // __POOL_H_INCLUDED__

