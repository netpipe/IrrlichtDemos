// Copyright (C) 2009 JoshM, Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_FBX_LOADER_

#include "CFBXMeshFileLoader.h"
#include "ISkinnedMesh.h"
#include "ISceneManager.h"
#include "IReadFile.h"
#include "irrMap.h"
#include "irrArray.h"
#include "fast_atof.h"
#include <cctype>
#include <iostream>

namespace irr
{
namespace scene
{

//! Constructor
	CFBXMeshFileLoader::CFBXMeshFileLoader(scene::ISceneManager* smgr) : SceneManager(smgr)
{
	#ifdef _DEBUG
	setDebugName("CFBXMeshFileLoader");
	#endif
}


//! returns true if the file maybe is able to be loaded by this class
//! based on the file extension (e.g. ".bsp")
bool CFBXMeshFileLoader::isALoadableFileExtension(const io::path& filename) const
{
        return (core::hasFileExtension( filename, "fbx"));
}


namespace
{

namespace TokenTypes
{
enum E_TokenTypes
{
	OBJECTS,
	MODEL,
	POSE,
	MATERIAL,
	TEXTURE,
	UNKNOWN
};
}

class FBXGrammarConsumer;

struct FBXTokenProcessor
{
	FBXTokenProcessor(void)
	{
		TokenType = TokenTypes::UNKNOWN;
	}

	virtual ~FBXTokenProcessor(void)
	{
		for (u32 i=0; i<SubTokens.size(); ++i)
			delete SubTokens[i];
	}

	virtual void ProcessToken(core::stringc& Token, FBXGrammarConsumer* reader) = 0;

	core::array<FBXTokenProcessor *> SubTokens;
	TokenTypes::E_TokenTypes TokenType;
};

struct ConnectionPair
{
	core::stringc A;
	core::stringc B;
};
struct FBXRootTokenType : public FBXTokenProcessor
{
	virtual void ProcessToken(core::stringc& Token, FBXGrammarConsumer* reader);

	core::stringc CreationTime;
	core::stringc Creator;
	core::array<ConnectionPair> Connections;
};

struct FBXModel;

class FBXGrammarConsumer
{
public:

	FBXGrammarConsumer(io::IReadFile* file) : File(file), FileSize(0), isBinary(false)
	{
		RootToken = new FBXRootTokenType();
		if (File)
			FileSize = File->getSize();
	}

	~FBXGrammarConsumer()
	{
		delete RootToken;
	}

	void skip(long i)
	{
		File->seek(i, true);
	}

	bool readOpenBrace()
	{
		if (isBinary)
			return true;
		while(File->getPos() != FileSize)
		{
			c8 c;
			File->read(&c, 1);
			// found it, so leave
			if (!core::isspace(c))
			{
				return (c=='{');
			}
		}
		return false;
	}

	core::stringc& ConsumeNextToken(core::stringc& token)
	{
		c8 c;
		token = "";
		if (isBinary)
		{
			u8 length;
			File->read(&skipPos,4); // next file pos
			File->read(&structs,4); // structs?
			File->read(&byteSize,4); // size in bytes, including type specifier
			if (skipPos==0)
			{
				File->read(&length, 1); // seems there's one additional 0 byte
				token="}";
				return token;
			}
			File->read(&length, 1);
			for (u32 i=0; i<length; ++i)
			{
				File->read(&c, 1);
				token.append(c);
			}
			token.append(":");
			return token;
		}
		while(File->getPos() != FileSize)
		{
			File->read(&c, 1);
			// found it, so leave
			if (!core::isspace(c))
			{
				File->seek(-1, true);
				break;
			}
		}
		while(File->getPos() != FileSize)
		{
			File->read(&c, 1);
			// found it, so leave
			if (core::isspace(c))
				break;
			if (token.size() && ((c=='{')||(c=='}')))
			{
				File->seek(-1, true);
				break;
			}
			token.append(c);
			if ((c=='{')||(c=='}')||(c==','))
				break;
		}
		return token;
	}

	void ConsumeToEndOfLine()
	{
		if (isBinary)
		{
			if (byteSize)
				File->seek(byteSize, true);
			else
				File->seek(skipPos, false);
			return;
		}
		core::stringc token;
		c8 c;
		while(File->getPos() != FileSize)
		{
			File->read(&c, 1);
			if (c=='\n')
				return;
		}
	}

	void ConsumeBlock()
	{
		if (isBinary)
		{
			File->seek(skipPos, false);
			return;
		}
		core::stringc CurrentToken;

		while (ConsumeNextToken(CurrentToken) != "}")
		{
			if (CurrentToken == "{")
			{
				ConsumeBlock();
			}
		}
	}

	void ConsumeFloatCSV(core::array<float>& ReturnVec)
	{
		if (isBinary)
		{
			ReturnVec.set_used(structs);
			for (int i=0; i<structs; ++i)
				FBXGrammarConsumer::ConsumeFloat(ReturnVec[i]);
		}
		else
		{
			while (true)
			{
				float val;
				if (FBXGrammarConsumer::ConsumeFloat(val))
					ReturnVec.push_back(val);
				else
					break;
			}
		}
	}

