/*!
	@file		IrrPlatformInformation.cpp
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

//#define _HAS_EXCEPTIONS 0 //This is a test. I need fix this problem

#include "IrrCompileConfig.h"
#include "IrrPlatformInformation.h"
#include "IrrLogManager.h"
#include <assert.h>
//#include "IrrUTFString.h"


#if defined(_IRR_WINDOWS_API_)
#include <excpt.h>      // For SEH values
	#if _MSC_VER >= 1400
		#include <intrin.h>
	#endif
#elif defined(__GNUC__)
#include <signal.h>
#include <setjmp.h>

#endif

#if defined(_MSC_VER) && (1300 <= _MSC_VER && _MSC_VER <= 1310)

# if defined(_DLL_CPPLIB)

namespace std
{
    template class _CRTIMP2 basic_string<unsigned short, char_traits<unsigned short>,
	    allocator<unsigned short> >;

    template class _CRTIMP2 basic_string<__wchar_t, char_traits<__wchar_t>,
	    allocator<__wchar_t> >;
}

# endif // defined(_DLL_CPPLIB)

#endif  // _MSC_VER && _MSC_VER == 1300

// Yes, I known, this file looks very ugly, but there hasn't other ways to do it better.

namespace irr {

		/*using namespace io;
		using namespace video;

		struct PlatformInformationStruct
		{
			s32 BitsPerPixel; // [Setup] (s32) Bits per pipxel (fullscreen mode only)
			s32 Width, Height; // [Setup] (s32) Width & Height of the window
			video::E_DRIVER_TYPE DriverType; // [Setup] (video::E_DRIVER_TYPE) Driver Type
			bool Fullscreen; // [Setup] (bool) Fullscreen mode
			bool AntiAlias; // [Setup] (bool) Fullscreen antialiasing
			bool Vsync; // [Setup] (bool) Video synchronization (fullscreen mode only)
			int PerPixelLighting; // [Setup] (int) 0=do not use; 1=use bump mapping; 2=use parallax mapping
			IOSOperator* OSInfo; // (IOSOperator*) IrrLicht operation system
			
			video::IVideoDriver* Video; // (video::IVideoDriver*) IrrLicht video driver
		};

		PlatformInformationStruct OSPlatformInformation;*/

#if defined(IRR_CPU_X86)

    //---------------------------------------------------------------------
    // Struct for store CPUID instruction result, compiler-independent
    //---------------------------------------------------------------------
    struct CpuidResult
    {
        // Note: DO NOT CHANGE THE ORDER, some code based on that.
        unsigned int _eax;
        unsigned int _ebx;
        unsigned int _edx;
        unsigned int _ecx;
    };

//---------------------------------------------------------------------
// Compiler-dependent routines
//---------------------------------------------------------------------

