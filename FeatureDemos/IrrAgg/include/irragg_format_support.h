// Copyright 2019 Nicolaus Anderson
// See license.txt for details.
/*
	This file contains functions identifying file format and indicating
	if such a format is supported by this library.
*/

#ifndef __IRR_AGG_FORMAT_SUPPORT_H__
#define __IRR_AGG_FORMAT_SUPPORT_H__

#include <IImage.h>

namespace irr {
namespace vecg {


	//! Check IrrAgg library support
	inline bool isImageSupported( irr::video::IImage*  image )
	{
		switch( image->getColorFormat() )
		{
		// Supported
		case irr::video::ECF_A8R8G8B8:
			return true;

		// Unsupported
		default:
			return false;
		}
	}


	//! Check if the Irrlicht image is a 32-bit ARGB color
	inline bool isImageARGB32( irr::video::IImage*  image )
	{
		if ( !image )
			return false;

		return image->getColorFormat() == irr::video::ECF_A8R8G8B8;
	}


} // end sub namespace vecg
} // end namespace irr

#endif // __IRR_AGG_FORMAT_SUPPORT_H__
