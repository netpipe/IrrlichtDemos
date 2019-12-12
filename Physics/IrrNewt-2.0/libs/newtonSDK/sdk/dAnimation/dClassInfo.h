
/* Copyright (c) <2009> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

#ifndef __DCLASS_INFO_H__
#define __DCLASS_INFO_H__

#include "dRtti.h"

class dClassInfo: public dRefCounter
{
	public:
	dClassInfo(void);
	virtual ~dClassInfo(void);
	virtual unsigned GetTypeId () const;
	virtual bool IsType (unsigned typeId) const;
	static unsigned GetRttiType();


	private:
	static dRtti m_rtti;
};


inline dClassInfo::dClassInfo(void)
{
}

inline dClassInfo::~dClassInfo(void)
{
}

inline unsigned dClassInfo::GetTypeId () const
{
	return m_rtti.GetTypeId ();
}

inline unsigned dClassInfo::GetRttiType()
{
	return m_rtti.GetTypeId();
}


inline bool dClassInfo::IsType (unsigned typeId) const
{
	return m_rtti.IsTypeID (typeId);
}

#endif