	void ConsumeFloatCSV(core::matrix4& returnMat)
	{
		if (isBinary)
		{
			const int m=core::min_(structs,16);
			for (int i=0; i<m; ++i)
				FBXGrammarConsumer::ConsumeFloat(returnMat[i]);
			float v;
			for (int i=m; i<structs; ++i)
				FBXGrammarConsumer::ConsumeFloat(v);
			return;
		}
		u32 i=0;
		while (true)
		{
			float val;
			if (FBXGrammarConsumer::ConsumeFloat(val))
			{
				if (i<16)
					returnMat[i++]=val;
			}
			else
				break;
		}
		returnMat[12] *= -1;
	}

	void ConsumeIntCSV(core::array<int>& ReturnVec)
	{
		if (isBinary)
		{
			ReturnVec.set_used(structs);
			for (int i=0; i<structs; ++i)
				FBXGrammarConsumer::ConsumeInt(ReturnVec[i]);
		}
		else
		{
			while (true)
			{
				int val;
				if (FBXGrammarConsumer::ConsumeInt(val))
					ReturnVec.push_back(val);
				else
					break;
			}
		}
	}

	core::stringc ConsumeString()
	{
		core::stringc token;
		if (isBinary)
		{
			c8 type;
			File->read(&type, 1);
			if (type != 'S')
				std::cout << "Expected 'S' qualifier" << std::endl;
			int length;
			File->read(&length, 4);
			if (length)
			{
				token.reserve(length);
				while (length--)
				{
					File->read(&type, 1);
					token.append(type);
				}
			}
			return token;
		}

		ConsumeNextToken(token);
		s32 pos=0;
		while ((pos=token.findLast('\"')) == 0)
		{
			core::stringc Temptoken;
			ConsumeNextToken(Temptoken);
			token.append(" ");
			token.append(Temptoken);
		}
		return token.subString(1, pos - 1);
	}

	bool ConsumeInt(int& val)
	{
		if (isBinary)
		{
			c8 type;
			File->read(&type, 1);
			if (type != 'I')
				std::cout << "Expected 'I' qualifier" << std::endl;
			File->read(&val, 4);
			return true;
		}
		core::stringc token;
		ConsumeNextToken(token);
		if (token[0]==',')
			ConsumeNextToken(token);
		if ((token[0]!='-') && !isdigit(token[0]))
		{
			File->seek(-1*token.size()-1, true);
			return false;
		}
		val = core::strtol10(token.c_str());
		return true;
	}

	bool ConsumeFloat(float& val)
	{
		if (isBinary)
		{
			c8 type;
			File->read(&type, 1);
			if (type != 'D')
				std::cout << "Expected 'D' qualifier" << std::endl;
			double d;
			File->read(&d, sizeof(d));
			val=(float)d;
			return true;
		}
		core::stringc token;
		ConsumeNextToken(token);
		if (token[0]==',')
			ConsumeNextToken(token);
		if ((token[0]!='.') && (token[0]!='-') && !isdigit(token[0]))
		{
			File->seek(-1*token.size()-1, true);
			return false;
		}
		val = core::fast_atof(token.c_str());
		return true;
	}

	void processFile()
	{
		core::stringc token;
		while (File->getPos()<FileSize)
		{
			RootToken->ProcessToken(ConsumeNextToken(token), this);
		}
	}

	FBXRootTokenType *RootToken;
	bool isBinary;
private:
	io::IReadFile* File;
	long FileSize;
	int skipPos;
	int structs;
	int byteSize;
};

struct FBXDefinitions : public FBXTokenProcessor
{
	virtual void ProcessToken(core::stringc& Token, FBXGrammarConsumer* reader)
	{
		reader->readOpenBrace();
		while (reader->ConsumeNextToken(Token) != "}")
		{
			if (Token == ";")
			{
				reader->ConsumeToEndOfLine();
			}
			else if (Token == "Version:")
			{
				reader->ConsumeInt(Version);
			}
			else if (Token == "Count:")
			{
				reader->ConsumeInt(Count);
			}
			else if (Token == "ObjectType:")
			{
				core::stringc ObjectType = reader->ConsumeString();
				reader->readOpenBrace();
				while (reader->ConsumeNextToken(Token) != "}")
				{
					if (Token == ";")
					{
						reader->ConsumeToEndOfLine();
					}
					else if (Token == "Count:")
					{
						int CountCurrentObjectType;
						reader->ConsumeInt(CountCurrentObjectType);
						DefinitionsMap[ObjectType] = CountCurrentObjectType;
					}
					else if (Token == "{")
					{
						reader->ConsumeBlock();
					}
				}
			}
			else if (Token == "{")
			{
				reader->ConsumeBlock();
			}
		}
	}

	int Version;
	int Count;
	core::map<core::stringc, int> DefinitionsMap;
};

struct FBXDocument : public FBXTokenProcessor
{
	virtual void ProcessToken(core::stringc& Token, FBXGrammarConsumer* reader)
	{
		reader->readOpenBrace();
		while (reader->ConsumeNextToken(Token) != "}")
		{
			if (Token == ";")
			{
				reader->ConsumeToEndOfLine();
			}
			else if (Token == "Name:")
			{
				Name = reader->ConsumeString();
			}
			else if (Token == "{")
			{
				reader->ConsumeBlock();
			}
		}
	}

