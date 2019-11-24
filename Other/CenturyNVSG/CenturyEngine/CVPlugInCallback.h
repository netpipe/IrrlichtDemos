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

#include  <fstream>
#include  <sstream>
#include  <iostream>
#include  "nvutil/PlugInCallback.h"
#include  "MessageBox.h"

#pragma warning(disable: 4267)  // disable warning C4267: possible loss of data, okay to ignore


class CVPlugInCallback : public nvutil::PlugInCallback
{
  public:
    CVPlugInCallback( const std::string &path );

    virtual void onError( PIC_ERROR eid, const void *info ) const;
    virtual bool onWarning( PIC_WARNING wid, const void *info ) const;

    virtual void onUnexpectedEndOfFile( size_t position ) const;
    virtual void onUnexpectedToken( size_t position, const std::string &expected, const std::string &encountered ) const;
    virtual void onUnknownToken( size_t position, const std::string &context, const std::string &token ) const;

    virtual void onFileAccessFailed(const std::string& file, unsigned int systemSpecificErrorCode) const;
    virtual void onFileMappingFailed(unsigned int systemSpecificErrorCode) const; 
    virtual void onImcompatibleFile(const std::string& file, const std::string& context, unsigned int expectedVersion, unsigned int detectedVersion) const; 
    virtual void onInvalidFile(const std::string& file, const std::string& context) const;

    virtual bool onEmptyToken( size_t position, const std::string &context, const std::string &token ) const;
    virtual bool onFileEmpty( const std::string &file ) const;
    virtual bool onFileNotFound( const std::string &file ) const;
    virtual bool onFilesNotFound( const std::vector<std::string> &files ) const;
    virtual bool onIncompatibleValues( size_t position, const std::string &context, const std::string &value0Name,
                                       int value0, const std::string &value1Name, int value1 ) const;
    virtual bool onIncompatibleValues( size_t position, const std::string &context, const std::string &value0Name,
                                       float value0, const std::string &value1Name, float value1 ) const;
    virtual bool onInvalidValue( size_t position, const std::string &context, const std::string &valueName, int value ) const;
    virtual bool onInvalidValue( size_t position, const std::string &context, const std::string &valueName, float value ) const;
    virtual bool onUndefinedToken( size_t position, const std::string &context, const std::string &token ) const;
    virtual bool onUnsupportedToken( size_t position, const std::string &context, const std::string &token ) const;
    virtual bool onDegenerateGeometry( size_t position, const std::string &name ) const;

    virtual void logMessage( int severity, const std::string& msg ) const;

  protected: 
    //! Protected destructor to prevent explicit creation on stack.
    virtual ~CVPlugInCallback();

  private:
    mutable std::ofstream  m_fh;
};

inline  CVPlugInCallback::CVPlugInCallback( const std::string &logFile )
: m_fh( logFile.c_str() )
{
}

inline  CVPlugInCallback::~CVPlugInCallback()
{
}

inline void CVPlugInCallback::logMessage(int severity, const std::string & msg ) const
{
  std::cerr << msg;
}

inline  void  CVPlugInCallback::onError( PIC_ERROR eid, const void *info ) const
{
  std::ostringstream message;
  message << "Unknown error: " << eid;
  MessageBox( "Error", message.str().c_str() );
  PlugInCallback::onError( eid, info );
}

inline  bool  CVPlugInCallback::onWarning( PIC_WARNING wid, const void *info ) const
{
  std::ostringstream msg;

  msg << "Unknown warning: " << wid << "\n";

  m_fh << msg.str();
 
  logMessage( 1, msg.str() );

  return( true );
}


inline void CVPlugInCallback::onUnexpectedEndOfFile( size_t position ) const
{
  std::ostringstream message;
  message << "Encountered unexpected end of file after line " << position;
  MessageBox( "Error", message.str().c_str() );
  PlugInCallback::onError( PICE_UNEXPECTED_EOF, NULL );
}

inline void CVPlugInCallback::onUnexpectedToken( size_t position, const std::string &expected, const std::string &encountered ) const
{
  std::ostringstream message;
  message << "Line " << position << ": Encountered unexpected token \"" << encountered << "\" instead of \"" << expected << "\"";
  MessageBox( "Error", message.str().c_str() );
  PlugInCallback::onError( PICE_UNEXPECTED_TOKEN, NULL );
}

inline void CVPlugInCallback::onUnknownToken( size_t position, const std::string &context, const std::string &token ) const
{
  std::ostringstream message;
  message << "Line " << position << ": Encountered unknown token \"" << token << "\" in context \"" << context << "\"";
  MessageBox( "Error", message.str().c_str() );
  PlugInCallback::onError( PICE_UNKNOWN_TOKEN, NULL );
}

inline void CVPlugInCallback::onFileAccessFailed(const std::string& file, unsigned int systemSpecificErrorCode) const
{
  std::ostringstream message;
  message << "Cannot access " << file << "\nError code: " << systemSpecificErrorCode;
  MessageBox( "Error", message.str().c_str() );
  PlugInCallback::onError( PICE_FILE_ACCESS_FAILED, NULL );
}

inline void CVPlugInCallback::onFileMappingFailed(unsigned int systemSpecificErrorCode) const
{
  std::ostringstream message;
  message << "File mapping failed! Error code: " << systemSpecificErrorCode;
  MessageBox( "Error", message.str().c_str() );
  PlugInCallback::onError( PICE_FILE_MAPPING_FAILED, NULL );
}

