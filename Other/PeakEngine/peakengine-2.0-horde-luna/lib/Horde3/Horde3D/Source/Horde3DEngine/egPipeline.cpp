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

#include "egPipeline.h"
#include "egModules.h"
#include "egMaterial.h"
#include "utXMLParser.h"
#include "utPlatform.h"
#include <fstream>
using namespace std;

#include "utDebug.h"


PipelineResource::PipelineResource( const string &name, int flags ) :
	Resource( ResourceTypes::Pipeline, name, flags )
{
	initDefault();	
}


PipelineResource::~PipelineResource()
{
	release();
}


void PipelineResource::initDefault()
{
}


void PipelineResource::release()
{
	destroyRenderTargets();

	for( uint32 i = 0; i < _stages.size(); ++i )
	{
		for( uint32 j = 0; j < _stages[i].commands.size(); ++j )
		{
			for( uint32 k = 0; k < _stages[i].commands[j].valParams.size(); ++k )
			{
				delete _stages[i].commands[j].valParams[k];
			}

			for( uint32 k = 0; k < _stages[i].commands[j].resParams.size(); ++k )
			{
				Modules::resMan().removeResource( _stages[i].commands[j].resParams[k], false );
			}
		}
	}

	_renderTargets.clear();
	_stages.clear();
}


bool PipelineResource::raiseError( const string &msg, int line )
{
	// Reset
	release();
	initDefault();

	if( line < 0 )
		Modules::log().writeError( "Pipeline resource '%s': %s", _name.c_str(), msg.c_str() );
	else
		Modules::log().writeError( "Pipeline resource '%s' in line %i: %s", _name.c_str(), line, msg.c_str() );
	
	return false;
}


