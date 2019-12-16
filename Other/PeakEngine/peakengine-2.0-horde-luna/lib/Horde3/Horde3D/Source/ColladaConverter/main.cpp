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

#include "daeMain.h"
#include "converter.h"
#include "utPlatform.h"

#ifdef PLATFORM_WIN
#   define WIN32_LEAN_AND_MEAN 1
#	include <windows.h>
#endif


int main( int argc, char **argv )
{
	log( "Horde3D Collada Converter" );
	log( "Version 1.0.0 Beta1" );
	log( "" );
	
	if( argc < 2 )
	{
		log( "Usage:" );
		log( "ColladaConv inputFile [-o outputName] [-s shaderName] [-noopt]" );
		log( "" );
		log( "inputFile:      filename of the COLLADA document" );
		log( "-o outputName:  name of the output files (without extension)" );
		log( "-s shaderName:  filename of the default shader for materials" );
		log( "-noopt:         disable geometry optimization" );
		log( "-anim:          export animations only" );
		return 0;
	}
	
	string inName = argv[1];
	string outName = extractFileName( inName, false );
	string defShader = "skinning.shader.xml";
	bool optimize = true, animsOnly = false;

	for( int i = 2; i < argc; ++i )
	{
		if( strcmp( argv[i], "-o" ) == 0 )
		{
			if( argc > i + 1 )
			{	
				outName = argv[++i];
			}
			else
			{
				log( "Invalid argument" );
				return 0;
			}
		}
		else if( strcmp( argv[i], "-s" ) == 0 )
		{
			if( argc > i + 1 )
			{
				defShader = argv[++i];
			}
			else
			{
				log( "Invalid argument" );
				return 0;
			}
		}
		else if( strcmp( argv[i], "-noopt" ) == 0 )
		{
			optimize = false;
		}
		else if( strcmp( argv[i], "-anim" ) == 0 )
		{
			animsOnly = true;
		}
		else
		{
			log( "Invalid argument" );
			return 0;
		}
	}

	ColladaDocument *colladaFile = new ColladaDocument();
	
	log( "Parsing collada file " + inName + "..." );
	if( !colladaFile->parseFile( inName ) )
	{
		return 0;
	}
	else
	{
		log( "Done." );
	}

	// Set output directory (needed when using drag&drop of input file on application)
	#ifdef PLATFORM_WIN
	SetCurrentDirectory( extractFilePath( inName ).c_str() );
	#endif
	
	// Convert
	log( "Converting data for model " + outName + "..." );
	if( !optimize ) log( "Geometry optimization disabled" );
	Converter *converter = new Converter();
	converter->convertModel( *colladaFile, optimize );
	log( "Done." );
	
	if( !animsOnly )
	{
		log( "Writing geometry..." );
		converter->saveModel( outName );
		log( "Done." );
		
		log( "Writing materials..." );
		converter->writeMaterials( *colladaFile, outName, defShader );
		log( "Done." );
	}

	if( converter->hasAnimation() )
	{
		log( "Writing animation..." );
		converter->writeAnimation( outName );
		log( "Done." );
	}

	log( "" );
	log( "Finished conversion." );
	
	delete colladaFile; colladaFile = 0x0;
	delete converter; converter = 0x0;
	
	return 1;
}
