/*
 * Copyright (c) 2007, elvman
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY elvman ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

//#incldue "stdafx.h" start
#include <iostream>
#include <tchar.h>
//#incldue "stdafx.h" end

#include <string>
#include <irrlicht.h>

//#incldue "stdafx.h" start
#pragma comment(lib, "Irrlicht.lib")
//#incldue "stdafx.h" end

using namespace irr;

class RealisticWaterSceneNode: public scene::ISceneNode, video::IShaderConstantSetCallBack
{
public:
	RealisticWaterSceneNode(scene::ISceneManager* sceneManager, f32 width, f32 height, video::ITexture* bumpTexture, ITimer* timer, 
		core::dimension2di renderTargetSize=core::dimension2di(512,512),scene::ISceneNode* parent = 0, s32 id = -1);
	virtual ~RealisticWaterSceneNode();

	// frame
	virtual void OnRegisterSceneNode();

	virtual void OnAnimate(u32 timeMs);

	// renders terrain
	virtual void render();
    
	// returns the axis aligned bounding box of terrain
	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

	virtual void setWindForce(const f32 windForce);
	virtual void setWindDirection(const core::vector2df& windDirection);
	virtual void setWaveHeight(const f32 waveHeight);

	virtual void setWaterColor(const video::SColorf& waterColor);
	virtual void setColorBlendFactor(const f32 colorBlendFactor);

private:

	scene::ICameraSceneNode*		Camera;
	scene::ISceneNode*				WaterSceneNode;

	video::IVideoDriver*			VideoDriver;
	scene::ISceneManager*			SceneManager;
	ITimer*							Timer;
	
	core::dimension2d<f32>			Size;

	s32								ShaderMaterial;

	video::ITexture*				RefractionMap;
	video::ITexture*				ReflectionMap;

	f32								WindForce;
	core::vector2df					WindDirection;
	f32								WaveHeight;

	video::SColorf					WaterColor;
	f32								ColorBlendFactor;
};