inline void CVPlugInCallback::onImcompatibleFile(const std::string& file, const std::string& context, unsigned int expectedVersion, unsigned int detectedVersion) const
{
  std::ostringstream message;
  unsigned short expected[2];
  unsigned short detected[2];

  *(unsigned int*)expected = expectedVersion;
  *(unsigned int*)detected = detectedVersion;

  message << "Incompatible file!\n"
          << file << "\n"
          << context << " version conflict detected!\n" 
          << "Expected Version: " << expected[1] << "." << expected[0] << "\n"
          << "Detected Version: " << detected[1] << "." << detected[0] << "\n";

  MessageBox( "Error", message.str().c_str() );
  PlugInCallback::onError( PICE_INCOMPATIBLE_FILE, NULL );
}

inline void CVPlugInCallback::onInvalidFile(const std::string& file, const std::string& context) const
{
  std::ostringstream message;
  message << "The file " << file << " is an invalid " << context << " file!";
  MessageBox( "Error", message.str().c_str() );
  PlugInCallback::onError( PICE_INVALID_FILE, NULL );
}

inline  bool  CVPlugInCallback::onEmptyToken( size_t position, const std::string &context, const std::string &token ) const
{
  std::ostringstream msg;

  msg << "Line " << position << ": Encountered empty token \"" << token << "\" in context \"" << context << "\"\n";

  m_fh << msg.str();

  logMessage( 0, msg.str() );

  return( true );
}

inline  bool  CVPlugInCallback::onFileEmpty( const std::string &file ) const
{
  std::ostringstream msg;

  msg << "Warning: File is empty: \"" << file.c_str() << "\"\n";
  m_fh << msg.str();

  logMessage( 1, msg.str() );

  return( true );
}

inline  bool  CVPlugInCallback::onFileNotFound( const std::string &file ) const
{
  std::ostringstream msg;

  msg << "Warning: File not found: \"" << file.c_str() << "\"\n";

  m_fh << msg.str();

  logMessage( 1, msg.str() );

  return( true );
}

inline  bool  CVPlugInCallback::onFilesNotFound( const std::vector<std::string> &files ) const
{
  std::ostringstream message;

  message << "None of the following files have been found:\n";
  for ( size_t i=0 ; i<files.size() ; i++ )
  {
    message << "    " << files[i] << "\n";
  }

  m_fh << message.str();

  logMessage( 1, message.str() );

  return( true );
}

inline  bool  CVPlugInCallback::onIncompatibleValues( size_t position, const std::string &context, const std::string &value0Name,
                                                        int value0, const std::string &value1Name, int value1 ) const
{
  std::ostringstream msg;

  msg << "Line " << position << ": Encountered incompatible values in context \"" << context << "\":\n";
  msg << "    value \"" << value0Name << "\" : " << value0 << "\n";
  msg << "    value \"" << value1Name << "\" : " << value1 << "\n";

  m_fh << msg.str();

  logMessage( 1, msg.str() );

  return( true );
}

inline  bool  CVPlugInCallback::onIncompatibleValues( size_t position, const std::string &context, const std::string &value0Name,
                                                        float value0, const std::string &value1Name, float value1 ) const
{
  std::ostringstream msg;
  msg << "Line " << position << ": Encountered incompatible values in context \"" << context << "\":\n";
  msg << "    value \"" << value0Name << "\" : " << value0 << "\n";
  msg << "    value \"" << value1Name << "\" : " << value1 << "\n";
  m_fh << msg.str();

  logMessage( 1, msg.str() );

  return( true );
}

inline bool CVPlugInCallback::onInvalidValue( size_t position, const std::string &context, const std::string &valueName, int value ) const
{
  std::ostringstream msg;
  msg << "Line " << position << ": Encountered invalid value in context \"" << context << "\": \"" << valueName << "\" = " << value << "\n";
  m_fh << msg.str();

  logMessage( 1, msg.str() );

  return( true );
}

inline bool CVPlugInCallback::onInvalidValue( size_t position, const std::string &context, const std::string &valueName, float value ) const
{
  std::ostringstream msg;
  msg << "Line " << position << ": Encountered invalid value in context \"" << context << "\": \"" << valueName << "\" = " << value << "\n";
  m_fh << msg.str();

  logMessage( 1, msg.str() );

  return( true );
}

inline bool CVPlugInCallback::onUndefinedToken( size_t position, const std::string &context, const std::string &token ) const
{
  std::ostringstream msg;
  msg << "Line " << position << ": Ignoring undefined token \"" << token << "\" in context \"" << context << "\"\n";
  m_fh << msg.str();

  logMessage( 1, msg.str() );

  return( true );
}

inline bool CVPlugInCallback::onUnsupportedToken( size_t position, const std::string &context, const std::string &token ) const
{
  std::ostringstream msg;
  msg << "Line " << position << ": Ignoring unsupported token \"" << token << "\" in context \"" << context << "\"\n";
  m_fh << msg.str();

  logMessage( 1, msg.str() );

  return( true );
}

inline bool CVPlugInCallback::onDegenerateGeometry( size_t position, const std::string &name ) const
{
  std::ostringstream msg;
  msg << "Ignoring degenerate geometry \"" << name << "\"\n";
  m_fh << msg.str();

  logMessage( 1, msg.str() );

  return( true );
}