/*#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4800)  // no return value
#endif*/

    //---------------------------------------------------------------------
    // Detect whether CPU supports CPUID instruction, returns non-zero if supported.
    static int _isSupportCpuid(void)
    {
#if defined(_MSC_VER)
		// Visual Studio 2005 & 64-bit compilers always supports __cpuid intrinsic
		// note that even though this is a build rather than runtime setting, all
		// 64-bit CPUs support this so since binary is 64-bit only we're ok
	#if _MSC_VER >= 1400 && defined(_WIN64)
		return true;
	#else
		// If we can modify flag register bit 21, the cpu is supports CPUID instruction
        __asm
        {
            // Read EFLAG
            pushfd
            pop     eax
            mov     ecx, eax

            // Modify bit 21
            xor     eax, 0x200000
            push    eax
            popfd

            // Read back EFLAG
            pushfd
            pop     eax

            // Restore EFLAG
            push    ecx
            popfd

            // Check bit 21 modifiable
            xor     eax, ecx
            neg     eax
            sbb     eax, eax

            // Return values in eax, no return statment requirement here for VC.
        }
	#endif
#elif defined(__GNUC__)
        #if defined(_WIN64)
           return true;
       #else

        unsigned oldFlags, newFlags;
        __asm__
        (
            "pushfl         \n\t"
            "pop    %0      \n\t"
            "mov    %0, %1  \n\t"
            "xor    %2, %0  \n\t"
            "push   %0      \n\t"
            "popfl          \n\t"
            "pushfl         \n\t"
            "pop    %0      \n\t"
            "push   %1      \n\t"
            "popfl          \n\t"
            : "=r" (oldFlags), "=r" (newFlags)
            : "n" (0x200000)
        );
        return oldFlags != newFlags;
       #endif // 64
#else
        // TODO: Supports other compiler
        return false;
#endif
    }

    //---------------------------------------------------------------------
    // Performs CPUID instruction with 'query', fill the results, and return value of eax.
    static unsigned int _performCpuid(int query, CpuidResult& result)
    {
#if defined(_MSC_VER)
	#if _MSC_VER >= 1400 
		int CPUInfo[4];
		__cpuid(CPUInfo, query);
		result._eax = CPUInfo[0];
		result._ebx = CPUInfo[1];
		result._ecx = CPUInfo[2];
		result._edx = CPUInfo[3];
		return result._eax;
	#else
        __asm
        {
            mov     edi, result
            mov     eax, query
            cpuid
            mov     [edi]._eax, eax
            mov     [edi]._ebx, ebx
            mov     [edi]._edx, edx
            mov     [edi]._ecx, ecx
            // Return values in eax, no return statment requirement here for VC.
        }
	#endif
#elif defined(__GNUC__)
        #if defined(_WIN64)
        __asm__
        (
            "cpuid": "=a" (result._eax), "=b" (result._ebx), "=c" (result._ecx), "=d" (result._edx) : "a" (query)
        );
        #else
        __asm__
        (
            "pushl  %%ebx           \n\t"
            "cpuid                  \n\t"
            "movl   %%ebx, %%edi    \n\t"
            "popl   %%ebx           \n\t"
            : "=a" (result._eax), "=D" (result._ebx), "=c" (result._ecx), "=d" (result._edx)
            : "a" (query)
        );
       #endif // _WIN64
        return result._eax;

#else
        // TODO: Supports other compiler
        return 0;
#endif
    }

/*#if defined(_MSC_VER)
#pragma warning(pop)
#endif*/

    //---------------------------------------------------------------------
    // Detect whether or not os support Streaming SIMD Extension.
#if defined(__GNUC__)
    static jmp_buf sIllegalJmpBuf;
    static void _illegalHandler(int x)
    {
        (void)(x); // Unused
        longjmp(sIllegalJmpBuf, 1);
    }