	core::stringc Name;
};

struct FBXHeaderExtension : public FBXTokenProcessor
{
	virtual void ProcessToken(core::stringc& Token, FBXGrammarConsumer* reader)
	{
		reader->readOpenBrace();
		while (reader->ConsumeNextToken(Token) != "}")
		{
			if (Token == ";")
			{
				reader->ConsumeToEndOfLine();
			}
			else if (Token == "FBXHeaderVersion:")
			{
				reader->ConsumeInt(FBXHeaderVersion);
			}
			else if (Token == "FBXVersion:")
			{
				reader->ConsumeInt(FBXVersion);
			}
			else if (Token == "EncryptionType:")
			{
				reader->ConsumeInt(EncryptionType);
			}
			else if (Token == "Creator:")
			{
				Creator = reader->ConsumeString();
			}
			else if (Token == "CreationTimeStamp:")
			{
				reader->readOpenBrace();
				while ((Token = reader->ConsumeNextToken(Token)) != "}")
				{
					if (Token == "Version:")
					{
						reader->ConsumeInt(CreationTimeStamp_Version);
					}
					else if (Token == "Year:")
					{
						reader->ConsumeInt(CreationTimeStamp_Year);
					}
					else if (Token == "Month:")
					{
						reader->ConsumeInt(CreationTimeStamp_Month);
					}
					else if (Token == "Day:")
					{
						reader->ConsumeInt(CreationTimeStamp_Day);
					}
					else if (Token == "Hour:")
					{
						reader->ConsumeInt(CreationTimeStamp_Hour);
					}
					else if (Token == "Minute:")
					{
						reader->ConsumeInt(CreationTimeStamp_Minute);
					}
					else if (Token == "Second:")
					{
						reader->ConsumeInt(CreationTimeStamp_Second);
					}
					else if (Token == "Millisecond:")
					{
						reader->ConsumeInt(CreationTimeStamp_Millisecond);
					}
					else if (Token == "{")
					{
						reader->ConsumeBlock();
					}
				}
			}
			else if (Token == "OtherFlags:")
			{
				reader->readOpenBrace();
				while (reader->ConsumeNextToken(Token) != "}")
				{
					if (Token == "FlagPLE:")
					{
						reader->ConsumeInt(OtherFlags_FlagPLE);
					}
					else if (Token == "{")
					{
						reader->ConsumeBlock();
					}
				}
			}
			else if (Token == "{")
			{
				reader->ConsumeBlock();
			}
		}
	}

	int FBXHeaderVersion;
	int FBXVersion;
	int EncryptionType;
	int CreationTimeStamp_Version;
	int CreationTimeStamp_Year;
	int CreationTimeStamp_Month;
	int CreationTimeStamp_Day;
	int CreationTimeStamp_Hour;
	int CreationTimeStamp_Minute;
	int CreationTimeStamp_Second;
	int CreationTimeStamp_Millisecond;

	int OtherFlags_FlagPLE;
	core::stringc Creator;
};

struct FBXTexture : public FBXTokenProcessor
{
	FBXTexture()
	{
		TokenType=TokenTypes::TEXTURE;
	}

	virtual void ProcessToken(core::stringc& Token, FBXGrammarConsumer* reader)
	{
		ModelPartName = reader->ConsumeString();
		ModelType = reader->ConsumeString();

		reader->readOpenBrace();
		while (reader->ConsumeNextToken(Token) != "}")
		{
			if (Token == ";")
			{
				reader->ConsumeToEndOfLine();
			}
			else if (Token == "Type:")
			{
				Type = reader->ConsumeString();
			}
			else if (Token == "Version:")
			{
				reader->ConsumeInt(Version);
			}
			else if (Token == "TextureName:")
			{
				TextureName = reader->ConsumeString();
			}
			else if (Token == "FileName:")
			{
				FileName = reader->ConsumeString();
			}
			else if (Token == "Properties60:")
			{
				reader->readOpenBrace();

				while ((Token = reader->ConsumeNextToken(Token)) != "}")
				{
					core::stringc name = reader->ConsumeString();
					core::stringc type = reader->ConsumeString();
					reader->ConsumeString(); // empty?
					core::stringc sVal;
					bool bVal;
					s32 iVal;
					video::SColor colVal;
					core::vector3df vVal;
					f32 fVal;
					if (type == "KString")
					{
						sVal=reader->ConsumeString();
					}
					else if ((type == "enum") || (type == "int"))
					{
						reader->ConsumeInt(iVal);
					}
					else if (type == "bool")
					{
						reader->ConsumeInt(iVal);
						bVal = iVal != 0;
					}
					else if ((type == "Vector3D") || (type == "Vector"))
					{
						reader->ConsumeFloat(vVal.X);
						reader->ConsumeFloat(vVal.Y);
						reader->ConsumeFloat(vVal.Z);
					}
					else if ((type == "ColorRGB") || (type == "Color"))
					{
						reader->ConsumeFloat(fVal);
						colVal.setRed(core::round32(fVal*255.f));
						reader->ConsumeFloat(fVal);
						colVal.setGreen(core::round32(fVal*255.f));
						reader->ConsumeFloat(fVal);
						colVal.setBlue(core::round32(fVal*255.f));
					}
					else if ((type == "Number") || (type == "double"))
					{
						reader->ConsumeFloat(fVal);
					}
					else if (type == "object") // do nothing
					{
					}
					if (name == "Translation")
					{
						t=vVal;
					}
					else if (name == "Rotation")
					{
						r=vVal;
					}
					else if (name == "Scaling")
					{
						s=vVal;
					}
					else if (name == "UseMipMap")
					{
						useMipmap=bVal;
					}
				}
			}
			else if (Token == "{")
			{
				reader->ConsumeBlock();
			}
			else if (reader->isBinary)
			{
				reader->ConsumeBlock();
			}
		}
	}

