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

#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include <string>
#include <vector>

//tolua_begin
namespace peak
{
	class File;
	
	enum EFileMode
	{
		EFM_Read = 0,
		EFM_Write,
		EFM_ReadWrite,
		EFM_Append,
		EFM_ReadAppend
	};
	
	/**
	 * \brief Vrtual file system class taking root directory and file system overlays
	 * into account.
	 *
	 * When a file is opened, first all overlays are checked for the file and then
	 * it is searched for relative to the root directory.
	 */
	class FileSystem
	{
		public:
			/**
			 * \brief Returns pointer to the FileSystem class.
			 *
			 * \return Pointer to the file system
			 */
			static FileSystem *get(void);
			~FileSystem();
			
			/**
			 * \brief Initializes the file system.
			 */
			bool init(void);
			/**
			 * \brief Destroys the file system.
			 */
			bool shutdown(void);
			
			/**
			 * \brief Sets the root directory.
			 * \param path New root directory
			 */
			bool setRoot(std::string path);
			/**
			 * \brief Adds an overlay.
			 *
			 * The newest overlay has the highest priority.
			 * \param path New overlay
			 */
			bool addOverlay(std::string path);
			
			/**
			 * \brief Opens a file with the desired mode.
			 * \param path Relative path of the file
			 * \param mode Mode of the file
			 * \param binary If this is set, line endings are not converted
			 */
			File *openFile(std::string path, EFileMode mode, bool binary = false);
			
			/**
			 * \brief Returns whether a file exists and returns the complete
			 * relative path
			 * \param path Path of the file, relative to the root directory or
			 * any overlay
			 */
			std::string getFileExists(std::string path);
			
			std::vector<std::string> listFiles(std::string directory,
				std::string extension = "", bool recursive = false);
		private:
			//tolua_end
			FileSystem();
			
			std::string root;
			std::vector<std::string> overlays;
			//tolua_begin
	};
}
//tolua_end

#endif