const string PipelineResource::parseStage( XMLNode &node, PipelineStage &stage )
{
	stage.id = node.getAttribute( "id", "" );
	
	if( _stricmp( node.getAttribute( "enabled", "true" ), "false" ) == 0 ||
		_stricmp( node.getAttribute( "enabled", "1" ), "0" ) == 0 )
		stage.enabled = false;
	else
		stage.enabled = true;

	if( strcmp( node.getAttribute( "link", "" ), "" ) != 0 )
	{
		uint32 mat = Modules::resMan().addResource(
			ResourceTypes::Material, node.getAttribute( "link" ), 0, false );
		stage.matLink = (MaterialResource *)Modules::resMan().resolveResHandle( mat );
	}
	
	int nodeItr1 = 0;
	XMLNode node1 = node.getChildNode();
	while( !node1.isEmpty() && node1.getName() != 0x0 )
	{
		if( strcmp( node1.getName(), "SwitchTarget" ) == 0 )
		{
			if( node1.getAttribute( "target" ) == 0x0 ) return "Missing SwitchTarget attribute 'target'";
			
			if( strcmp( node1.getAttribute( "target" ), "" ) == 0 )
			{
				stage.commands.push_back( PipelineCommand( PipelineCommands::SwitchTarget ) );
				(stage.commands.end() - 1)->refParams.push_back( 0x0 );
			}
			else
			{
				if( findRenderTarget( node1.getAttribute( "target" ) ) == 0x0 ) return "Reference to undefined render target in SwitchTarget";
				stage.commands.push_back( PipelineCommand( PipelineCommands::SwitchTarget ) );
				(stage.commands.end() - 1)->refParams.push_back( findRenderTarget( node1.getAttribute( "target" ) ) );
			}
		}
		else if( strcmp( node1.getName(), "BindBuffer" ) == 0 )
		{
			if( node1.getAttribute( "texUnit" ) == 0x0 || node1.getAttribute( "target" ) == 0x0 ||
				node1.getAttribute( "bufIndex" ) == 0x0 ) return "Missing BindBuffer attribute";
			if( findRenderTarget( node1.getAttribute( "target" ) ) == 0x0 ) return "Reference to undefined render target in BindBuffer";
			stage.commands.push_back( PipelineCommand( PipelineCommands::BindBuffer ) );
			(stage.commands.end() - 1)->refParams.push_back( findRenderTarget( node1.getAttribute( "target" ) ) );
			vector< PCParam * > &valParams = (stage.commands.end() - 1)->valParams;
			valParams.resize( 2 );
			valParams[0] = new PCFloatParam( (float)atof( node1.getAttribute( "texUnit" ) ) );
			valParams[1] = new PCFloatParam( (float)atof( node1.getAttribute( "bufIndex" ) ) );
		}
		else if( strcmp( node1.getName(), "ClearTarget" ) == 0 )
		{
			stage.commands.push_back( PipelineCommand( PipelineCommands::ClearTarget ) );
			
			vector< PCParam * > &valParams = (stage.commands.end() - 1)->valParams;
			valParams.resize( 9 );
			valParams[0] = new PCBoolParam( false );
			valParams[1] = new PCBoolParam( false );
			valParams[2] = new PCBoolParam( false );
			valParams[3] = new PCBoolParam( false );
			valParams[4] = new PCBoolParam( false );
			valParams[5] = new PCFloatParam( (float)atof( node1.getAttribute( "col_R", "0" ) ) );
			valParams[6] = new PCFloatParam( (float)atof( node1.getAttribute( "col_G", "0" ) ) );
			valParams[7] = new PCFloatParam( (float)atof( node1.getAttribute( "col_B", "0" ) ) );
			valParams[8] = new PCFloatParam( (float)atof( node1.getAttribute( "col_A", "0" ) ) );
			
			if( _stricmp( node1.getAttribute( "depthBuf", "false" ), "true" ) == 0 ||
				_stricmp( node1.getAttribute( "depthBuf", "0" ), "1" ) == 0 )
				((PCBoolParam *)valParams[0])->set( true );
			
			if( _stricmp( node1.getAttribute( "colBuf0", "false" ), "true" ) == 0 ||
				_stricmp( node1.getAttribute( "colBuf0", "0" ), "1" ) == 0 )
				((PCBoolParam *)valParams[1])->set( true );

			if( _stricmp( node1.getAttribute( "colBuf1", "false" ), "true" ) == 0 ||
				_stricmp( node1.getAttribute( "colBuf1", "0" ), "1" ) == 0 )
				((PCBoolParam *)valParams[2])->set( true );

			if( _stricmp( node1.getAttribute( "colBuf2", "false" ), "true" ) == 0 ||
				_stricmp( node1.getAttribute( "colBuf2", "0" ), "1" ) == 0 )
				((PCBoolParam *)valParams[3])->set( true );

			if( _stricmp( node1.getAttribute( "colBuf3", "false" ), "true" ) == 0 ||
				_stricmp( node1.getAttribute( "colBuf3", "0" ), "1" ) == 0 )
				((PCBoolParam *)valParams[4])->set( true );
		}
		else if( strcmp( node1.getName(), "DrawGeometry" ) == 0 )
		{
			if( node1.getAttribute( "context" ) == 0x0 ) return "Missing DrawGeometry attribute 'context'";
			stage.commands.push_back( PipelineCommand( PipelineCommands::DrawGeometry ) );
			(stage.commands.end() - 1)->valParams.push_back( new PCStringParam( node1.getAttribute( "context" ) ) );
			(stage.commands.end() - 1)->valParams.push_back( new PCStringParam( node1.getAttribute( "class", "" ) ) );
			
			string orderString = node1.getAttribute( "order", "" );
			int order = RenderingOrder::None;
			if( orderString == "FRONT_TO_BACK" ) order = RenderingOrder::FrontToBack;
			else if( orderString == "BACK_TO_FRONT" ) order = RenderingOrder::BackToFront;
			else if( orderString == "STATECHANGES" ) order = RenderingOrder::StateChanges;
			(stage.commands.end() - 1)->valParams.push_back( new PCIntParam( order ) );
		}
		else if( strcmp( node1.getName(), "DrawOverlays" ) == 0 )
		{
			if( node1.getAttribute( "context" ) == 0x0 ) return "Missing DrawOverlays attribute 'context'";
			stage.commands.push_back( PipelineCommand( PipelineCommands::DrawOverlays ) );
			(stage.commands.end() - 1)->valParams.push_back( new PCStringParam( node1.getAttribute( "context" ) ) );
		}
		else if( strcmp( node1.getName(), "DrawQuad" ) == 0 )
		{
			if( node1.getAttribute( "material" ) == 0x0 ) return "Missing DrawQuad attribute 'material'";
			if( node1.getAttribute( "context" ) == 0x0 ) return "Missing DrawQuad attribute 'context'";
			stage.commands.push_back( PipelineCommand( PipelineCommands::DrawQuad ) );
			uint32 matRes = Modules::resMan().addResource(
				ResourceTypes::Material, node1.getAttribute( "material" ), 0, false );
			(stage.commands.end() - 1)->resParams.push_back( Modules::resMan().resolveResHandle( matRes ) );
			(stage.commands.end() - 1)->valParams.push_back( new PCStringParam( node1.getAttribute( "context" ) ) );
		}
		else if( strcmp( node1.getName(), "DoForwardLightLoop" ) == 0 )
		{
		    stage.commands.push_back( PipelineCommand( PipelineCommands::DoForwardLightLoop ) );
		    (stage.commands.end() - 1)->valParams.push_back( new PCStringParam( node1.getAttribute( "context", "" ) ) );
			(stage.commands.end() - 1)->valParams.push_back( new PCStringParam( node1.getAttribute( "class", "" ) ) );
			(stage.commands.end() - 1)->valParams.push_back( new PCBoolParam(
				_stricmp( node1.getAttribute( "noShadows", "false" ), "true" ) == 0 ) );

			string orderString = node1.getAttribute( "order", "" );
			int order = RenderingOrder::None;
			if( orderString == "FRONT_TO_BACK" ) order = RenderingOrder::FrontToBack;
			else if( orderString == "BACK_TO_FRONT" ) order = RenderingOrder::BackToFront;
			else if( orderString == "STATECHANGES" ) order = RenderingOrder::StateChanges;
			(stage.commands.end() - 1)->valParams.push_back( new PCIntParam( order ) );
		}
		else if( strcmp( node1.getName(), "DoDeferredLightLoop" ) == 0 )
		{
			stage.commands.push_back( PipelineCommand( PipelineCommands::DoDeferredLightLoop ) );
			(stage.commands.end() - 1)->valParams.push_back( new PCStringParam( node1.getAttribute( "context", "" ) ) );
			(stage.commands.end() - 1)->valParams.push_back( new PCBoolParam(
				_stricmp( node1.getAttribute( "noShadows", "false" ), "true" ) == 0 ) );
		}
		else if( strcmp( node1.getName(), "SetUniform" ) == 0 )
		{
			if( node1.getAttribute( "material" ) == 0x0 ) return "Missing SetUniform attribute 'material'";
			if( node1.getAttribute( "uniform" ) == 0x0 ) return "Missing SetUniform attribute 'uniform'";
			stage.commands.push_back( PipelineCommand( PipelineCommands::SetUniform ) );
			uint32 matRes = Modules::resMan().addResource(
				ResourceTypes::Material, node1.getAttribute( "material" ), 0, false );
			(stage.commands.end() - 1)->resParams.push_back( Modules::resMan().resolveResHandle( matRes ) );

			vector< PCParam * > &valParams = (stage.commands.end() - 1)->valParams;
			valParams.resize( 5 );
			valParams[0] = new PCStringParam( node1.getAttribute( "uniform" ) );
			valParams[1] = new PCFloatParam( (float)atof( node1.getAttribute( "a", "0" ) ) );
			valParams[2] = new PCFloatParam( (float)atof( node1.getAttribute( "b", "0" ) ) );
			valParams[3] = new PCFloatParam( (float)atof( node1.getAttribute( "c", "0" ) ) );
			valParams[4] = new PCFloatParam( (float)atof( node1.getAttribute( "d", "0" ) ) );
		}

		node1 = node.getChildNode( ++nodeItr1 );
	}

	return "";
}