	core::stringc ModelPartName;
	core::stringc ModelType;
	core::stringc Type;
	core::stringc TextureName;
	core::stringc FileName;

	int Version;
	core::vector3df t,r,s;
	bool useMipmap;
};

struct FBXMaterial : public FBXTokenProcessor
{
	FBXMaterial()
	{
		TokenType=TokenTypes::MATERIAL;
	}

	virtual void ProcessToken(core::stringc& Token, FBXGrammarConsumer* reader)
	{
		ModelPartName = reader->ConsumeString();
		ModelType = reader->ConsumeString();

		reader->readOpenBrace();
		while (reader->ConsumeNextToken(Token) != "}")
		{
			if (Token == ";")
			{
				reader->ConsumeToEndOfLine();
			}
			else if (Token == "Version:")
			{
				reader->ConsumeInt(Version);
			}
			else if (Token == "ShadingModel:")
			{
				ShaderType = reader->ConsumeString();
			}
			else if (Token == "MultiLayer:")
			{
				reader->ConsumeInt(MultiLayer);
			}
			else if (Token == "Properties60:")
			{
				reader->readOpenBrace();

				while ((Token = reader->ConsumeNextToken(Token)) != "}")
				{
					core::stringc name = reader->ConsumeString();
					core::stringc type = reader->ConsumeString();
					core::stringc qualifier = reader->ConsumeString(); // unknown, but changes types
					core::stringc sVal;
					bool bVal;
					s32 iVal;
					video::SColor colVal;
					f32 fVal;
					if (type == "Number" && qualifier == "AU")
						type="Vector3D";
					if (type == "KString")
					{
						sVal=reader->ConsumeString();
					}
					else if (type == "bool")
					{
						reader->ConsumeInt(iVal);
						bVal = iVal != 0;
					}
					else if (type == "int")
					{
						reader->ConsumeInt(iVal);
					}
					else if ((type == "ColorRGB") || (type == "Vector3D") || (type == "Color"))
					{
						reader->ConsumeFloat(fVal);
						colVal.setRed(core::round32(fVal*255.f));
						reader->ConsumeFloat(fVal);
						colVal.setGreen(core::round32(fVal*255.f));
						reader->ConsumeFloat(fVal);
						colVal.setBlue(core::round32(fVal*255.f));
					}
					else if ((type == "double") || (type == "Number"))
					{
						reader->ConsumeFloat(fVal);
					}
					if (name == "EmissiveColor")
					{
						Mat.EmissiveColor=colVal;
					}
					else if (name == "AmbientColor")
					{
						Mat.AmbientColor=colVal;
					}
					else if (name == "DiffuseColor")
					{
						Mat.DiffuseColor=colVal;
					}
					else if (name == "SpecularColor")
					{
						Mat.SpecularColor=colVal;
					}
					else if (name == "ShininessExponent")
					{
						Mat.Shininess=fVal;
					}
				}
			}
			else if (Token == "{")
			{
				reader->ConsumeBlock();
			}
		}
	}

	core::stringc ModelPartName;
	core::stringc ModelType;
	core::stringc ShaderType;

	int Version;
	int MultiLayer;
	video::SMaterial Mat;
};

struct FBXReferences : public FBXTokenProcessor
{
	virtual void ProcessToken(core::stringc& Token, FBXGrammarConsumer* reader)
	{
		reader->readOpenBrace();
		while (reader->ConsumeNextToken(Token) != "}")
		{
			if (Token == ";")
			{
				reader->ConsumeToEndOfLine();
			}
		}
	}

};

struct FBXPose : public FBXTokenProcessor
{
	FBXPose()
	{
		TokenType=TokenTypes::POSE;
	}

	struct PosePair
	{
		core::stringc Name;
		core::matrix4 M;
	};

	virtual void ProcessToken(core::stringc& Token, FBXGrammarConsumer* reader)
	{
		ModelPartName = reader->ConsumeString();
		ModelType = reader->ConsumeString();

		reader->readOpenBrace();
		while (reader->ConsumeNextToken(Token) != "}")
		{
			if (Token == ";")
			{
				reader->ConsumeToEndOfLine();
			}
			else if (Token == "Type:")
			{
				PoseType = reader->ConsumeString();
			}
			else if (Token == "Version:")
			{
				reader->ConsumeInt(Version);
			}
			else if (Token == "NbPoseNodes:")
			{
				reader->ConsumeInt(NumPoseNodes);
			}
			else if (Token == "PoseNode:")
			{
				reader->readOpenBrace();

				PosePair p;

				while ((Token = reader->ConsumeNextToken(Token)) != "}")
				{
					if (Token == "Node:")
					{
						p.Name = reader->ConsumeString();
					}
					else if (Token == "Matrix:")
					{
						reader->ConsumeFloatCSV(p.M);
					}
					else if (Token == "{")
					{
						reader->ConsumeBlock();
					}
				}
				Nodes.push_back(p);
			}
			else if (Token == "{")
			{
				reader->ConsumeBlock();
			}
			else if (reader->isBinary)
				reader->ConsumeBlock();
		}
	}

