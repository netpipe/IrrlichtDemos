// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#pragma once
#ifndef MANCUSO_RAFFAELE_IRRNEWT_DLL_EXPORTS_876902106469048740
#define MANCUSO_RAFFAELE_IRRNEWT_DLL_EXPORTS_876902106469048740

#pragma warning( disable: 4251 ) //needs to have dll-interface to be use by clint of class...

//temporany

//LINK STATICALLY
//#define IRRNEWT_API 

#ifdef WIN32

#ifdef _MSC_VER

#ifdef IRRNEWT_DLL_EXPORTS
#define IRRNEWT_API __declspec(dllexport)
#else 
#define IRRNEWT_API __declspec(dllimport)
#endif //IRRNEWT_DLL_EXPORTS

#else //ifdef _MSC_VER

//link statically
//#define IRRNEWT_API
#ifdef IRRNEWT_DLL_EXPORTS
#define IRRNEWT_API __declspec(dllexport)
#else  //IRRNEWT_DLL_EXPORTS
#define IRRNEWT_API __declspec(dllimport)
#endif //IRRNEWT_DLL_EXPORTS

#endif //ifdef _MSC_VER


#else //ifdef WIN32

#define IRRNEWT_API

#endif //ifdef WIN32

#endif//#ifndef MANCUSO_RAFFAELE_ARES_PRUNTIME_DLL_EXPORTS