void PipelineResource::addRenderTarget( const string &id, bool depthBuf, uint32 numColBufs,
										RenderBufferFormats::List format, bool bilinear, uint32 samples,
										uint32 width, uint32 height, float scale )
{
	if( numColBufs > RenderBuffer::MaxColorAttachmentCount ) return;
	
	RenderTarget rt;
	
	rt.id = id;
	rt.hasDepthBuf = depthBuf;
	rt.numColBufs = numColBufs;
	rt.format = format;
	rt.bilinear = bilinear;
	rt.samples = samples;
	rt.width = width;
	rt.height = height;
	rt.scale = scale;

	_renderTargets.push_back( rt );
}


RenderTarget *PipelineResource::findRenderTarget( const string &id )
{
	if( id == "" ) return 0x0;
	
	for( uint32 i = 0; i < _renderTargets.size(); ++i )
	{
		if( _renderTargets[i].id == id )
		{
			return &_renderTargets[i];
		}
	}
	
	return 0x0;
}


bool PipelineResource::createRenderTargets()
{
	for( uint32 i = 0; i < _renderTargets.size(); ++i )
	{
		RenderTarget &rt = _renderTargets[i];
	
		uint32 width = (int)(rt.width * rt.scale), height = (int)(rt.height * rt.scale);
		if( width == 0 ) width = (int)(Modules::renderer().getVPWidth() * rt.scale);
		if( height == 0 ) height = (int)(Modules::renderer().getVPHeight() * rt.scale);
		
		rt.rendBuf = Modules::renderer().createRenderBuffer(
			width, height, rt.format, rt.hasDepthBuf, rt.numColBufs, rt.bilinear, 0 );
		if( rt.rendBuf.fbo == 0 ) return false;

		if( rt.samples > 0 )
		{
			// Also create a multisampled renderbuffer
			rt.rendBufMultisample = Modules::renderer().createRenderBuffer(
				width, height, rt.format, rt.hasDepthBuf, rt.numColBufs, rt.bilinear, rt.samples );
			if( rt.rendBufMultisample.fbo == 0 ) return false;
		}
	}
	
	return true;
}


void PipelineResource::destroyRenderTargets()
{
	for( uint32 i = 0; i < _renderTargets.size(); ++i )
	{
		RenderTarget &rt = _renderTargets[i];
		
		Modules::renderer().destroyRenderBuffer( rt.rendBuf );
	}
}


