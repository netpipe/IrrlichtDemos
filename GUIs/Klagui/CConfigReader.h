#ifndef _C_CONFIG_READER_H_
#define _C_CONFIG_READER_H_

#include <string>
#include <fstream>

namespace irr
{
namespace io
{
    class IReadFile;
}
}

using namespace irr;

class CConfigReader
{
public:
    //! Creates a config reader. Call loadFile or loadStream to parse a config file with it.
    CConfigReader();
    
    //! Creates a config reader, reading from a file. The reader does not keep the file open, it buffers it all.
    CConfigReader( const char* filename );
    
    //! Creates a config reader, reading from a stream.
    //! The config reader no longer needs the stream when this constructor returns, so you can safely close it right after.
    CConfigReader( std::istream& stream );
    
    //! Loads a config file from a stream. 
    //! The config reader no longer needs the stream when this method returns, so you can safely close it right after.
    bool loadStream( std::istream& stream );
    
    //! Loads a config file
    bool loadFile( const char* filename );

    //! Loads a config file from an irrlicht read file.
    bool loadReadFile( io::IReadFile* file );
    
    //! Moves on to the next assignment. This should be called before getConfigValue() and getConfigName().
    bool nextConfig();
    
    //! Returns the value of the assignment.
    const char* getConfigValue() const;
    
    //! Returns the name of the variable that has been assigned.
    const char* getConfigName() const;
    
private:
    std::string Text;
    const char* TextPtr;    // 'TextPtr' points to a char inside 'Text'.
    
    std::string ConfigValue;
    std::string ConfigName;
};

#endif
