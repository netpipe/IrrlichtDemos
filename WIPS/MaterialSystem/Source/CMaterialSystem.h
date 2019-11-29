#pragma once
#include "IMaterialSystem.h"
#include "IVideoDriver.h"
#include "irrArray.h"
#include "irrString.h"
#include "irrMap.h"
#include "irrArray.h"
#include "IAttributes.h"
#include "irrXML.h"

namespace irr
{
	using namespace core;
namespace video
{
class CMaterialSystem :	public IMaterialSystem
{
public:
	CMaterialSystem(IVideoDriver *video,io::IFileSystem *io);
	~CMaterialSystem(void);
	virtual bool addTextureLayerName(u32 mattype,u32 tex,const c8 * name);
	virtual bool loadMaterial(SMaterial &material,io::IReadFile *file);
	virtual bool loadMaterial(SMaterial &material,const c8 *filename,bool load_texture_onfail = true);

	virtual s32 getSystemParam(const c8 *param);
	virtual void setSystemParam(const c8 *param,s32 value);

	virtual void setDefaultValues(io::IAttributes *attributes);

	virtual void addNamedTexture(const c8 *name,ITexture *texture);
	virtual void removeNamedTexture(const c8 *name);

protected:
	virtual void updateStandardParams();
private:
	bool isNormalMap(ITexture * texture);
	ITexture *getNamedTexture(stringc name);
	stringc getMaterialFileName(stringc imagefile);
	bool skipElement(io::IXMLReader *Reader);
	bool loadMaterial(SMaterial &material,io::IXMLReader *Reader,bool SecondPass);
	void loadMaterialFallback(SMaterial &material,io::IXMLReader *reader);
	void loadMaterialAttribute(SMaterial &material,io::IXMLReader *reader);
	void loadMaterialDefaults(SMaterial &material,io::IXMLReader *reader);
	void loadTexture(SMaterial &material,io::IXMLReader *reader);
	void loadTextureParams(SMaterial &material,SMaterialLayer &layer,io::IXMLReader *reader);
	bool checkFlags(io::IXMLReader *Reader);

	void setupDefaultMaterials();
	void setDefaultParams();
	bool DefaultParamsCreated;
	io::IAttributes *Defaults;
	IVideoDriver *VideoDriver;
	io::IFileSystem *FileSystem;
	struct STextureLayer
	{
		u32 MatType;
		stringc TextureName;
		STextureLayer() : MatType(0)
		{}
		STextureLayer(u32 id,stringc name) : MatType(id),TextureName(name)
		{}
		STextureLayer(const STextureLayer &cpy) : MatType(cpy.MatType),TextureName(cpy.TextureName)
		{}
		const STextureLayer &operator=(const STextureLayer &RHS)
		{
			MatType = RHS.MatType;
			TextureName = RHS.TextureName;
			return *this;
		}
		bool operator==(const STextureLayer &RHS) const
		{
			return MatType == RHS.MatType && TextureName == RHS.TextureName;
		}
		bool operator<(const STextureLayer &RHS) const
		{
			if(MatType == RHS.MatType)
				return TextureName < RHS.TextureName;
			else
				return MatType < RHS.MatType;
		}
	};
	typedef map<STextureLayer,u32> TLayerMap;
	TLayerMap TextureLayers;
	typedef map<stringc,s32> TParamsMap;
	TParamsMap SysParams;

	typedef map<stringc,ITexture*> TTextureMap;
	TTextureMap NamedTextures;

};
}
}