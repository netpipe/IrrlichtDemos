#include "CShaderPre.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace irr;
using namespace video;
using namespace core;
using namespace scene;
using namespace io;

struct SDefineExp
{
	SDefineExp() : IfPos(-1), ElsePos(-1), EndPos(-1), IfExp(""), Inverse(false) {};
	s32 IfPos;
	s32 ElsePos;
	s32 EndPos;
	core::stringc IfExp;
	bool Inverse;
};

core::array<SDefineExp> grabDefineExpressions(core::stringc &shaderProgram)
{
	s32 CurrentSearchPos = 1;
	s32 FindHelper = 1;
	s32 FindHelper2 = 1;
	
	core::array<SDefineExp> DefineArray;

	// Dont bother stripping comments if theres no defines.
	if(CurrentSearchPos = shaderProgram.find("##ifdef") == -1)
		return DefineArray;

	// Strip all comments, they get in the way.
	while((CurrentSearchPos = shaderProgram.find("//")) > -1)
	{
		FindHelper = shaderProgram.findNext('\n',CurrentSearchPos);
		if(FindHelper != -1)
			for(u32 i = CurrentSearchPos;i < (u32)FindHelper;++i)
				shaderProgram[i] = ' ';
		else
			for(u32 i = CurrentSearchPos;i < shaderProgram.size();++i)
				shaderProgram[i] = ' ';
	}

	while((CurrentSearchPos = shaderProgram.find("/*")) > -1)
	{
		FindHelper = shaderProgram.find("*/");
		if(FindHelper > CurrentSearchPos)
			for(u32 i = CurrentSearchPos;i <= (u32)(FindHelper + 1);++i)
				shaderProgram[i] = ' ';
		else
			for(u32 i = CurrentSearchPos;i < shaderProgram.size();++i)
				shaderProgram[i] = ' ';
	}

	while((CurrentSearchPos = shaderProgram.find("##ifdef")) > -1)
	{
		SDefineExp DExp;
		
		DExp.IfPos = CurrentSearchPos;
		
		// Comment out the ##ifdef so that we do not find it again, and so that the compiler ignores it.
		shaderProgram[CurrentSearchPos] = '/';
		shaderProgram[CurrentSearchPos + 1] = '/';

		FindHelper = shaderProgram.findNext(' ',CurrentSearchPos);
		FindHelper2 = shaderProgram.findNext('\n',FindHelper);

		if(FindHelper == -1 || FindHelper2 == -1)
		{
			std::cerr << "Shader preprocessor encountered invalid if statement." << std::endl;
			return DefineArray;
		}

		// Find the appropriate expression and trim all white space.
		DExp.IfExp = shaderProgram.subString(FindHelper,FindHelper2 - FindHelper);
		DExp.IfExp.trim();
		
		// Record if its inverse and remove ! sign from expression.
		if(DExp.IfExp[0] == '!')
		{
			DExp.IfExp[0] = ' ';
			DExp.IfExp.trim();
			DExp.Inverse = true;
		}

		bool EndIfFound = false;

		FindHelper2 = CurrentSearchPos;
		s32 IfEndScope = 0;

		while(!EndIfFound)
		{
			FindHelper = shaderProgram.findNext('#',FindHelper2);

			if(FindHelper == -1 || FindHelper >= (s32)(shaderProgram.size() - 3))
			{
				std::cerr << "Shader preprocessor encountered unmatched if statement." << std::endl;
				return DefineArray;
			}

			if(IfEndScope < 0)
			{
				std::cerr << "Shader preprocessor encountered unmatched endif statement." << std::endl;
				return DefineArray;
			}
			
			if(shaderProgram[FindHelper + 1] != '#')
			{
				FindHelper2 = FindHelper + 1;
				continue;
			}
			else if(shaderProgram[FindHelper + 2] == 'i')
			{
				IfEndScope++;
			}
			else if(shaderProgram[FindHelper + 2] == 'e' && shaderProgram[FindHelper + 3] == 'n')
			{
				if(IfEndScope == 0)
					break;

				IfEndScope--;
			}
			else if(shaderProgram[FindHelper + 2] == 'e' && shaderProgram[FindHelper + 3] == 'l')
			{
				if(IfEndScope == 0)
				{
					if(DExp.ElsePos != -1)
					{
						std::cerr << "Shader preprocessor encountered duplicate else statements per if statement." << std::endl;
						return DefineArray;
					}

					// Comment out the ##else so that we do not find it again, and so that the compiler ignores it.
					shaderProgram[FindHelper] = '/';
					shaderProgram[FindHelper + 1] = '/';

					DExp.ElsePos = FindHelper;
				}
			}
			
			FindHelper2 = FindHelper + 2;
		}

		// Comment out the ##endif so that we do not find it again, and so that the compiler ignores it.
		shaderProgram[FindHelper] = '/';
		shaderProgram[FindHelper + 1] = '/';

		DExp.EndPos = FindHelper;
		
		// Add the define expression to the array.
		DefineArray.push_back(DExp);
	}

	return DefineArray;
}

CShaderPreprocessor::CShaderPreprocessor(irr::video::IVideoDriver* driverIn) : driver(driverIn) 
{initDefineMap();};

