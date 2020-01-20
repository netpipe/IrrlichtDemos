#include "Configuration.h"

namespace conf
{
	std::string getFileContent(const std::string pFile)
	{
		std::ifstream File(pFile.c_str(), std::ios::in);
		std::string Content;

		if(File.is_open())
		{
			for(std::string Line; std::getline(File, Line);)
				Content += Line + "\n"; 

			File.close();
		}
		
		return Content;
	}

	std::string stripTextFormatting(std::string pText)
	{
		if(!pText.empty())
		{
			//strip spaces at beginning of data-string
			std::string FirstSign; FirstSign += (pText.at(0));
			while(FirstSign == " ")
			{
				pText.erase(0, 1);
										
				FirstSign = "";		
				FirstSign += (pText.at(0));								
			}
			
			//strip spaces at end of data-string
			std::string LastSign; LastSign += (pText.at(pText.size()-1));
			while(LastSign == " ")
			{
				pText.erase(pText.size()-1, 1);
										
				LastSign = "";		
				LastSign += (pText.at(pText.size()-1));							
			}
			
			//strip tab-escape-characters (\t)
			std::string::size_type TabSign = pText.find("\t", 0);
			while(TabSign != std::string::npos)
			{
				pText.erase(TabSign, 1);
				TabSign = pText.find("\t", 0);
			}
			
			//strip mac-escape-characters (\r)
			std::string::size_type MacSign = pText.find("\r", 0);
			while(MacSign != std::string::npos)
			{
				pText.erase(MacSign, 1);
				MacSign = pText.find("\r", 0);
			}
			
			//strip unix-escape-characters (\n)
			std::string::size_type UnSign = pText.find("\n", 0);
			while(UnSign != std::string::npos)
			{
				pText.erase(UnSign, 1);
				UnSign = pText.find("\n", 0);
			}
			
			//strip windows-escape-characters (\r\n)
			std::string::size_type WinSign = pText.find("\r\n", 0);
			while(WinSign != std::string::npos)
			{
				pText.erase(WinSign, 1);
				WinSign = pText.find("\r\n", 0);
			}
		}	
		
		return pText;	
	}
			
	/*std::vector<std::string> getDirectoryContent(std::string pDir, 
		const bool pAppendDirectory, const bool pIncludeDirectories,
		const std::string pExtension, const bool pStripExtensions)
	{
		std::vector<std::string> Values;
		std::vector<std::string> Files;
		
		DIR* DirPointer;
		dirent* CurrentFile;

		//we want a /-sign at the end of the string
		if(pDir[pDir.size()-1] != '/')
			pDir += '/';
		
		if((DirPointer = opendir(pDir.c_str()))) 
		{
			while((CurrentFile = readdir(DirPointer)))
			{
				std::string File = CurrentFile->d_name;
				
				//workaround: . = current directory sign, .. = parent directory sign		
				if(File != "." && File != "..")
				{
					if(!pIncludeDirectories)
					{
						//check if "File" is really a file and no directory
						std::string AbsoluteFile = pDir + File;
						FILE* FilePointer = fopen(AbsoluteFile.c_str(),"r+");

						if(FilePointer)
						{
							fclose(FilePointer);
							Files.push_back(File);
						}
					}
					else
					{
						Files.push_back(File);
					}
				}
			}
			
			//we got all files in the list, close the dir
			closedir(DirPointer);
			
			//filter extensions
			if(pExtension != "")
			{	
				for(unsigned int i = 0; i != Files.size(); ++i)
				{
					//if file name is bigger than extension-name
					if(Files[i].size() > pExtension.size())
					{
						const std::string File = Files[i];
						const int Size = File.size();
						
						std::string Extension = "";
						
						for(unsigned int j = 0; j != pExtension.size(); ++j)
							Extension += File[Size-(pExtension.size()-j)];
						
						if(Extension == pExtension)
							Values.push_back(File);
					}						 
				}
			}
			//dont filter extensions
			else
			{
				Values = Files;
			}
			
			//strip file-name extensions
			if(pStripExtensions)
			{
				for(unsigned int i = 0; i != Values.size(); ++i)
				{
					std::string OldFileName = Values[i];
					std::string NewFileName = "";
					
					std::string Char = "";
					for(unsigned int j = 0; j != OldFileName.size(); ++j)
					{
						Char = OldFileName[j];
						
						if(Char == ".")
							break;
						else								
							NewFileName += OldFileName[j];
					}
					
					Values[i] = NewFileName;
				}				
			}
			
			//append directory to file-names
			if(pAppendDirectory)
			{
				for(unsigned int i = 0; i != Values.size(); ++i)
					Values[i] = pDir + Values[i];
			}
		}
		
		return Values;		
	}	*/

	bool toBoolean(const std::string pData)
	{
		if(pData == "true" || pData == "TRUE" || pData == "True" || pData == "1")
			return true;
		
		return false;
	}

	float toNumber(const std::string pData)
	{
		std::stringstream stream;			
		stream << pData;
		
		float temp = 0.0f;
		stream >> temp;
		
		return temp;
	}

