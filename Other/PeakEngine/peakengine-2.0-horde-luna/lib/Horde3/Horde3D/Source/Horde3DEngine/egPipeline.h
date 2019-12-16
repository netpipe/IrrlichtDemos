// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
// --------------------------------------
// Copyright (C) 2006-2008 Nicolas Schulz
//
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// *************************************************************************************************

#ifndef _egPipeline_H_
#define _egPipeline_H_

#include "egPrerequisites.h"
#include "egRendererBase.h"
#include "egResource.h"
#include "egMaterial.h"
#include <string>
#include <vector>
using namespace std;

struct XMLNode;


struct PipelineCommands
{
	enum List
	{
		SwitchTarget,
		BindBuffer,
		ClearTarget,
		DrawGeometry,
		DrawOverlays,
		DrawQuad,
		DoForwardLightLoop,
		DoDeferredLightLoop,
		SetUniform
	};
};

class PCParam
{
public:
	virtual ~PCParam() { }
};

class PCBoolParam : public PCParam
{
protected:
	bool	_value;
public:
	PCBoolParam( bool value ) { _value = value; }
	bool get() { return _value; }
	void set( bool value ) { _value = value; }
};

class PCIntParam : public PCParam
{
protected:
	int		_value;
public:
	PCIntParam( int value ) { _value = value; }
	int get() { return _value; }
	void set( int value ) { _value = value; }
};

class PCFloatParam : public PCParam
{
protected:
	float	_value;
public:
	PCFloatParam( float value ) { _value = value; }
	float get() { return _value; }
	void set( float value ) { _value = value; }
};

class PCStringParam : public PCParam
{
protected:
	string	_value;
public:
	PCStringParam( const string &value ) { _value = value; }
	const string &get() { return _value; }
	void set( const string &value ) { _value = value; }
};


struct RenderingOrder
{
	enum List
	{
		None = 0,
		FrontToBack,
		BackToFront,
		StateChanges
	};
};


struct PipelineCommand
{
	PipelineCommands::List	command;
	vector< void * >		refParams;		// Pointer to object
	vector< PCParam * >		valParams;		// Newly created object
	vector< PResource >		resParams;		// Pointers to used resources


	PipelineCommand( PipelineCommands::List	command )
	{
		this->command = command;
	}
};


struct PipelineStage
{
	string						id;
	bool						enabled;
	PMaterialResource			matLink;
	vector< PipelineCommand >	commands;

	PipelineStage() : matLink( 0x0 ) {}
};


struct RenderTarget
{
	string						id;
	bool						hasDepthBuf;
	uint32						numColBufs;
	RenderBufferFormats::List	format;
	bool						bilinear;
	uint32						samples;	// Multisampled if > 0
	uint32						width, height;
	float						scale;		// Scale factor for FB width and height
	RenderBuffer				rendBuf;
	RenderBuffer				rendBufMultisample;

	RenderTarget()
	{
		hasDepthBuf = false;
		numColBufs = 0;
	}
};


class PipelineResource : public Resource
{
private:

	vector< RenderTarget >		_renderTargets;
	vector< PipelineStage >		_stages;
	
	bool raiseError( const string &msg, int line = -1 );
	const string parseStage( XMLNode &node, PipelineStage &stage );

	void addRenderTarget( const string &id, bool depthBuffer, uint32 numBuffers,
						  RenderBufferFormats::List format, bool bilinear, uint32 samples,
						  uint32 width, uint32 height, float scale );
	RenderTarget *findRenderTarget( const string &id );
	bool createRenderTargets();
	void destroyRenderTargets();

public:

	static Resource *factoryFunc( const string &name, int flags )
		{ return new PipelineResource( name, flags ); }
	
	PipelineResource( const string &name, int flags );
	~PipelineResource();
	
	void initDefault();
	void release();
	bool load( const char *data, int size );
	void resize();

	bool setStageActivation( const string &stageName, bool enabled );
	bool getRenderTargetData( const string &target, int bufIndex, int *width, int *height,
							  int *compCount, float *dataBuffer, int bufferSize );

	friend class ResourceManager;
	friend class Renderer;
};

typedef SmartResPtr< PipelineResource > PPipelineResource;

#endif // _egPipeline_H_