void CShaderPreprocessor::initDefineMap()
{
	if(driver->queryFeature(EVDF_TEXTURE_NPOT))
		DefineMap["EVDF_TEXTURE_NPOT"] = "";
	if(driver->queryFeature(EVDF_FRAMEBUFFER_OBJECT))
		DefineMap["EVDF_FRAMEBUFFER_OBJECT"] = "";
	if(driver->queryFeature(EVDF_VERTEX_SHADER_1_1))
		DefineMap["EVDF_VERTEX_SHADER_1_1"] = "";
	if(driver->queryFeature(EVDF_VERTEX_SHADER_2_0))
		DefineMap["EVDF_VERTEX_SHADER_2_0"] = "";
	if(driver->queryFeature(EVDF_VERTEX_SHADER_3_0))
		DefineMap["EVDF_VERTEX_SHADER_3_0"] = "";
	if(driver->queryFeature(EVDF_PIXEL_SHADER_1_1))
		DefineMap["EVDF_PIXEL_SHADER_1_1"] = "";
	if(driver->queryFeature(EVDF_PIXEL_SHADER_1_2))
		DefineMap["EVDF_PIXEL_SHADER_1_2"] = "";
	if(driver->queryFeature(EVDF_PIXEL_SHADER_1_3))
		DefineMap["EVDF_PIXEL_SHADER_1_3"] = "";
	if(driver->queryFeature(EVDF_PIXEL_SHADER_1_4))
		DefineMap["EVDF_PIXEL_SHADER_1_4"] = "";
	if(driver->queryFeature(EVDF_PIXEL_SHADER_2_0))
		DefineMap["EVDF_PIXEL_SHADER_2_0"] = "";
	if(driver->queryFeature(EVDF_PIXEL_SHADER_3_0))
		DefineMap["EVDF_PIXEL_SHADER_3_0"] = "";

	// Commented for backwards compatibility.
	//DefineMap[driver->getVendorInfo()] = "";
}

void CShaderPreprocessor::addShaderDefine(const core::stringc name, const core::stringc value)
{
	// No need for this as its already inited at startup.
	//// If DefineMap is empty then initialize it.
	//if(DefineMap.isEmpty())
	//	initDefineMap();

	DefineMap[name] = value;
}

void CShaderPreprocessor::removeShaderDefine(const core::stringc name)
{
	DefineMap.remove(name);
}

//! PreProcesses a shader using Irrlicht's built-in shader preprocessor.
core::stringc CShaderPreprocessor::ppShader(core::stringc shaderProgram)
{
	core::array<SDefineExp> DefineArray = grabDefineExpressions(shaderProgram);

	// No need for this as its already inited at startup.
	//// If DefineMap is empty then initialize it.
	//if(DefineMap.isEmpty())
	//	initDefineMap();

	for(u32 i = 0; i < DefineArray.size();++i)
	{
		if(DefineArray[i].IfPos == -1)
			break;
	
		// Either it is true and not inversed, or it is false, but inversed. 
		// (Wish C++ had a built-in (logical) XOR operator sometimes. :P) 
		if((DefineMap.find(DefineArray[i].IfExp) && !DefineArray[i].Inverse) 
		|| (!DefineMap.find(DefineArray[i].IfExp) && DefineArray[i].Inverse))
		{
			if(DefineArray[i].ElsePos > -1)
			{
				// If there is an else statement then clear the else section.
				if(DefineArray[i].EndPos != -1)
				{
					for(int z = DefineArray[i].ElsePos;z <= DefineArray[i].EndPos + 6;++z)
						shaderProgram[z] = ' ';
				}
			}
		}
		else if(DefineArray[i].ElsePos != -1)
		{
			// If there is an else statement then clear the if section.
			for(int z = DefineArray[i].IfPos;z <= DefineArray[i].ElsePos + 5;++z)
				shaderProgram[z] = ' ';
		}
		else
		{
			// Else just clear the whole block.
			if(DefineArray[i].EndPos != -1)
			{
				for(int z = DefineArray[i].IfPos;z <= DefineArray[i].EndPos + 6;++z)
					shaderProgram[z] = ' ';
			}
		}
	}

	core::map<core::stringc,core::stringc>::ParentFirstIterator DefIter;
	s32 DefFinder = 1;

	// Replace all shader defines.
	for(DefIter = DefineMap.getParentFirstIterator();!DefIter.atEnd();DefIter++)
	{
		if(DefIter->getValue().size() == 0)
			continue;

		// Replace all occurances.
		while((DefFinder = shaderProgram.find(DefIter->getKey().c_str())) > -1)
		{
			// Clear the define from the code.
			for(u32 z = DefFinder;z < DefFinder + DefIter->getKey().size();++z)
				shaderProgram[z] = ' ';

			// Stitch value and shader program together. (Is there a faster way?)
			shaderProgram = shaderProgram.subString(0,DefFinder) + DefIter->getValue() 
					+ shaderProgram.subString(DefFinder,shaderProgram.size() - 1);
		}
	}

	return shaderProgram;
}

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

//! PreProcesses a shader using Irrlicht's built-in shader preprocessor.
core::stringc CShaderPreprocessor::ppShaderFF(core::stringc shaderProgram)
{
	return ppShader(getFileContent(shaderProgram.c_str()).c_str());
}