#endif
    static bool _checkOperatingSystemSupportSSE(void)
    {
#if defined(_MSC_VER)
        /*
            The FP part of SSE introduces a new architectural state and therefore
            requires support from the operating system. So even if CPUID indicates
            support for SSE FP, the application might not be able to use it. If
            CPUID indicates support for SSE FP, check here whether it is also
            supported by the OS, and turn off the SSE FP feature bit if there
            is no OS support for SSE FP.

            Operating systems that do not support SSE FP return an illegal
            instruction exception if execution of an SSE FP instruction is performed.
            Here, a sample SSE FP instruction is executed, and is checked for an
            exception using the (non-standard) __try/__except mechanism
            of Microsoft Visual C/C++.
        */
		// Visual Studio 2005, Both AMD and Intel x64 support SSE
		// note that even though this is a build rather than runtime setting, all
		// 64-bit CPUs support this so since binary is 64-bit only we're ok
	#if _MSC_VER >= 1400 && defined(_WIN64)
			return true;
	#else
        __try
        {
            __asm orps  xmm0, xmm0
            return true;
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return false;
        }
	#endif
#elif defined(__GNUC__)
        #if defined(_WIN64)
            return true;
        #else
        // Does gcc have __try/__except similar mechanism?
        // Use signal, setjmp/longjmp instead.
        void (*oldHandler)(int);
        oldHandler = signal(SIGILL, _illegalHandler);

        if (setjmp(sIllegalJmpBuf))
        {
            signal(SIGILL, oldHandler);
            return false;
        }
        else
        {
            __asm__ __volatile__ ("orps %xmm0, %xmm0");
            signal(SIGILL, oldHandler);
            return true;
        }
       #endif
#else
        // TODO: Supports other compiler, assumed is supported by default
        return true;
#endif
    }

    //---------------------------------------------------------------------
    // Compiler-independent routines
    //---------------------------------------------------------------------

    static unsigned int queryCpuFeatures(void)
    {
#define CPUID_STD_FPU               (1<<0)
#define CPUID_STD_TSC               (1<<4)
#define CPUID_STD_CMOV              (1<<15)
#define CPUID_STD_MMX               (1<<23)
#define CPUID_STD_SSE               (1<<25)
#define CPUID_STD_SSE2              (1<<26)
#define CPUID_STD_HTT               (1<<28)     // EDX[28] - Bit 28 set indicates  Hyper-Threading Technology is supported in hardware.

#define CPUID_STD_SSE3              (1<<0)      // ECX[0] - Bit 0 of standard function 1 indicate SSE3 supported

#define CPUID_FAMILY_ID_MASK        0x0F00      // EAX[11:8] - Bit 11 thru 8 contains family  processor id
#define CPUID_EXT_FAMILY_ID_MASK    0x0F00000   // EAX[23:20] - Bit 23 thru 20 contains extended family processor id
#define CPUID_PENTIUM4_ID           0x0F00      // Pentium 4 family processor id

#define CPUID_EXT_3DNOW             (1<<31)
#define CPUID_EXT_AMD_3DNOWEXT      (1<<30)
#define CPUID_EXT_AMD_MMXEXT        (1<<22)

        unsigned int features = 0;

        // Supports CPUID instruction ?
        if (_isSupportCpuid())
        {
            CpuidResult result;

            // Has standard feature ?
            if (_performCpuid(0, result))
            {
                // Check vendor strings
                if (memcmp(&result._ebx, "GenuineIntel", 12) == 0)
                {
                    if (result._eax > 2)
                        features |= PlatformInformation::CPU_FEATURE_PRO;

                    // Check standard feature
                    _performCpuid(1, result);

                    if (result._edx & CPUID_STD_FPU)
                        features |= PlatformInformation::CPU_FEATURE_FPU;
                    if (result._edx & CPUID_STD_TSC)
                        features |= PlatformInformation::CPU_FEATURE_TSC;
                    if (result._edx & CPUID_STD_CMOV)
                        features |= PlatformInformation::CPU_FEATURE_CMOV;
                    if (result._edx & CPUID_STD_MMX)
                        features |= PlatformInformation::CPU_FEATURE_MMX;
                    if (result._edx & CPUID_STD_SSE)
                        features |= PlatformInformation::CPU_FEATURE_MMXEXT | PlatformInformation::CPU_FEATURE_SSE;
                    if (result._edx & CPUID_STD_SSE2)
                        features |= PlatformInformation::CPU_FEATURE_SSE2;

                    if (result._ecx & CPUID_STD_SSE3)
                        features |= PlatformInformation::CPU_FEATURE_SSE3;

                    // Check to see if this is a Pentium 4 or later processor
                    if ((result._eax & CPUID_EXT_FAMILY_ID_MASK) ||
                        (result._eax & CPUID_FAMILY_ID_MASK) == CPUID_PENTIUM4_ID)
                    {
                        // Check hyper-threading technology
                        if (result._edx & CPUID_STD_HTT)
                            features |= PlatformInformation::CPU_FEATURE_HTT;
                    }
                }
                else if (memcmp(&result._ebx, "AuthenticAMD", 12) == 0)
                {
                    features |= PlatformInformation::CPU_FEATURE_PRO;

                    // Check standard feature
                    _performCpuid(1, result);

                    if (result._edx & CPUID_STD_FPU)
                        features |= PlatformInformation::CPU_FEATURE_FPU;
                    if (result._edx & CPUID_STD_TSC)
                        features |= PlatformInformation::CPU_FEATURE_TSC;
                    if (result._edx & CPUID_STD_CMOV)
                        features |= PlatformInformation::CPU_FEATURE_CMOV;
                    if (result._edx & CPUID_STD_MMX)
                        features |= PlatformInformation::CPU_FEATURE_MMX;
                    if (result._edx & CPUID_STD_SSE)
                        features |= PlatformInformation::CPU_FEATURE_SSE;
                    if (result._edx & CPUID_STD_SSE2)
                        features |= PlatformInformation::CPU_FEATURE_SSE2;

                    if (result._ecx & CPUID_STD_SSE3)
                        features |= PlatformInformation::CPU_FEATURE_SSE3;

                    // Has extended feature ?
                    if (_performCpuid(0x80000000, result) > 0x80000000)
                    {
                        // Check extended feature
                        _performCpuid(0x80000001, result);

                        if (result._edx & CPUID_EXT_3DNOW)
                            features |= PlatformInformation::CPU_FEATURE_3DNOW;
                        if (result._edx & CPUID_EXT_AMD_3DNOWEXT)
                            features |= PlatformInformation::CPU_FEATURE_3DNOWEXT;
                        if (result._edx & CPUID_EXT_AMD_MMXEXT)
                            features |= PlatformInformation::CPU_FEATURE_MMXEXT;
                    }
                }
            }
        }

        return features;
    }
    //---------------------------------------------------------------------
    static unsigned int _detectCpuFeatures(void)
    {
        unsigned int features = queryCpuFeatures();

        const unsigned int sse_features = PlatformInformation::CPU_FEATURE_SSE |
            PlatformInformation::CPU_FEATURE_SSE2 | PlatformInformation::CPU_FEATURE_SSE3;
        if ((features & sse_features) && !_checkOperatingSystemSupportSSE())
        {
            features &= ~sse_features;
        }

        return features;
    }
    //---------------------------------------------------------------------
	static String _detectCpuIdentifier(void)
    {
		// Supports CPUID instruction ?
		if (_isSupportCpuid())
		{
			CpuidResult result;
			unsigned int nExIds;
			char CPUString[0x20];
			char CPUBrandString[0x40];

			StringUtil::StrStreamType detailedIdentStr;


			// Has standard feature ?
			if (_performCpuid(0, result))
			{
				memset(CPUString, 0, sizeof(CPUString));
				memset(CPUBrandString, 0, sizeof(CPUBrandString));

				*((int*)CPUString) = result._ebx;
				*((int*)(CPUString+4)) = result._edx;
				*((int*)(CPUString+8)) = result._ecx;

				detailedIdentStr << CPUString;

				// Calling _performCpuid with 0x80000000 as the query argument
				// gets the number of valid extended IDs.
				nExIds = _performCpuid(0x80000000, result);

				for (unsigned int i=0x80000000; i<=nExIds; ++i)
				{
					_performCpuid(i, result);

					// Interpret CPU brand string and cache information.
					if  (i == 0x80000002)
                    {
						memcpy(CPUBrandString + 0, &result._eax, sizeof(result._eax));
						memcpy(CPUBrandString + 4, &result._ebx, sizeof(result._ebx));
						memcpy(CPUBrandString + 8, &result._ecx, sizeof(result._ecx));
						memcpy(CPUBrandString + 12, &result._edx, sizeof(result._edx));
                    }
					else if  (i == 0x80000003)
                    {
						memcpy(CPUBrandString + 16 + 0, &result._eax, sizeof(result._eax));
						memcpy(CPUBrandString + 16 + 4, &result._ebx, sizeof(result._ebx));
						memcpy(CPUBrandString + 16 + 8, &result._ecx, sizeof(result._ecx));
						memcpy(CPUBrandString + 16 + 12, &result._edx, sizeof(result._edx));
                    }
					else if  (i == 0x80000004)
                    {
						memcpy(CPUBrandString + 32 + 0, &result._eax, sizeof(result._eax));
						memcpy(CPUBrandString + 32 + 4, &result._ebx, sizeof(result._ebx));
						memcpy(CPUBrandString + 32 + 8, &result._ecx, sizeof(result._ecx));
						memcpy(CPUBrandString + 32 + 12, &result._edx, sizeof(result._edx));
                    }
				}


				String brand(CPUBrandString);
				StringUtil::trim(brand);
				if (!brand.empty())
					detailedIdentStr << ": " << brand;

				return detailedIdentStr.str();
			}
		}

		return "X86";
    }

