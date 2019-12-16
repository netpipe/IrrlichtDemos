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

#include "egResource.h"
#include "egModules.h"
#include "egGeometry.h"
#include "egSceneGraphRes.h"
#include "egTextures.h"
#include "egMaterial.h"
#include "egAnimation.h"
#include "egShader.h"
#include <sstream>

#include "utDebug.h"


// **********************************************************************************
// Class Resource
// **********************************************************************************

Resource::Resource( ResourceTypes::List type, const string &name, int flags )
{
	_type = type;
	_name = name;
	_handle = 0;
	_loaded = false;
	_refCount = 0;
	_userRefCount = 0;
	_flags = flags;
	
	if( (flags & ResourceFlags::NoQuery) == ResourceFlags::NoQuery ) _noQuery = true;
	else _noQuery = false;
}


Resource::~Resource()
{
	// Remove all references
	// Nothing to do here
}


Resource *Resource::clone()
{
	Modules::log().writeDebugInfo( "Resource cloning not implemented for type %i", _type );
	return 0x0;
}


void Resource::initDefault()
{
}


void Resource::release()
{
}


bool Resource::load( const char *data, int size )
{	
	// Resources can only be loaded once
	if( _loaded ) return false;
	
	// A NULL pointer can be used if the file could not be loaded
	if( data == 0x0 || size <= 0 )
	{	
		Modules::log().writeWarning( "Resource '%s' of type %i: No data loaded (file not found?)", _name.c_str(), _type );
		_noQuery = true;
		return false;
	}

	_loaded = true;
	
	return true;
}


void Resource::unload()
{
	release();
	initDefault();
	_loaded = false;
}


int Resource::getParami( int /*param*/ )
{
	return 0;
}


bool Resource::setParami( int /*param*/, int /*value*/ )
{
	return false;
}


float Resource::getParamf( int /*param*/ )
{
	return 0.0f;
}


bool Resource::setParamf( int /*param*/, float /*value*/ )
{
	return false;
}


const char *Resource::getParamstr( int param )
{
	return "";
}


bool Resource::setParamstr( int param, const char *value )
{
	return false;
}


const void *Resource::getData( int /*param*/ )
{
	return 0x0;
}


bool Resource::updateData( int /*param*/, const void * /*data*/, int /*size*/ )
{
	return false;
}


// **********************************************************************************
// Class ResourceManager
// **********************************************************************************

ResourceManager::ResourceManager()
{
	_resources.reserve( 100 );
}


ResourceManager::~ResourceManager()
{
	clear();

	// Release resource types
	map< int, ResourceRegEntry >::const_iterator itr = _registry.begin();
	while( itr != _registry.end() )
	{
		if( itr->second.initializationFunc != 0x0 )
			(*itr->second.initializationFunc)();

		++itr;
	}
}


void ResourceManager::registerType( int type, const string &typeString, ResTypeInitializationFunc inf,
									ResTypeReleaseFunc rf, ResTypeFactoryFunc ff )
{
	ResourceRegEntry entry;
	entry.typeString = typeString;
	entry.initializationFunc = inf;
	entry.releaseFunc = rf;
	entry.factoryFunc = ff;
	_registry[type] = entry;

	// Initialize resource type
	if( inf != 0 ) (*inf)();
}


Resource *ResourceManager::findResource( ResourceTypes::List type, const string &name )
{
	for( uint32 i = 0; i < _resources.size(); ++i )
	{
		if( _resources[i] != 0x0 && _resources[i]->_type == type && _resources[i]->_name == name )
		{
			return _resources[i];
		}
	}
	
	return 0x0;
}


ResHandle ResourceManager::addResource( Resource &resource )
{
	// Try to insert resource in free slot
	for( uint32 i = 0; i < _resources.size(); ++i )
	{
		if( _resources[i] == 0x0 )
		{
			resource._handle = i + 1;
			_resources[i] = &resource;
			return i + 1;
		}
	}
	
	// If there is no free slot, add resource to end
	resource._handle = (ResHandle)_resources.size() + 1;
	_resources.push_back( &resource );
	return resource._handle;
}