	core::stringc ModelPartName;
	core::stringc ModelType;
	core::stringc PoseType;

	int Version;
	int NumPoseNodes;
	core::array<PosePair> Nodes;
};

struct FBXNormals
{
	core::array<float> Normals;
	core::stringc MappingInformationTypeForNormals; //"ByVertice"
	core::stringc ReferenceInformationTypeForNormals; //"Direct"
};

struct UVLayer
{
	int LayerNum;
	core::stringc Name;
	core::stringc MappingInformationType; // ByPolygonVertx
	core::stringc ReferenceInformationType; //IndexToDirect
	core::array<float> UV;
	core::array<int> UVIndex;
};

struct FBXMaterials
{
	core::array<int> MaterialIndices;
	core::array<int> TextureIndices;
	core::stringc MappingInformationTypeMaterial; //"ByPolygon"
	core::stringc ReferenceInformationTypeMaterial; //"IndexToDirect"
	core::stringc MappingInformationTypeTexture; //"ByPolygon"
	core::stringc ReferenceInformationTypeTexture; //"IndexToDirect"
};

struct FBXModel : public FBXTokenProcessor
{
	FBXModel()
	{
		TokenType=TokenTypes::MODEL;
	}

	virtual void ProcessToken(core::stringc& Token, FBXGrammarConsumer* reader)
	{
		ModelPartName = reader->ConsumeString();
		ModelType = reader->ConsumeString();

		reader->readOpenBrace();
		while (reader->ConsumeNextToken(Token) != "}")
		{
			if (Token == ";")
			{
				reader->ConsumeToEndOfLine();
			}
			else if (Token == "Version:")
			{
				reader->ConsumeInt(Version);
			}
			else if (Token == "GeometryVersion:")
			{
				reader->ConsumeInt(GeometryVersion);
			}
			else if (Token == "Name:")
			{
				reader->ConsumeString();
			}
			else if (Token == "NodeAttributeName:")
			{
				reader->ConsumeString();
			}
			else if (Token == "Vertices:")
			{
				reader->ConsumeFloatCSV(Vertices);
				std::cout << "Vertices: " << Vertices.size() << std::endl;
			}
			else if (Token == "PolygonVertexIndex:")
			{
				reader->ConsumeIntCSV(PolygonVertexIndices);
				std::cout << "PolygonVertexIndices: " << PolygonVertexIndices.size() << std::endl;
			}
			else if (Token == "Edges:")
			{
				reader->ConsumeIntCSV(Edges);
				std::cout << "Edges: " << Edges.size() << std::endl;
			}
			else if (Token == "LayerElementNormal:")
			{
				int layer;
				reader->ConsumeInt(layer);
				reader->readOpenBrace();

				while (reader->ConsumeNextToken(Token) != "}")
				{
					if (Token == "Normals:")
					{
						Normals.Normals.reallocate(Vertices.size());
						reader->ConsumeFloatCSV(Normals.Normals);
						std::cout << "Normals: " << Normals.Normals.size() << std::endl;
					}
					else if (Token == "Version:")
					{
						int tmp;
						reader->ConsumeInt(tmp);
					}
					else if (Token == "MappingInformationType:")
					{
						Normals.MappingInformationTypeForNormals = reader->ConsumeString();
					}
					else if (Token == "ReferenceInformationType:")
					{
						Normals.ReferenceInformationTypeForNormals = reader->ConsumeString();
					}
					else if (Token == "Name:")
					{
						reader->ConsumeString();
					}
					else if (Token == "{")
					{
						reader->ConsumeBlock();
					}
				}
			}
			else if (Token == "LayerElementUV:")
			{
				UVLayers.push_back(UVLayer());
				UVLayer& CurrentLayer=UVLayers.getLast();
				reader->ConsumeInt(CurrentLayer.LayerNum);
				reader->readOpenBrace();
				while (reader->ConsumeNextToken(Token) != "}")
				{
					if (Token == "Name:")
					{
						CurrentLayer.Name = reader->ConsumeString();
					}
					else if (Token == "Version:")
					{
						int tmp;
						reader->ConsumeInt(tmp);
					}
					else if (Token == "MappingInformationType:")
					{
						CurrentLayer.MappingInformationType = reader->ConsumeString();
					}
					else if (Token == "ReferenceInformationType:")
					{
						CurrentLayer.ReferenceInformationType = reader->ConsumeString();
					}
					else if (Token == "UV:")
					{
						reader->ConsumeFloatCSV(CurrentLayer.UV);
						std::cout << "UVs: " << CurrentLayer.UV.size() << std::endl;
					}
					else if (Token == "UVIndex:")
					{
						reader->ConsumeIntCSV(CurrentLayer.UVIndex);
						std::cout << "UV indices: " << CurrentLayer.UVIndex.size() << std::endl;
					}
					else if (Token == "{")
					{
						reader->ConsumeBlock();
					}
				}
			}
			else if (Token == "LayerElementMaterial:")
			{
				int layer;
				reader->ConsumeInt(layer);
				reader->readOpenBrace();
				while (reader->ConsumeNextToken(Token) != "}")
				{
					if (Token == "Name:")
					{
						reader->ConsumeString();
					}
					else if (Token == "Version:")
					{
						int tmp;
						reader->ConsumeInt(tmp);
					}
					else if (Token == "MappingInformationType:")
					{
						Materials.MappingInformationTypeMaterial = reader->ConsumeString();
					}
					else if (Token == "ReferenceInformationType:")
					{
						Materials.ReferenceInformationTypeMaterial = reader->ConsumeString();
					}
					else if (Token == "Materials:")
					{
						reader->ConsumeIntCSV(Materials.MaterialIndices);
						std::cout << "Materials: " << Materials.MaterialIndices.size() << std::endl;
					}
					else if (Token == "{")
					{
						reader->ConsumeBlock();
					}
				}
			}
			else if (Token == "LayerElementTexture:")
			{
				int layer;
				reader->ConsumeInt(layer);
				reader->readOpenBrace();
				while (reader->ConsumeNextToken(Token) != "}")
				{
					if (Token == "Name:")
					{
						reader->ConsumeString();
					}
					else if (Token == "Version:")
					{
						int tmp;
						reader->ConsumeInt(tmp);
					}
					else if (Token == "MappingInformationType:")
					{
						Materials.MappingInformationTypeTexture = reader->ConsumeString();
					}
					else if (Token == "ReferenceInformationType:")
					{
						Materials.ReferenceInformationTypeTexture = reader->ConsumeString();
					}
					else if (Token == "TextureId:")
					{
						reader->ConsumeIntCSV(Materials.TextureIndices);
						std::cout << "Textures: " << Materials.TextureIndices.size() << std::endl;
					}
					else if (Token == "{")
					{
						reader->ConsumeBlock();
					}
					else if (reader->isBinary)
					{
						reader->ConsumeBlock();
					}
				}
			}
			else if (Token == "{")
			{
				reader->ConsumeBlock();
			}
			else if (reader->isBinary)
				reader->ConsumeBlock();
	//		else
	//			std::cout << "Unknown token:" << Token.c_str() << std::endl;
		}
	}

