
#ifndef GET_LINES_H
#define GET_LINES_H

#include "Litha.h"

std::vector<core::stringc> get_lines(io::path path)
{
	io::IFileSystem *filesys = GetEngine()->GetIrrlichtDevice()->getFileSystem();
	
	std::vector<core::stringc> lines;
	
	if (filesys->existFile(path))
	{
		if (io::IReadFile *file = filesys->createAndOpenFile(path))
		{
			c8 *buf = new c8[file->getSize()+1];
			
			s32 readBytes = file->read(buf, file->getSize());
			
			buf[readBytes] = 0;
			
			s32 start = 0;
			
			// Split buffer into lines
			for (s32 i = 0; i < readBytes; i ++)
			{
				if (buf[i] == '\r' || buf[i] == '\n' || i == readBytes-1)
				{
					while ( (buf[i] == '\r' || buf[i] == '\n') && i < readBytes )
					{
						buf[i] = 0;
						i ++;
					}
					
					if (i > start)
						lines.push_back( &buf[start] );
					
					start = i;
				}
			}
			
			delete [] buf;
			
			file->drop();
		}
	}
	
	return lines;
}

#endif
