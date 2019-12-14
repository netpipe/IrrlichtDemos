/////////////////////////////////////////////////////////////////////////////
// Name:        alchemediaTextCache.h
// Purpose:     
// Author:      Julio Jerez
// Modified by: 
// Created:     22/05/2010 08:02:08
// RCS-ID:      
// Copyright:   Copyright (c) <2010> <Newton Game Dynamics>
// License:     
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely
/////////////////////////////////////////////////////////////////////////////

// alchemediaTextCache.h : interface of the alchemediaTextCache class
//

#ifndef __ALCHEMEDIA_TEX_CACHE_H__
#define __ALCHEMEDIA_TEX_CACHE_H__

#include "alchemediaScene.h"


class alchemediaTextureResource: public dRefCounter
{
	public:
	GLuint m_textID;
};

class alchemediaTextureCache: public dTree<alchemediaTextureResource, unsigned>  
{
	public:
	void LoadTexture (const char* const fileName, dTextureNodeInfo* const texture);
	void UnloadTexture (dTextureNodeInfo* const texture);
};




#endif