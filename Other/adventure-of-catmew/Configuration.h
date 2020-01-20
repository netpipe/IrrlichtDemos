#ifndef _ConfigurationHeader_
#define _ConfigurationHeader_

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>

namespace conf
{
	//extern std::vector<std::string> getDirectoryContent(std::string pDir, 
	//	const bool pAppendDirectory = false, const bool pIncludeDirectories = false,
	//	const std::string pExtension = "", const bool pStripExtensions = false);
		
	extern std::string getFileContent(const std::string pFile);
	extern std::string stripTextFormatting(std::string pText);		
	extern bool toBoolean(const std::string pData);
	extern float toNumber(const std::string pData);
	extern std::vector<std::string> toArray(const std::string pData);
	

	class Section
	{
		public:			
			Section(const std::string pName);
			~Section() {}
			
			void setKeyData(const std::string pKey, const std::string pData);

			std::string getName() const;
			std::string getKeyName(const int pID) const;
			std::string getKeyData(const std::string pKey);
			int	getKeyCount() const;

			void addKey(const std::string pKey, const std::string pData);
				
		private:				
			std::vector<std::string> Keys;
			std::map<std::string, std::string> Data;
			std::string Name; 		
	};

	class Configuration 
	{
		public:
			Configuration(const std::string pFileName);
			~Configuration();
			
			Section* getSection(const std::string pName);
			Section* getSectionByID(const int pID);
			int getSectionCount() const;
			std::string getFileName() const;
		
		private:			
			std::vector<std::string> Lines;
			std::vector<std::string> Sections;
			std::map<std::string, Section*> SectionMap;
			std::string FileName;
	};
};

#endif