ResHandle ResourceManager::addResource( ResourceTypes::List type, const string &name,
										int flags, bool userCall )
{
	if( name == "" || name.find( ":" ) != string::npos )
	{	
		Modules::log().writeDebugInfo( "Invalid name for added resource of type %i", type );
		return 0;
	}
	
	// Check if resource is already in list and return index
	for( uint32 i = 0; i < _resources.size(); ++i )
	{
		if( _resources[i] != 0x0 && _resources[i]->_name == name )
		{
			if( _resources[i]->_type == type )
			{
				if( userCall ) ++_resources[i]->_userRefCount;
				return i + 1;
			}
		}
	}
	
	// Create resource
	Resource *resource = 0x0;
	map< int, ResourceRegEntry >::iterator itr = _registry.find( type );
	if( itr != _registry.end() ) resource = (*itr->second.factoryFunc)( name, flags );
	if( resource == 0x0 ) return 0;

	Modules::log().writeInfo( "Adding %s resource '%s'", itr->second.typeString.c_str(), name.c_str() );
	
	if( userCall ) resource->_userRefCount = 1;
	
	return addResource( *resource );
}


ResHandle ResourceManager::addNonExistingResource( Resource &resource, bool userCall )
{
	if( resource._name == "" || resource._name.find( ":" ) != string::npos ) return 0;

	// Check that name does not yet exist
	for( uint32 i = 0; i < _resources.size(); ++i )
	{
		if( _resources[i] != 0x0 && _resources[i]->_name == resource._name ) return 0;
	}

	if( userCall ) resource._userRefCount += 1;
	return addResource( resource );
}


ResHandle ResourceManager::cloneResource( ResHandle sourceRes, const string &name )
{
	Resource *res = resolveResHandle( sourceRes );
	if( res == 0x0 ) return 0;
	
	// Check that name does not yet exist
	if( name != "" )
	{
		for( uint32 i = 0; i < _resources.size(); ++i )
		{
			if( _resources[i] != 0x0 && _resources[i]->_name == name )
			{	
				Modules::log().writeDebugInfo( "Name '%s' used for cloneResource already exists", name.c_str() );
				return 0;
			}
		}
	}

	Resource *newRes = res->clone();
	if( newRes == 0x0 ) return 0;

	newRes->_name = name != "" ? name : "|tmp|";
	newRes->_userRefCount = 1;
	int handle = addResource( *newRes );
	
	if( name == "" )
	{
		stringstream ss;
		ss << res->_name << "|" << handle;
		newRes->_name = ss.str();
	}

	return handle;
}


int ResourceManager::removeResource( ResHandle handle, bool userCall )
{
	Resource *res = resolveResHandle( handle );

	if( res == 0x0 )
	{	
		Modules::log().writeDebugInfo( "Invalid resource handle %i in removeResource", handle );
		return -1;
	}

	// Decrease reference count
	if( userCall && res->_userRefCount > 0 ) --res->_userRefCount;

	return (signed)res->_userRefCount;
}


void ResourceManager::clear()
{
	// Release resources and remove dependencies
	for( uint32 i = 0; i < _resources.size(); ++i )
	{
		if( _resources[i] != 0x0 ) _resources[i]->release();
	}

	// Delete resources
	for( uint32 i = 0; i < _resources.size(); ++i )
	{
		if( _resources[i] != 0x0 )
		{
			delete _resources[i]; _resources[i] = 0x0;
		}
	}
}


ResHandle ResourceManager::queryUnloadedResource( int index )
{
	int j = 0;

	for( uint32 i = 0; i < _resources.size(); ++i )
	{
		if( _resources[i] != 0x0 && !_resources[i]->_loaded && !_resources[i]->_noQuery )
		{	
			if( j == index ) return _resources[i]->_handle;
			else ++j;
		}
	}

	return 0;
}


void ResourceManager::releaseUnusedResources()
{
	vector< uint32 > killList;
	
	// Find unesed resources and release dependencies
	for( uint32 i = 0; i < _resources.size(); ++i )
	{
		if( _resources[i] != 0x0 && _resources[i]->_userRefCount == 0 && _resources[i]->_refCount == 0 )
		{
			killList.push_back( i );
			_resources[i]->release();
		}
	}
	
	// Delete unused resources
	for( uint32 i = 0; i < killList.size(); ++i )
	{
		Modules::log().writeInfo( "Removed resource '%s'", _resources[killList[i]]->_name.c_str() );
		delete _resources[killList[i]]; _resources[killList[i]] = 0x0;
	}

	// Releasing a resource can remove dependencies from other resources which can also be released
	if( !killList.empty() ) releaseUnusedResources();
}
