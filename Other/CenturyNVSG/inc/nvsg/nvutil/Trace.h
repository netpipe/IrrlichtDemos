// Copyright NVIDIA Corporation 2002-2005
// TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
// *AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
// BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES 

#pragma once
/** \file */

#include "nvsg/nvsgapi.h"  // storage-class defines

#include "nvutil/nvutil.h"

#include <string>

#ifndef LINUX
# include <conio.h>
# include "nvutil/Registry.h"
#else
# include <stdio.h>
# include <stdarg.h>
#endif

//! Subkey of Registry where tracing is enabled/disabled on function level.
extern NVSG_API const char * REG_NV_TRACE_SUBKEY;

namespace nvutil {


//! Define for the trace log file name.
#define TRACE_LOG_FILE       "QuadroVR.log" /* log file for debug printouts */

//
// a certain output functors
//

//! output to attached debugger
struct traceDebugOutput
{ 
  //! Function call operator to output a string
  void operator()(const char * str)
  {
#ifdef _WIN32
  #ifndef UNICODE
    OutputDebugStringA(str);
  #endif
#elif defined(LINUX)
    printf("%s", str);
#endif
  }
};

//! output to console
struct traceConsoleOutput
{ 
  //! Function call operator to output a string
  void operator()(const char * str)
  {
#ifdef _WIN32
  _cputs(str); 
#endif
  }
};

//! output to console _AND_ attached debugger
struct traceConsoleDebugOutput
{ 
  //! Function call operator to output a string
  void operator()(const char * str)
  {
#ifdef _WIN32
    traceDebugOutput()(str); 
    traceConsoleOutput()(str); 
#endif
  }
};

//! output to file
struct traceFileOutput
{
  //! Function call operator to output a string
  void operator()(const char * str)
  {
#ifdef _WIN32
    FILE * fp = fopen(TRACE_LOG_FILE, "a");
    if ( fp ) 
    {
      fputs(str, fp);
      fclose(fp);
    }
#endif
  }
};

//! output to log file _AND_ attached debugger
struct traceFileDebugOutput
{
  //! Function call operator to output a string
  void operator()(const char * str)
  {
#ifdef _WIN32
    traceFileOutput()(str);
    traceDebugOutput()(str);
#endif
  }
};

//! output to log file _AND_ console
struct traceFileConsoleOutput
{
  //! Function call operator to output a string
  void operator()(const char * str)
  {
#ifdef _WIN32
    traceFileOutput()(str);
    traceConsoleOutput()(str);
#endif
  }
};

//! Trace class template
template <typename Functor>
class Trace
{
public:
  //! Constructor for entering a function \ fn.
  Trace(const char * fn, bool traceOut) : m_fn(NULL), m_traceOut(traceOut) 
  { 
    if (m_traceOut) 
    {
      m_fn = new std::string(fn);
      enter(); 
    } 
  }

  //! Destructor for leaving a function.
  ~Trace() 
  { 
    if (m_traceOut) 
    { 
      leave();
      delete m_fn;
    } 
  }

private:
  void enter() 
  {
    std::string outstr(">>>> Enter ");
    outstr += m_fn->c_str();
    outstr += "\n";
    Functor()(outstr.c_str());
  }

  void leave() 
  {
    std::string outstr("<<<< Leave ");
    outstr += m_fn->c_str();
    outstr += "\n";
    Functor()(outstr.c_str());
  }

  std::string * m_fn;
  bool m_traceOut;
};

//! uses the function given as template argument to print out a given string
template <typename OutFunctor>
struct TraceFunctor
{
  //! Function call operator to print out a string
  void operator()(const char* str) { OutFunctor()(str); }
  
  //! Formatted output of a variable number of arguments.
  void format(const char* fmt, ...)
  {
    const int bufSize = 1024;
    char outBuf[bufSize]; 
    
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(outBuf, bufSize-1, fmt, ap);
    va_end(ap);

    OutFunctor()(outBuf);
  }
};


#if !defined(NDEBUG) && defined(_WIN32) && !defined(UNICODE) 
// uncomment one of the following for desired tracing
# define OutputString traceDebugOutput
//# define OutputString traceConsoleOutput
//# define OutputString traceConsoleDebugOutput 
//# define OutputString traceFileOutput
//# define OutputString traceFileDebugOutput
//# define OutputString traceFileConsoleOutput
# define NVSG_TRACE()                                                       \
  static DWORD dwTraceEnabled = RegVal<DWORD>( __FUNCTION__              \
                                             , REG_NV_TRACE_SUBKEY       \
                                             , REG_DWORD_LITTLE_ENDIAN   \
                                             , HKEY_CURRENT_USER        \
                                             , DWORD(0) );               \
  Trace<TraceFunctor<OutputString> >  traceObject(__FUNCTION__, dwTraceEnabled!=0); 
// NVSG_TRACE_OUT(s) - string output 
// requires TRACE() macro to be expanded before within the same function
# define NVSG_TRACE_OUT(s)   \
  if ( dwTraceEnabled ) { TraceFunctor<OutputString>()(s); }
// NVSG_TRACE_OUT_F(s) - formatted string output 
// USAGE: NVSG_TRACE_OUT_F((const char* fmt, ...)) 
//                      ^                    ^
// requires TRACE() macro to be expanded before within the same function
# define NVSG_TRACE_OUT_F(s) \
  if ( dwTraceEnabled ) { TraceFunctor<OutputString>().format s; }

#else // !defined(NDEBUG) && defined(_WIN32)

# define NVSG_TRACE()
# define NVSG_TRACE_OUT(s)
# define NVSG_TRACE_OUT_F(s)

#endif // !NDEBUG

} // namespace nvutil

// avoid tedious, repeated qualification of namespace scope
#if !defined(NDEBUG) && defined(_WIN32)

using nvutil::RegVal;
using nvutil::traceConsoleDebugOutput;
using nvutil::traceConsoleOutput;
using nvutil::traceDebugOutput;
using nvutil::traceFileConsoleOutput;
using nvutil::traceFileDebugOutput;
using nvutil::traceFileOutput;
using nvutil::TraceFunctor;
using nvutil::Trace;

#endif // !defined(NDEBUG) && defined(_WIN32)

