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

#include <stdarg.h>
#include <string>

#include "nvutil/Singleton.h"

//! Global string containing the name of the log file.
extern NVSG_API std::string NVSG_LOG_FILE;

namespace nvutil {

/** @defgroup logging Logging a certain test results
  * @{
  */

//! Test modes available for logging
typedef enum
{
  TM_NONE = 0         //!< None of the following modes enabled
, TM_TEST             //!< Any test mode 
, TM_BENCH_TEST       //!< Bench test relevant logging
, TM_SHADER_TEST      //!< Shader test relevant logging
} TESTMODE;

//! Type of output function to use with \c Log class objects 
typedef void LOG_OUTPUT_FUNCTION(const char *); 
NVSG_API LOG_OUTPUT_FUNCTION logFileOutput;  //!<  Function to log into a file.


//! Logging test results
/** Provides output dependent on a specified \a TestMode using a certain output function of type 
  * \c LOG_OUTPUT_FUNCTION specified by \a OutputFn.
  */
template <TESTMODE TestMode, LOG_OUTPUT_FUNCTION * OutputFn>
class Log
{
public:

  //! Default constructor
  /** Constructs a \c Log object and disables output by default.
    * Use init to enable output via \a OutputFn. 
    */
  Log() : m_enabled(false) {}
  //! Initialization
  /** Based on \a enable, decides wether to output or not.
    */ 
  void init( bool enable //!< Enable logging if \c true. 
           ); 
  //! Formatted output 
  /** Outputs the format string \a fmt using \a OutputFn.   
  */
  void format( const char * fmt //!< Format string.
             , ...              //!< Optional arguments.
             ) const;
  //! Determine whether a certain test mode is enabled  
  /** \returns \c true if output is enabled, \c fasle otherwise. 
  */
  bool isEnabled() const;

private:
  bool m_enabled;
};

// inline implementations

template <TESTMODE TestMode, LOG_OUTPUT_FUNCTION * OutputFn>
inline void Log<TestMode, OutputFn>::init(bool enable) 
{
  m_enabled = enable;
}

template <TESTMODE TestMode, LOG_OUTPUT_FUNCTION * OutputFn>
inline void Log<TestMode, OutputFn>::format(const char * fmt, ...) const
{
  if ( !m_enabled ) 
  {
    return;
  }
  // walk the parameter list and perform output
  const int bufSize = 1024;
  char outBuf[bufSize]; 
  
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(outBuf, bufSize-1, fmt, ap);
  va_end(ap);

  OutputFn(outBuf);
}

template <TESTMODE TestMode, LOG_OUTPUT_FUNCTION * OutputFn>
inline bool Log<TestMode, OutputFn>::isEnabled() const
{
  return m_enabled;
}

typedef Singleton< Log<TM_TEST,        logFileOutput> > LogTest;       //!< Dedicated type for general test logging
typedef Singleton< Log<TM_BENCH_TEST,  logFileOutput> > LogBenchTest;  //!< Dedicated type for benchmark test logging
typedef Singleton< Log<TM_SHADER_TEST, logFileOutput> > LogShaderTest; //!< Dedicated type for shader test logging
// ... more similar typedefs for additional tests to log

// NVSG_LOG macro defines

/** @defgroup initialization Macros providing initialization of certain test modes for logging
  * @{
  * By default, logging is disabled for all test modes. This macros can be used to initialize
  * logging for a certain test mode:
  * \code
  * NVSG_LOG_BENCH_TEST_INIT(testmode)  // enable bench test looging if testmode==TM_BENCH_TEST
  * NVSG_LOG_SHADER_TEST_INIT(testmode) // enable shader test logging if testmode==TM_SHADER_TEST
  * \endcode
  */

//! Initialize general test logging
/** Enables general test logging if \a TM equals any test mode but \c TM_NONE. 
  */
#define NVSG_LOG_TEST_INIT(TM)         nvutil::LogTest::instance()->init(TM!=nvutil::TM_NONE)
//! Initialize benchmark test logging
/** Enables benchmark test logging if \a TM equals \c TM_BENCH_TEST. 
  */
#define NVSG_LOG_BENCH_TEST_INIT(TM)   nvutil::LogBenchTest::instance()->init(TM==nvutil::TM_BENCH_TEST)    
//! Ininialize shader test logging
/** Enables shader test logging if \a TM equals \c TM_SHADER_TEST. 
  */
#define NVSG_LOG_SHADER_TEST_INIT(TM)  nvutil::LogShaderTest::instance()->init(TM==nvutil::TM_SHADER_TEST)
/** @} */

/** @defgroup output Macros providing formatted output relevant for a certain test modes
  * @{
  * These macros will only output if a specified test mode was enabled before, using 
  * the matching \c NVSG_LOG_TEST_INIT macro.
  * \note As macros cannot handle optional arguments as used with format strings, 
  * clients must use extra brackets for the argument:
  * \code 
  * NVSG_LOG_SHADER_TEST(( "Error: %X\n", getErrCode() ));
  * \endcode
  */

//! Output test relevant format string \a s if output is enabled
#define NVSG_LOG_TEST(s)        { if ( nvutil::LogTest::instance()->isEnabled()       ) { nvutil::LogTest::instance()->format s; }       }
//! Output benchmark test relevant format string \a s if output is enabled
#define NVSG_LOG_BENCH_TEST(s)  { if ( nvutil::LogBenchTest::instance()->isEnabled()  ) { nvutil::LogBenchTest::instance()->format s; }  }
//! Output shader test relevant format string \a s if output is enabled
#define NVSG_LOG_SHADER_TEST(s) { if ( nvutil::LogShaderTest::instance()->isEnabled() ) { nvutil::LogShaderTest::instance()->format s; } }
/** @} */

/** @defgroup queries Macros to inquire if logging is enabled for a certain test mode
  * @{
  * This macros all can be used in conditional statements:
  * \code
  * if ( NVSG_LOG_BENCH_TEST_ENABLED() ) 
  * {
  *    // ...
  * }
  * \endcode
  */

//! Query if any test is enabled
#define NVSG_LOG_TEST_ENABLED()        nvutil::LogTest::instance()->isEnabled()
//! Query if benchmark test is enabled
#define NVSG_LOG_BENCH_TEST_ENABLED()  nvutil::LogBenchTest::instance()->isEnabled()
//! Query if shader test is enabled
#define NVSG_LOG_SHADER_TEST_ENABLED() nvutil::LogShaderTest::instance()->isEnabled()
/** @} */

/** @} */
} // namespace nvutil
