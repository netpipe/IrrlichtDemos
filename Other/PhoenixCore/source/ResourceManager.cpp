/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#include "ResourceManager.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

using namespace phoenix;

/*!---------------------
Clear
-----------------------*/

void phoenix::ResourceManager::clear( bool drop  )
{
	boost::recursive_mutex::scoped_lock l( getMutex()  );
	if( drop ){
		for( std::list< boost::intrusive_ptr<phoenix::Resource> >::iterator i = resourcelist.begin(); i != resourcelist.end(); ++i )
		{
			if( *i ) (*i)->drop();
		}
	}
	recyclelist.clear();
    resourcelist.clear();
}

/*!---------------------
Find resource
-----------------------*/
boost::intrusive_ptr<phoenix::Resource> phoenix::ResourceManager::find( const std::string& name )
{
	boost::recursive_mutex::scoped_lock( *getMutex() );
    for( std::list< boost::intrusive_ptr<phoenix::Resource> >::iterator i = resourcelist.begin(); i != resourcelist.end(); ++i )
    {
        if( (*i)->getName() == name )
        {
            return (*i)->grab<phoenix::Resource>();
        }
    }
    return boost::intrusive_ptr<phoenix::Resource>();
}

//Garbage collector
void phoenix::ResourceManager::clean()
{
	
	boost::recursive_mutex::scoped_lock l( getMutex() );

    unsigned int multiplier = recyclelist.size()/getCollectionRate();
    if( multiplier < getCollectionRate() ) multiplier = getCollectionRate();
	
	for( unsigned int i = 0; i < multiplier; ++i )
	{
		if( ! recyclelist.empty() )
		{
			boost::intrusive_ptr<phoenix::Resource>& g = recyclelist.back();
			if( g )
				resourcelist.remove( g );
			recyclelist.pop_back();
		}
		else
		{
			break;
		}
	}

}