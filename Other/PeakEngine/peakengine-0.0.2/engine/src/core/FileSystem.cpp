/*
Copyright (C) 2008  Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "core/FileSystem.h"
#include "core/File.h"
#include "core/Logger.h"

#ifdef unix
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#else
#include <windows.h>
#endif

#include <unistd.h>


namespace peak
{
	FileSystem *FileSystem::get(void)
	{
		static FileSystem fs;
		return &fs;
	}
	FileSystem::FileSystem()
	{
		root = ".";
	}
	FileSystem::~FileSystem()
	{
	}

	bool FileSystem::init(void)
	{
		return true;
	}
	bool FileSystem::shutdown(void)
	{
		return true;
	}

	bool FileSystem::setRoot(std::string path)
	{
		root = path;
		return true;
	}
	bool FileSystem::addOverlay(std::string path)
	{
		overlays.push_back(path);
		return true;
	}

	File *FileSystem::openFile(std::string path, EFileMode mode, bool binary)
	{
		const char *modestr = 0;
		switch (mode)
		{
			case EFM_Read:
				modestr = "r";
				break;
			case EFM_Write:
				modestr = "w";
				break;
			case EFM_ReadWrite:
				modestr = "w+";
				break;
			case EFM_Append:
				modestr = "a";
				break;
			case EFM_ReadAppend:
				modestr = "a+";
				break;
			default:
				modestr = "r";
				break;
		}
		// Check overlays
		for (unsigned int i = 0; i < overlays.size(); i++)
		{
			FILE *file = fopen((overlays[overlays.size() - 1 - i] + "/" + path).c_str(), modestr);
			if (file)
			{
				fclose(file);
				return new File((overlays[overlays.size() - 1 - i] + "/" + path).c_str(), mode);
			}
		}
		// Check normal root
		FILE *file = fopen((root + "/" + path).c_str(), modestr);
		if (file)
		{
			fclose(file);
			return new File((root + "/" + path).c_str(), mode, binary);
		}

		return 0;
	}

	std::string FileSystem::getFileExists(std::string path)
	{
		// Check overlays
		for (unsigned int i = 0; i < overlays.size(); i++)
		{
			FILE *file = fopen((overlays[overlays.size() - 1 - i] + "/" + path).c_str(), "r");
			if (file)
			{
				fclose(file);
				return overlays[overlays.size() - 1 - i] + "/" + path;
			}
		}
		// Check normal root
		FILE *file = fopen((root + "/" + path).c_str(), "r");
		if (file)
		{
			fclose(file);
			return root + "/" + path;
		}

		return "";
	}

	std::vector<std::string> FileSystem::listFiles(std::string directory, std::string extension, bool recursive)
	{
		std::vector<std::string> files;
		#ifdef __unix__

		std::vector<std::string> directories;

		std::string fulldirname = root + "/" + directory;
		DIR *dir = opendir(fulldirname.c_str());
		if (!dir)
		{
			LERROR("Could not read directory \"%s\".\n", fulldirname.c_str());
			return std::vector<std::string>();
		}

		while (1)
		{
			struct dirent *dirent = readdir(dir);
			if (!dirent) break;
			if (strcmp(dirent->d_name, "..") && strcmp(dirent->d_name, "."))
			{
				std::string filename = fulldirname + "/" + dirent->d_name;
				int file = open(filename.c_str(), O_RDONLY);
				if (!file)
				{
					printf("Could not open file \"%s\".\n", filename.c_str());
					continue;
				}
				struct stat stat;
				fstat(file, &stat);
				close(file);

				if (stat.st_mode & S_IFDIR)
				{
					if (recursive)
					{
						if (directory != "")
						{
							directories.push_back(directory + "/" + dirent->d_name);
						}
						else
						{
							directories.push_back(dirent->d_name);
						}
					}
				}
				else if (stat.st_mode & S_IFREG)
				{
					std::string ext = filename.substr(filename.size() - extension.size(), extension.size());
					if (ext == extension)
					{
						files.push_back(dirent->d_name);
					}
				}
			}
		}
		closedir(dir);

		// TODO: Scan subdirectories
		/*for (unsigned int i = 0; i < directories.size(); i++)
		{
			std::vector<std::string> morefiles = listFiles(directories[i], true);
		}*/
		#else
		// TODO: Recursive scanning
		std::string pattern = root + "/" + directory + "/*" + extension;
		WIN32_FIND_DATA finddata;
        HANDLE findhandle = FindFirstFile(pattern.c_str(), &finddata);
        if (findhandle == INVALID_HANDLE_VALUE)
        {
            return files;
        }
        files.push_back(finddata.cFileName);
        while (FindNextFile(findhandle, &finddata))
        {
        	files.push_back(finddata.cFileName);
        }
        FindClose(findhandle);
		#endif
		return files;
	}
}