#else   // IRR_CPU_X86

    //---------------------------------------------------------------------
    static unsigned int _detectCpuFeatures(void)
    {
        return 0;
    }
    //---------------------------------------------------------------------
	static String _detectCpuIdentifier(void)
    {
        return "Unknown";
    }

#endif  // IRR_CPU_X86

    //---------------------------------------------------------------------
    // Platform-independent routines, but the returns value are platform-dependent
    //---------------------------------------------------------------------

	const String& PlatformInformation::getCpuIdentifier(void)
    {
		static const String sIdentifier = _detectCpuIdentifier();
        return sIdentifier;
    }
    //---------------------------------------------------------------------
    unsigned int PlatformInformation::getCpuFeatures(void)
    {
        static const unsigned int sFeatures = _detectCpuFeatures();
        return sFeatures;
    }
	//---------------------------------------------------------------------
	bool PlatformInformation::hasCpuFeature(CpuFeatures feature)
	{
		return (getCpuFeatures() & feature) != 0;
	}
	//---------------------------------------------------------------------
	void PlatformInformation::logPlatformInformation( void )
	{	
		//u32 mem_total, mem_avail, memory;		
		//OSPlatformInformation.OSInfo->getSystemMemory(&mem_total, &mem_avail);
		//memory = mem_total>>10;

		LogManager::logMessage("CPU Identifier & Features", LogManager::Info);
		LogManager::logMessage("-------------------------", LogManager::Info);
		LogManager::logMessage(
			" *   CPU ID: " + getCpuIdentifier(), LogManager::Info);
		//LogManager::logMessage(
		//	" *   MEMORY AVAILABLE: " + memory, LogManager::Info);
#if defined(IRR_CPU_X86)
		if(_isSupportCpuid())
		{
			LogManager::logMessage(
				" *      SSE: " + StringConverter::toString(hasCpuFeature(CPU_FEATURE_SSE), true), LogManager::Info);
			LogManager::logMessage(
				" *     SSE2: " + StringConverter::toString(hasCpuFeature(CPU_FEATURE_SSE2), true), LogManager::Info);
			LogManager::logMessage(
				" *     SSE3: " + StringConverter::toString(hasCpuFeature(CPU_FEATURE_SSE3), true), LogManager::Info);
			LogManager::logMessage(
				" *      MMX: " + StringConverter::toString(hasCpuFeature(CPU_FEATURE_MMX), true), LogManager::Info);
			LogManager::logMessage(
				" *   MMXEXT: " + StringConverter::toString(hasCpuFeature(CPU_FEATURE_MMXEXT), true), LogManager::Info);
			LogManager::logMessage(
				" *    3DNOW: " + StringConverter::toString(hasCpuFeature(CPU_FEATURE_3DNOW), true), LogManager::Info);
			LogManager::logMessage(
				" * 3DNOWEXT: " + StringConverter::toString(hasCpuFeature(CPU_FEATURE_3DNOWEXT), true), LogManager::Info);
			LogManager::logMessage(
				" *     CMOV: " + StringConverter::toString(hasCpuFeature(CPU_FEATURE_CMOV), true), LogManager::Info);
			LogManager::logMessage(
				" *      TSC: " + StringConverter::toString(hasCpuFeature(CPU_FEATURE_TSC), true), LogManager::Info);
			LogManager::logMessage(
				" *      FPU: " + StringConverter::toString(hasCpuFeature(CPU_FEATURE_FPU), true), LogManager::Info);
			LogManager::logMessage(
				" *      PRO: " + StringConverter::toString(hasCpuFeature(CPU_FEATURE_PRO), true), LogManager::Info);
			LogManager::logMessage(
				" *       HT: " + StringConverter::toString(hasCpuFeature(CPU_FEATURE_HTT), true), LogManager::Info);
		}
#endif
		LogManager::logMessage("-------------------------", LogManager::Info);

	}

}