	int Version;
	int GeometryVersion;
	core::stringc ModelPartName;
	core::stringc ModelType;
	core::array<float> Vertices;
	core::array<int> PolygonVertexIndices;
	core::array<int> Edges;
	FBXNormals Normals;
	FBXMaterials Materials;
	core::array<UVLayer> UVLayers;
};

struct FBXObjects : public FBXTokenProcessor
{
	FBXObjects()
	{
		TokenType = TokenTypes::OBJECTS;
	}

	virtual void ProcessToken(core::stringc& Token, FBXGrammarConsumer* reader)
	{
		reader->readOpenBrace();
		while (reader->ConsumeNextToken(Token) != "}")
		{
			if (Token == ";")
			{
				reader->ConsumeToEndOfLine();
			}
			else if (Token == "Version:")
			{
				reader->ConsumeInt(Version);
			}
			else if (Token == "Model:")
			{
				FBXModel *Node = new FBXModel();
				Node->ProcessToken(Token, reader);
				SubTokens.push_back(Node);
			}
			else if (Token == "Pose:")
			{
				FBXPose *Node = new FBXPose();
				Node->ProcessToken(Token, reader);
				SubTokens.push_back(Node);
			}
			else if (Token == "Material:")
			{
				FBXMaterial Node;
				Node.ProcessToken(Token, reader);
				Materials.push_back(Node);
			}
			else if (Token == "Texture:")
			{
				FBXTexture Node;
				Node.ProcessToken(Token, reader);
				Textures.push_back(Node);
			}
			else if (Token == "{")
			{
				reader->ConsumeBlock();
			}
			else if (reader->isBinary)
			{
				reader->ConsumeBlock();
			}
		}
	}