bool PipelineResource::load( const char *data, int size )
{
	if( !Resource::load( data, size ) ) return false;
	if( data[size - 1] != '\0' )
	{	
		return raiseError( "Data block not NULL-terminated" );
	}

	XMLResults res;
	XMLNode rootNode = XMLNode::parseString( data, "Pipeline", &res );
	if( res.error != eXMLErrorNone )
	{
		return raiseError( XMLNode::getError( res.error ), res.nLine );
	}

	// Parse setup
	XMLNode node1 = rootNode.getChildNode( "Setup" );
	if( !node1.isEmpty() )
	{
		int nodeItr2 = 0;
		XMLNode node2 = node1.getChildNode( "RenderTarget", nodeItr2 );
		while( !node2.isEmpty() )
		{
			if( node2.getAttribute( "id" ) == 0x0 ) return raiseError( "Missing RenderTarget attribute 'id'" );
			string id = node2.getAttribute( "id" );
			
			if( node2.getAttribute( "depthBuf" ) == 0x0 ) return raiseError( "Missing RenderTarget attribute 'depthBuf'" );
			bool depth = false;
			if( _stricmp( node2.getAttribute( "depthBuf" ), "true" ) == 0 ) depth = true;
			
			if( node2.getAttribute( "numColBufs" ) == 0x0 ) return raiseError( "Missing RenderTarget attribute 'numColBufs'" );
			uint32 numBuffers = atoi( node2.getAttribute( "numColBufs" ) );
			
			RenderBufferFormats::List format = RenderBufferFormats::RGBA8;
			if( node2.getAttribute( "format" ) != 0x0 )
			{
				if( _stricmp( node2.getAttribute( "format" ), "RGBA8" ) == 0 )
					format = RenderBufferFormats::RGBA8;
				else if( _stricmp( node2.getAttribute( "format" ), "RGBA16F" ) == 0 )
					format = RenderBufferFormats::RGBA16F;
				else if( _stricmp( node2.getAttribute( "format" ), "RGBA32F" ) == 0 )
					format = RenderBufferFormats::RGBA32F;
				else return raiseError( "Unknown RenderTarget format" );
			}

			bool bilinear = false;
			if( _stricmp( node2.getAttribute( "bilinear", "false" ), "true" ) == 0 ||
				_stricmp( node2.getAttribute( "bilinear", "0" ), "1" ) == 0 )
				bilinear = true;

			int maxSamples = atoi( node2.getAttribute( "maxSamples", "0" ) );

			uint32 width = atoi( node2.getAttribute( "width", "0" ) );
			uint32 height = atoi( node2.getAttribute( "height", "0" ) );
			float scale = (float)atof( node2.getAttribute( "scale", "1" ) );

			addRenderTarget( id, depth, numBuffers, format, bilinear,
				min( maxSamples, Modules::config().sampleCount ), width, height, scale );

			node2 = node1.getChildNode( "RenderTarget", ++nodeItr2 );
		}
	}

	// Parse commands
	node1 = rootNode.getChildNode( "CommandQueue" );
	if( !node1.isEmpty() )
	{
		int nodeItr2 = 0;
		XMLNode node2 = node1.getChildNode( "Stage", nodeItr2 );
		while( !node2.isEmpty() )
		{
			_stages.push_back( PipelineStage() );
			string errorMsg = parseStage( node2, _stages[_stages.size() - 1] );
			if( errorMsg != "" ) 
				return raiseError( "Error in stage '" + _stages[_stages.size() - 1].id + "': " + errorMsg );
			
			node2 = node1.getChildNode( "Stage", ++nodeItr2 );
		}
	}

	// Create render targets
	if( !createRenderTargets() )
	{
		return raiseError( "Failed to create render target" );
	}

	return true;
}


void PipelineResource::resize()
{
	// Recreate render targets
	destroyRenderTargets();
	createRenderTargets();
}


bool PipelineResource::setStageActivation( const string &stageName, bool enabled )
{
	for( uint32 i = 0; i < _stages.size(); ++i )
	{
		if( _stages[i].id == stageName )
		{
			_stages[i].enabled = enabled;
			return true;
		}
	}
	
	return false;
}


bool PipelineResource::getRenderTargetData( const string &target, int bufIndex, int *width, int *height,
											int *compCount, float *dataBuffer, int bufferSize )
{
	RenderBuffer *rb = 0x0;
	if( target != "" )
	{	
		RenderTarget *rt = findRenderTarget( target );
		if( rt == 0x0 ) return false;
		else rb = &rt->rendBuf;
	}
	
	return Modules::renderer().getBufferData(
		rb, bufIndex, width, height, compCount, dataBuffer, bufferSize );
}
