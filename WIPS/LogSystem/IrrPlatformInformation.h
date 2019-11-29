/*!
	@file		IrrPlatformInformation.h
	@author		manguonden
	@date		11/2009
	@module
*//*
	This file is part of Irrlicht Engine.
	
	Irrlicht Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	Irrlicht Engine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with Irrlicht Engine.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifndef __PlatformInformation_H__
#define __PlatformInformation_H__

#include "IrrStringConverter.h"

namespace irr 
{
//
// TODO: Puts following macros into OgrePlatform.h?
//
/* Initial CPU stuff to set.
*/
#define IRR_CPU_UNKNOWN    0
#define IRR_CPU_X86        1
#define IRR_CPU_PPC        2

    /** Class which provides the run-time platform information Ogre runs on.
        @remarks
            Ogre is designed to be platform-independent, but some platform
            and run-time environment specific optimised functions are built-in
            to maximise performance, and those special optimised routines are
            need to determine run-time environment for select variant executing
            path.
        @par
            This class manages that provides a couple of functions to determine
            platform information of the run-time environment.
        @note
            This class is supposed to use by advanced user only.
    */

    class PlatformInformation
    {
    public:
        /// Enum describing the different CPU features we want to check for, platform-dependent
        enum CpuFeatures
        {
#if defined(IRR_CPU_X86)
            CPU_FEATURE_SSE         = 1 << 0,
            CPU_FEATURE_SSE2        = 1 << 1,
            CPU_FEATURE_SSE3        = 1 << 2,
            CPU_FEATURE_MMX         = 1 << 3,
            CPU_FEATURE_MMXEXT      = 1 << 4,
            CPU_FEATURE_3DNOW       = 1 << 5,
            CPU_FEATURE_3DNOWEXT    = 1 << 6,
            CPU_FEATURE_CMOV        = 1 << 7,
            CPU_FEATURE_TSC         = 1 << 8,
            CPU_FEATURE_FPU         = 1 << 9,
            CPU_FEATURE_PRO         = 1 << 10,
            CPU_FEATURE_HTT         = 1 << 11,
#endif

            CPU_FEATURE_NONE        = 0
        };

        /** Gets a string of the CPU identifier.
        @note
            Actual detecting are performs in the first time call to this function,
            and then all future calls with return internal cached value.
        */
        static const String& getCpuIdentifier(void);

        /** Gets a or-masked of enum CpuFeatures that are supported by the CPU.
        @note
            Actual detecting are performs in the first time call to this function,
            and then all future calls with return internal cached value.
        */
        static unsigned int getCpuFeatures(void);

		/** Gets whether a specific feature is supported by the CPU.
		@note
			Actual detecting are performs in the first time call to this function,
			and then all future calls with return internal cached value.
		*/
		static bool hasCpuFeature(CpuFeatures feature);

		/** Write the CPU information to the passed in Log */
		static void logPlatformInformation( void );

    };

} // End of  irr namespace section

#endif  // __PlatformInformation_H__