	int Version;
	core::array<FBXMaterial> Materials;
	core::array<FBXTexture> Textures;
};

void FBXRootTokenType::ProcessToken(core::stringc& Token, FBXGrammarConsumer* reader)
{
	if (Token == "Kaydara")
	{
		reader->ConsumeNextToken(Token); // FBX
		reader->ConsumeNextToken(Token); // Binary
		reader->isBinary=true;
		reader->skip(8);
		reader->ConsumeNextToken(Token); // first normal keyword
	}
	if (Token[0] == ';')
	{
		reader->ConsumeToEndOfLine();
	}
	else if (Token == "FBXHeaderExtension:")
	{
		FBXHeaderExtension *Node = new FBXHeaderExtension();
		Node->ProcessToken(Token, reader);
		SubTokens.push_back(Node);
	}
	else if (Token == "CreationTime:")
	{
		CreationTime = reader->ConsumeString();
	}
	else if (Token == "Creator:")
	{
		Creator = reader->ConsumeString();
	}
	else if (Token == "Definitions:")
	{
		FBXDefinitions *Node = new FBXDefinitions();
		Node->ProcessToken(Token, reader);
		SubTokens.push_back(Node);
	}
	else if (Token == "Document:")
	{
		FBXDocument *Node = new FBXDocument();
		Node->ProcessToken(Token, reader);
		SubTokens.push_back(Node);
	}
	else if (Token == "References:")
	{
		FBXReferences *Node = new FBXReferences();
		Node->ProcessToken(Token, reader);
		SubTokens.push_back(Node);
	}
	else if (Token == "Objects:")
	{
		FBXObjects *Node = new FBXObjects();
		Node->ProcessToken(Token, reader);
		SubTokens.push_back(Node);
	}
	else if (Token == "Connections:")
	{
		reader->readOpenBrace();
		core::stringc type("OO");
		while (reader->ConsumeNextToken(Token) != "}")
		{
			type=reader->ConsumeString();

			Connections.push_back(ConnectionPair());
			Connections.getLast().A=reader->ConsumeString();
			Connections.getLast().B=reader->ConsumeString();
			if ((type=="OP") || type=="PO")
				reader->ConsumeString();
		}
	}
	else if (Token == "{")
	{
		reader->ConsumeBlock();
	}
	else if (Token == "}")
	{
		return;
	}
	else if (reader->isBinary)
		reader->ConsumeBlock();
}

struct FBXPolygon
{
	u32 MaterialID;
	s32 TextureID;
	core::array<u32> Indices;
	bool operator==(const FBXPolygon& other) const
	{
		return (MaterialID==other.MaterialID && TextureID==other.TextureID);
	}
};

void addVertex(u16 index, SSkinMeshBuffer* skin, u32 k, FBXModel* fm)
{
	skin->Indices.push_back(skin->Vertices_Standard.size());
	{
		video::S3DVertex v(-fm->Vertices[3*index], fm->Vertices[3*index+1], fm->Vertices[3*index+2],
			0,0,0, video::SColor(255,128,128,128), 0,0);
		if (fm->Normals.Normals.size())
		{
			if (fm->Normals.MappingInformationTypeForNormals=="ByVertice")
			    v.Normal.set(-fm->Normals.Normals[3*index], fm->Normals.Normals[3*index+1], fm->Normals.Normals[3*index+2]);
			else if (fm->Normals.MappingInformationTypeForNormals=="ByPolygonVertex")
				v.Normal.set(-fm->Normals.Normals[3*k], fm->Normals.Normals[3*k+1], fm->Normals.Normals[3*k+2]);
		}
		if (fm->UVLayers.size())
		{
			if (fm->UVLayers[0].MappingInformationType=="ByVertice")
			{
				if (fm->UVLayers[0].ReferenceInformationType=="Direct")
				    v.TCoords.set(fm->UVLayers[0].UV[2*index], -fm->UVLayers[0].UV[2*index+1]);
				else
				    v.TCoords.set(fm->UVLayers[0].UV[fm->UVLayers[0].UVIndex[2*index]], -fm->UVLayers[0].UV[fm->UVLayers[0].UVIndex[2*index+1]]);
			}
			else if (fm->UVLayers[0].MappingInformationType=="ByPolygonVertex")
			{
				if (fm->UVLayers[0].ReferenceInformationType=="Direct")
					v.TCoords.set(fm->UVLayers[0].UV[2*index], -fm->UVLayers[0].UV[2*index+1]);
				else
					v.TCoords.set(fm->UVLayers[0].UV[2*fm->UVLayers[0].UVIndex[index]], -fm->UVLayers[0].UV[2*fm->UVLayers[0].UVIndex[index]+1]);
			}
		}
		skin->Vertices_Standard.push_back(v);
	}
}

}

//! creates/loads an animated mesh from the file.
//! \return Pointer to the created mesh. Returns 0 if loading failed.
//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
//! See IReferenceCounted::drop() for more information.
IAnimatedMesh* CFBXMeshFileLoader::createMesh(io::IReadFile* file)
{
	FBXGrammarConsumer gc(file);
	gc.processFile();

	std::cout << "Finished parsing, creating mesh." << std::endl;

	ISkinnedMesh* msh = SceneManager->createSkinnedMesh();

	for (unsigned int i = 0; i < gc.RootToken->SubTokens.size(); i++)
	{
		if (gc.RootToken->SubTokens[i]->TokenType == TokenTypes::OBJECTS)
		{
			const core::array<FBXTokenProcessor *>& Objects = gc.RootToken->SubTokens[i]->SubTokens;
			for (unsigned int j = 0; j < Objects.size(); j++)
			{
				if (Objects[j]->TokenType == TokenTypes::MODEL)
				{
					FBXModel* fm = ((FBXModel*)Objects[j]);
					if (fm->ModelType == "Mesh")
					{
						core::array<s32> matIndices;
						core::array<s32> texIndices;
						for (u32 k=0; k<gc.RootToken->Connections.size(); ++k)
						{
							if (gc.RootToken->Connections[k].B==fm->ModelPartName)
							{
								FBXObjects* obj =(FBXObjects*)gc.RootToken->SubTokens[i];
								for (u32 l=0; l<obj->Textures.size(); ++l)
								{
									if (obj->Textures[l].ModelPartName==gc.RootToken->Connections[k].A)
										texIndices.push_back(l);
								}
								for (u32 l=0; l<obj->Materials.size(); ++l)
								{
									if (obj->Materials[l].ModelPartName==gc.RootToken->Connections[k].A)
										matIndices.push_back(l);
								}
							}
						}
						const u32 bufStart=msh->getMeshBufferCount();
						bool allSame=fm->Materials.MappingInformationTypeMaterial=="AllSame";
						core::array<FBXPolygon> polys;
						for (u32 k=0; k<fm->Materials.MaterialIndices.size(); ++k)
						{
							FBXPolygon p;
							p.MaterialID=fm->Materials.MaterialIndices[k];
							if (k<fm->Materials.TextureIndices.size())
								p.TextureID=fm->Materials.TextureIndices[k];
							else
								p.TextureID=-1;
							const s32 pos=polys.linear_reverse_search(p);
							if (pos<0)
								polys.push_back(p);
							else
								polys[pos].Indices.push_back(k);
						}
						for (u32 l=0; l<polys.size(); ++l)
						{
							polys[l].Indices.set_sorted(true);
							scene::SSkinMeshBuffer *skin = msh->addMeshBuffer();
							skin->VertexType = video::EVT_STANDARD;
							FBXObjects* obj = (FBXObjects*)gc.RootToken->SubTokens[i];
							if (polys[l].MaterialID<fm->Materials.MaterialIndices.size())
							{
								const u32 mid = matIndices[fm->Materials.MaterialIndices[polys[l].MaterialID]];
								if (mid<obj->Materials.size())
									skin->Material=obj->Materials[mid].Mat;
							}
							if (polys[l].TextureID>=0)
								skin->Material.setTexture(0, SceneManager->getVideoDriver()->getTexture(((FBXObjects*)gc.RootToken->SubTokens[i])->Textures[texIndices[polys[l].TextureID]].FileName));
						}
						u32 k=0;
						for (u32 l=0; allSame?k<fm->PolygonVertexIndices.size():l<fm->Materials.MaterialIndices.size(); ++l)
						{
							scene::SSkinMeshBuffer *skin = 0;
							if (allSame)
							{
								skin = (SSkinMeshBuffer*)msh->getMeshBuffer(bufStart);
							}
							else
							{
								for (u32 m=0; m<polys.size(); ++m)
								{
									if (polys[m].Indices.binary_search(l) != -1)
									{
										skin=(SSkinMeshBuffer*)msh->getMeshBuffer(bufStart+m);
										break;
									}
								}
							}
							if (!skin)
								continue;

							u32 o=2;
							while (fm->PolygonVertexIndices[k+o]>=0)
								++o;
							u16 index=0;

							addVertex(0xffffffff ^ fm->PolygonVertexIndices[k+o], skin, k, fm);
							addVertex(fm->PolygonVertexIndices[k+o-1], skin, k, fm);
							addVertex(fm->PolygonVertexIndices[k+0], skin, k, fm);
							const u32 p=o;
							while (o>=3)
							{
								--o;
								addVertex(fm->PolygonVertexIndices[k+o], skin, k, fm);
								addVertex(fm->PolygonVertexIndices[k+o-1], skin, k, fm);
								addVertex(fm->PolygonVertexIndices[k+0], skin, k, fm);
							}
							k += p+1;
						}
						for (u32 l=0; l<msh->getMeshBufferCount(); ++l)
						{
							IMeshBuffer *buf = msh->getMeshBuffer(l);
							buf->recalculateBoundingBox();
						}
					}
					else if (fm->ModelType=="LimbNode")
					{
						scene::ISkinnedMesh::SJoint* joint = msh->addJoint(0);
						joint->Name=fm->ModelPartName;
					}
				}
			}
		}
	}
	for (u32 i=0; i<gc.RootToken->Connections.size(); ++i)
	{
		const s32 a = msh->getJointNumber(gc.RootToken->Connections[i].A.c_str());
		const s32 b = msh->getJointNumber(gc.RootToken->Connections[i].B.c_str());
		if ((a != -1) && (b != -1))
		{
			msh->getAllJoints()[b]->Children.push_back(msh->getAllJoints()[a]);
		}
	}
	for (unsigned int i = 0; i < gc.RootToken->SubTokens.size(); i++)
	{
		if (gc.RootToken->SubTokens[i]->TokenType == TokenTypes::OBJECTS)
		{
			const core::array<FBXTokenProcessor *>& Objects = gc.RootToken->SubTokens[i]->SubTokens;
			for (unsigned int j = 0; j < Objects.size(); j++)
			{
				if (Objects[j]->TokenType == TokenTypes::POSE)
				{
					FBXPose* fm = ((FBXPose*)Objects[j]);
					for (u32 k=0; k<fm->Nodes.size(); ++k)
					{
						const s32 num = msh->getJointNumber(fm->Nodes[k].Name.c_str());
						if (num != -1)
							msh->getAllJoints()[num]->LocalMatrix = fm->Nodes[k].M;
					}
				}
			}
		}
	}

	msh->finalize();
	return msh;
}

} // end namespace scene
} // end namespace irr

#endif // _IRR_COMPILE_WITH_FBX_LOADER_

