#include <os.h>
#include "script.h"

namespace irr
{
namespace scene
{
namespace quake3
{

char *SkipWhitespace( char *data, bool &hasNewLines, int &com_lines )
{
	int c;

	while( (c = *data) <= ' ')
	{
		if( !c )
			return 0;

		if( c == '\n' ) {
			hasNewLines = true;
			com_lines++;
		}

		data++;
	}

	return data;
}

E_Q3_TOKEN COM_Parse( char *com_token, int &com_lines, char **data_p, bool allowLineBreaks, bool toLower )
{
	int   c = 0, len;
	bool  hasNewLines = false;
	char  *data;

	data = *data_p;
	len = 0;
	com_token[0] = 0;

	const int A = (int)'A';
    const int Z = (int)'Z';
    const int diff = (int)'a' - A;

	if ( !data )
	{
		*data_p = 0;
		return ETK_TOKEN_EOF;
	}

	while ( 1 )
	{
		data = SkipWhitespace( data, hasNewLines, com_lines );
		if ( !data )
		{
			*data_p = 0;
			return ETK_TOKEN_EOF;
		}
		if ( hasNewLines && !allowLineBreaks )
		{
			*data_p = data;
			return ETK_TOKEN_EOL;
		}

		c = *data;

		// skip double slash comments
		if ( c == '/' && data[1] == '/' )
		{
			data += 2;
			while (*data && *data != '\n') {
				data++;
			}
		}
		// skip /* */ comments
		else if ( c=='/' && data[1] == '*' )
		{
			data += 2;
			while ( *data && ( *data != '*' || data[1] != '/' ) )
			{
				data++;
			}
			if ( *data )
			{
				data += 2;
			}
		}
		else
		{
			break;
		}
	}

	// handle quoted strings
	if (c == '\"')
	{
		data++;
		while (1)
		{
			c = *data++;
			if (c=='\"' || !c)
			{
				com_token[len] = 0;
				*data_p = ( char * ) data;
				return ETK_TOKEN_ENTITY;
			}
			if (len < MAX_TOKEN_CHARS)
			{
				com_token[len] = c;
				len++;
			}
		}
	}

	// parse a regular word
	do
	{
		if (len < MAX_TOKEN_CHARS)
		{
			if (toLower && c>=A && c<=Z )
			   c += diff;
			com_token[len] = c;
			len++;
		}
		data++;
		c = *data;
		if ( c == '\n' )
			com_lines++;
	} while (c>32);

	if (len == MAX_TOKEN_CHARS)
       len = 0;

	com_token[len] = 0;

	*data_p = ( char * ) data;

	switch (*com_token) {
	    case '{': return ETK_TOKEN_START_LIST; break;
	    case '}': return ETK_TOKEN_END_LIST; break;
	    default : return ETK_TOKEN_TOKEN;
	}
}


Script::Script(io::IReadFile* file, u32 size, s32 offset)
    : ToLower(true), AllowLineBreaks(false)
{
    setScript(file, size, offset);
}

Script::Script(const c8* name, const c8* script, u32 size, u32 offset)
    : ToLower(true), AllowLineBreaks(false)
{
    setScript(name, script, size, offset);
}

void Script::setScript(io::IReadFile* file, u32 size, s32 offset)
{
    clear();
    if (!file) {
        os::Printer::log("The parameter file is null");
        return;
    }

    core::stringc sfile;
    sfile = "Script: ";
    sfile += file->getFileName();
    os::Printer::log(sfile.c_str());

    if (offset && !( file->seek(offset, true) ) ) {
        os::Printer::log("Failed offset operation in the file");
        return;
    }

    long rsize;
    if ((rsize= (file->getSize() - file->getPos())) <= 0) {
        os::Printer::log("The position of the beginning of the script does not exist");
        return;
    }

    u32 toread;
    toread = ( (size>0 && size<(u32)rsize) ? size : (u32)rsize );

    Data.set_used(toread + 1);

    s32 readed;
    if ( (readed = file->read(Data.pointer(), toread)) >= 0 ) {
        Data[readed] = 0;
        Name = file->getFileName();
        begin();
    }
    else {
        os::Printer::log("Failed read operation in the file");
        clear();
    }
}

void Script::setScript(const c8* name, const c8* script, u32 size, u32 offset)
{
 // Check parameter.
    clear();
    if (!script) {
        os::Printer::log("The parameter script is null");
        return;
    }

 // Show script name.
    os::Printer::log("Script", name, ELL_INFORMATION);

 // Extracting information from the script: Point of beginning and length.
    u32 rsize,       // Real size: Length.
        maximum,     // Máximun to read.
        valid;       // N char valid to read.
    c8 *p,           // Current char.
       *source;      // Point of beginning: NULL if not exists.

    p = (c8*)script;
    source = NULL;

    rsize = 0;
    maximum = (size > 0 ? size : (u32)(-1));
    valid = 0;

    for (; p && valid < maximum; p++, rsize++) {
       if (rsize >= offset) {
           if (!source)
               source = p;
           valid++;
       }
    }

    if (source == NULL) {
        os::Printer::log("Failed offset operation in the script");
        return;
    }

 // Extracting data.
    u32 toread; // Total char to read.
    toread = ( (size>0 && size<(u32)rsize) ? size : (u32)rsize );

    Data.set_used(toread + 1);

    c8 *dest;
    dest = Data.pointer();
    do { *dest++ = *source++; } while (--toread);
    *dest = 0;

    Name = name;
    begin();
}


void Script::clear()
{
    Name = "";
    Data.set_used(1);
    Data[0] = 0;
    BeginData = Data.pointer();
    COM_Token[0] = 0;
    COM_Lines = 0;
    CurrentToken = Token();
}


void Script::begin()
{
    if (BeginData != Data.pointer())
    {
        BeginData = Data.pointer();
        COM_Token[0] = 0;
        COM_Lines = 0;
        CurrentToken = Token();
    }
}

E_Q3_TOKEN Script::nextToken()
{
    E_Q3_TOKEN type;

    type = COM_Parse(COM_Token, COM_Lines, &BeginData, AllowLineBreaks, ToLower);

    CurrentToken.setType(type);
    CurrentToken.setValue(COM_Token);

    return CurrentToken.getType();
}


} // end namespace quake3
} // end namespace scene
} // end namespace irr

