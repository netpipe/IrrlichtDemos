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

#include "nvsgcommon.h"
#include <vector>

#include <stdio.h>

namespace nvutil
{ 

  //! Find files matching a pattern
  /** Tries to find files matching \a mask and output results (full path)
    * to \a results
    * \par
    * The function tries to lookup the file as follows:
    * -# If mask contains a drive or path, that path will be looked for first
    * -# If it does not contain a path and searchPaths is empty we look inside
    *    current dir for a match
    * -# Look inside the search paths stored in \a searchPaths
    * -# Dive into all the subdirs if \a recursive is set true 
    * 
    * Note: Use FindFileFirst, if you are only interested in the first occurence
    *       of a file.
    *         
    * \returns true if successfull, false otherwise.
    */
  extern NVSG_API bool FindFile( const std::string &mask                  //!< the path/file mask to match
                               , const std::vector<std::string> &searchPaths   //!< the dirs to addtionally search                                             
                               , std::vector<std::string> &results             //!< store results in here
                               , bool recursive=false                //!< search all subdirs also
                               );           
    
  //! Check for the first occurence of a file    
  /** Works as FindFile, but return the first match only to \a nameOut
    */
  extern NVSG_API bool FindFileFirst( const std::string &mask                //!< Name of the file to search for.
                                    , const std::vector<std::string> &searchPaths //!< Collection of search pathes to lookup the file.
                                    , std::string &nameOut                   //!< Holds the full path and filename, if successfull.
                                    , bool recursive=false              //!< search all subdirs also
                                    );

  //! Check if a given file exists
  extern NVSG_API bool FileExists( const std::string &filepath  //<! Input (path+)filename.
                                 );

  //! Extract filename
  /** Extracts the filename with extension from \a path and returns it in \a fname.
    */
  extern NVSG_API void GetFileNameFromPath( const std::string& path //!< Input path and filename.
                                          , std::string& fname      //!< Extracted filename with extension.
                                          ); 
  //! Extract file extension
  /** Extracts the file extension, including leading period from \a path and returns it in \a fext.
    */
  extern NVSG_API void GetFileExtFromPath( const std::string& path //!< Input path and filename.
                                         , std::string& fext       //!< Extracted file extension, including leading period.
                                         );
  //! Extract directory path
  /** Extracts the directory path, including trailing slash from \a path and returns it in \a dir.
    */
  extern NVSG_API void GetDirFromPath( const std::string& path //!< Input path and filename.
                                     , std::string& dir        //!< Extracted directory path, including trailing slash.
                                     );
  //! Extract drive letter
  /** Extracts the optional drive letter, followed by a colon from \a path and returns it in \a drv.
    */
  extern NVSG_API void GetDrvFromPath( const std::string& path //!< Input path and filename.
                                     , std::string& drv        //!< Extracted drive letter, followed by a colon.
                                     );
  //! Extract full path
  /** Extracts the drive and directory from \a path and returns it in \a dir.
    */
  extern NVSG_API void GetDrvAndDirFromPath(const std::string& path //!< Input path and filename.
                                           , std::string& dir             //!< Extracted path, including trailing slash.
                                           );

  extern NVSG_API void SplitPath( const std::string& path
                                , std::string& drv
                                , std::string& dir
                                , std::string& fname
                                , std::string& fext );

#if defined(_WIN32)
  //! Determine the module path **Windows only**
  /** Determines the full path to the module, given the module's name in \a module and returns it in \a path.
    */
  extern NVSG_API void GetModulePath( const std::string& module //!< Module name.
                                    , std::string& path         //!< Holds the full path to the module if the module was found.
                                    );
#else 
  //! Determine the executable module path **Linux only**
  /** Determines the full path to the current executable and returns it in \a path.
    */
  extern NVSG_API void GetModulePath( std::string& path );   //!< Holds the full executable path.
#endif                                    

#if defined(_WIN32)
  //! Determine the module path **Windows only**
  /** Determines the full path of the module, given the module handle in \a hModule and returns it in \a path.
    */
  extern NVSG_API void GetModulePath( const HMODULE hModule //!< Module handle.
                                    , std::string& path          //!< Holds the full path to the module.
                                    );
#endif

  //! Get the current directory for the current process
  /** \a path will hold the current directory if the function succeeds
    * \returns \c true if successful, false otherwise.
    */
  extern NVSG_API bool GetCurrentDir( std::string& path //!< Holds the current directory if the function succeeds.
                                    );

  //! Set the current directory for the current process
  /** Sets the current directory for the current process to \a path and returns the former current directory.
    * \returns A string holding the former current directory.
    */
  extern NVSG_API std::string SetCurrentDir( const std::string& dir //!< Path to be set as current directory.
                                      );
  //! Separate a string into tokens
  /** Consecutive calls to getNextToken() return the tokens of \a inputString
      that are separated by one or multiple occurences of characters from \a delimString.
      If no more tokens are left, an empty string will be returned and hasMoreTokens() will return false.
    */
  class StrTokenizer
  {
    public:
      explicit NVSG_API StrTokenizer( const std::string& delimString );
      NVSG_API ~StrTokenizer();

      NVSG_API void setInput( const std::string& inputString );
      NVSG_API const std::string& getNextToken();
      NVSG_API bool hasMoreTokens() const;

    private:
      size_t m_currentPos;
      std::string m_inputString;
      std::string m_token;
      std::string m_delim;
  };

} // namespace nvutil