	std::vector<std::string> toArray(const std::string pData)
	{
		std::vector<std::string> Array;
		
		std::string search = pData;
		std::string::size_type Separator = search.find(",", 0);
		
		while(Separator != std::string::npos)
		{
			Array.push_back(stripTextFormatting(search.substr(0, Separator)));
			search.erase(0, Separator+1);

			Separator = search.find(",", 0);
		}
		
		Array.push_back(stripTextFormatting(search));

		return Array;
	}	




	Section::Section(const std::string pName)
	{
		Name = pName;
	}

	void Section::setKeyData(const std::string pKey, const std::string pData)
	{
		Data[pKey] = pData;
	}

	std::string Section::getName() const
	{
		return Name;
	}

	std::string Section::getKeyName(const int pID) const
	{
		return Keys.at(pID);	
	}

	std::string Section::getKeyData(const std::string pKey)
	{
		return Data[pKey];
	}

	int	Section::getKeyCount() const
	{
		return Keys.size();
	}

	void Section::addKey(const std::string pKey, const std::string pData)
	{
		Keys.push_back(pKey);
		Data[pKey] = pData;
	}

	Configuration::Configuration(const std::string pFileName)
	{
		FileName = pFileName;
		
		std::ifstream File (FileName.c_str(), std::ios::in);
		
		//check if the file exists
		if(File.is_open())	
		{
			std::string ActiveSection = "";
			std::string MultiLineKeyName = "";
			std::string MultiLineKeyValue = "";
			
			for(std::string Line; getline(File, Line);)
				Lines.push_back(Line); 
				
			for(unsigned int i = 0; i != Lines.size(); ++i)
			{
				std::string Line = Lines[i];
				
				//clean line from formatting
				Line = stripTextFormatting(Line);
				
				if(!Line.empty())
				{
					//check for missing identifiers 
					//(e.g.: "identifier=value;" -> "=123;")
					//explanation:
					//if equal-sign is not at beginning (position 0) 
					//of the line, continue
					if(Line.find("=", 0) != 0)
					{
						std::string Begin; Begin += (Line.at(0));
						std::string End; End += (Line.at(Line.size()-1));
						
						//if line is not commented out
						if(Begin != "#")
						{
							//is a section
							if(Begin == "[" && End == "]")
							{
								Line.erase(0, 1);
								Line.erase(Line.size()-1, 1);
							
								Line = stripTextFormatting(Line);
								ActiveSection = Line;
								
								if(!SectionMap[Line])
								{	
									Section* sec = new Section(Line);
														
									Sections.push_back(Line);
									SectionMap[Line] = sec;
								}
							}
							//is an entry
							else 
							{
								if(ActiveSection != "")
								{
									std::string::size_type EqualSign = Line.find("=", 0);
									std::string::size_type Semicolon = Line.find(";", 0);
									std::string::size_type Slash	 = Line.find("/", 0);
								
									//complete entry
									if(EqualSign != std::string::npos && Semicolon != std::string::npos)
									{
										std::string KeyName = Line.substr(0, EqualSign);
										std::string KeyValue= Line.substr(EqualSign+1);
										
										KeyName	= stripTextFormatting(KeyName);
										KeyValue = stripTextFormatting(KeyValue);

										//strip the semicolon
										if(!KeyValue.empty())
										{
											KeyValue.erase(KeyValue.size()-1, 1);
										}
										
										getSection(ActiveSection)->addKey(KeyName, KeyValue);
									}	
									//multi-line entry
									else
									{	
										//check for missing slashes
										if(Slash != std::string::npos || Semicolon != std::string::npos)
										{
											//multi-line entry start
											if(EqualSign != std::string::npos && Semicolon == std::string::npos)
											{
												MultiLineKeyName = Line.substr(0, EqualSign);
												MultiLineKeyValue += Line.substr(EqualSign+1, Slash);
												
												//clean out the name
												MultiLineKeyName = stripTextFormatting(MultiLineKeyName);
												
												//strip the slash
												MultiLineKeyValue.erase(MultiLineKeyValue.size()-1, 1);
											}
											//check if identifier is not missing
											if(MultiLineKeyName != "")
											{
												if(EqualSign == std::string::npos && Semicolon == std::string::npos && 
												Slash != std::string::npos)
												{
													MultiLineKeyValue += Line.substr(0, Slash); 
												}										
												//multi-line entry end
												else if(EqualSign == std::string::npos && Semicolon != std::string::npos)
												{
													MultiLineKeyValue += Line.substr(0, Semicolon);
													MultiLineKeyValue = stripTextFormatting(MultiLineKeyValue);
													
													getSection(ActiveSection)->addKey(MultiLineKeyName, MultiLineKeyValue);
													
													MultiLineKeyName = "";
													MultiLineKeyValue = "";
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			
			File.close();
		}
	}
		
	Configuration::~Configuration()
	{
		for(unsigned int i = 0; i != Sections.size(); ++i)
			delete(SectionMap[Sections.at(i)]);
	}

	Section* Configuration::getSection(const std::string pName)
	{
		return SectionMap[pName];
	}
		
	Section* Configuration::getSectionByID(const int pID)
	{
		return getSection(Sections.at(pID));
	}
		
	int Configuration::getSectionCount() const
	{
		return Sections.size();
	}
		
	std::string Configuration::getFileName() const
	{
		return FileName;
	}
};
