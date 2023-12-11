
#include "utils/file.h"

namespace utils
{
namespace file
{

core::stringc get(const io::path &fileName)
{
	u32 len = os::path::size(fileName);
	
	if (len > 0)
	{
		FILE *fp = fopen(fileName.c_str(), "rb");
		
		if (fp)
		{
			char *buf = new char[len+1];
			
			int read = fread(buf, sizeof(char), len, fp);
			
			ASSERT( read <= len );
			buf[read] = 0;
			
			core::stringc result = buf;
			
			delete [] buf;
			fclose(fp);
			
			return result;
		}
	}
	
	return "";
}

bool put(const io::path &fileName, const core::stringc &str)
{
	FILE *fp = fopen(fileName.c_str(), "wb");
	
	if (fp)
	{
		size_t written = fwrite(str.c_str(), sizeof(char), str.size(), fp);
		fclose(fp);
		
		if (written < str.size())
			return false;
		else
			return true;
	}
	
	return false;
}

std::vector<core::stringc> get_lines(const io::path &fileName)
{
	return str::explode_chars("\r\n", get(fileName));
}

bool put_lines(const io::path &fileName, const std::vector<core::stringc> &lines)
{
	core::stringc contents;
	
	for (u32 i = 0; i < lines.size(); i ++)
	{
		contents += lines[i];
		
		if (i != lines.size()-1)
			contents += os::newline();
	}
	
	return put(fileName, contents);
}

bool savesettings(const io::path &fileName, const VariantMap &settings)
{
	std::vector<core::stringc> lines;
	
	// Through map, each key => value pair is a line.
	
	for (VariantMap::const_iterator i = settings.begin(); i != settings.end(); i ++)
		lines.push_back( i->first + core::stringc("=") + i->second.To<core::stringc>() );
	
	if (!lines.size())
		return true; // Nothing to write...
	
	return put_lines(fileName, lines);
}

VariantMap loadsettings(const io::path &fileName)
{
	VariantMap settings;
	
	std::vector<core::stringc> lines = get_lines(fileName);
	
	if (!lines.size())
		return settings;
	
	for (u32 i = 0; i < lines.size(); i ++)
	{
		std::vector<core::stringc> parts = str::explode_at_assignment(lines[i]);
		
		if (parts[0].size() && parts[1].size())
			settings[parts[0]] = parts[1];
	}
	
	return settings;
}

}
}

