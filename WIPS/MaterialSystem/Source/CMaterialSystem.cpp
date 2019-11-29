#include "CMaterialSystem.h"
#include "IReadFile.h"
#include "coreutil.h"
#include "os.h"
#include "IMaterialRenderer.h"
#include "CAttributeImpl.h"
namespace irr
{
using namespace io;
using namespace core;
namespace video
{

CMaterialSystem::CMaterialSystem(IVideoDriver *video,io::IFileSystem *io)
{
	VideoDriver = video;
	FileSystem = io;
	Defaults = NULL;
	setupDefaultMaterials();
	DefaultParamsCreated = false;
	//setDefaultParams();
}

CMaterialSystem::~CMaterialSystem(void)
{
	if(Defaults)
		Defaults->drop();
	/*TTextureMap::ParentLastIterator it = NamedTextures.getParentLastIterator();
	while(!it.atEnd())
	{
		it->getValue()->drop();
	}*/
}
bool CMaterialSystem::addTextureLayerName(u32 mattype,u32 tex,const c8 *name)
{
	if(isdigit(name[0]))
		return false;
	for(c8 *ch = (c8 *)name;*ch != 0;ch++)
		if(isspace(*ch))
			return false;
	if(tex > MATERIAL_MAX_TEXTURES)
		return false;
	TextureLayers.set(STextureLayer(mattype,name),tex);
	return true;
}
bool CMaterialSystem::loadMaterial(SMaterial &material,io::IReadFile *file)
{
	bool ret = false;
	os::Printer::log("Loading Material",file->getFileName());
	IXMLReader *reader = FileSystem->createXMLReader(file);
	ret = loadMaterial(material,reader,false);
	reader->drop();
	if(!ret)
	{
		file->seek(0);
		reader = FileSystem->createXMLReader(file);
		ret = loadMaterial(material,reader,true);
		reader->drop();
	}

	if(ret)
		os::Printer::log("Finished Material",file->getFileName());
	else
		os::Printer::log("Material Load Failed",file->getFileName(),ELL_WARNING);
	
	return ret;
}
bool CMaterialSystem::loadMaterial(SMaterial &material,const c8 *filename,bool load_texture_onfail)
{
	stringc matfilename = getMaterialFileName(filename);
	bool ret = FileSystem->existFile(matfilename);
	if(ret)
	{
		IReadFile *file = FileSystem->createAndOpenFile(matfilename.c_str());
		ret = loadMaterial(material,file);
		file->drop();
	}
	if(!ret && load_texture_onfail)
	{
		material = SMaterial(); //resets all default values;
		material.setTexture(0,VideoDriver->getTexture(filename));
	}
	return ret;
}

s32 CMaterialSystem::getSystemParam(const c8 *param)
{
	if(!DefaultParamsCreated)
		setDefaultParams();
	TParamsMap::Node *node = SysParams.find(param);
	if(!node)
		return 0;
	else
		return node->getValue();
}
void CMaterialSystem::setSystemParam(const c8 *param,s32 value)
{
	if(!DefaultParamsCreated)
		setDefaultParams();

	if(value == 0)
		SysParams.remove(param);
	else
		SysParams.set(param,value);
}
void CMaterialSystem::setDefaultValues(io::IAttributes *attributes)
{
	if(Defaults)
		Defaults->drop();
	Defaults = IMaterialSystem::copyAttributes(attributes);
}

stringc CMaterialSystem::getMaterialFileName(stringc imagefile)
{
	stringc ret = FileSystem->getFileBasename(imagefile,false);

	return FileSystem->getFileDir(imagefile) + "/" + ret + ".irrmaterial";

}
bool CMaterialSystem::skipElement(io::IXMLReader *Reader)
{
	if(Reader->getNodeType() != EXN_ELEMENT)
		return false;
	if(Reader->isEmptyElement())
		return true;
	stringw name = Reader->getNodeName();
	while(Reader->read())
	{
		if(Reader->getNodeType() == EXN_ELEMENT_END && name == Reader->getNodeName())
			return true;
	}
	return false;
}
bool CMaterialSystem::loadMaterial(SMaterial &material,io::IXMLReader *Reader,bool SecondPass)
{
	while(Reader->getNodeType() != EXN_ELEMENT)
	{
		if(!Reader->read())
			return false;
	}
	if(stringw(Reader->getNodeName()) != L"material")
		return false;
	while(Reader->read())
	{
		if(Reader->getNodeType() == EXN_ELEMENT)
		{
			stringc mattypename = Reader->getNodeName();
			s32 mattype = VideoDriver->findMaterialRendererId(mattypename.c_str());
			if(mattype == -1)
			{
				os::Printer::log("Invalid material type.",mattypename.c_str(),ELL_ERROR);
				skipElement(Reader);
				continue;
			}
			if(!checkFlags(Reader))
			{
				skipElement(Reader);
				continue;
			}
			IMaterialRenderer *renderer = VideoDriver->getMaterialRenderer(mattype);
			if(!SecondPass && renderer->getRenderCapability() != 0)
			{
				skipElement(Reader);
				continue;
			}
			material = SMaterial();
			material.MaterialType = (E_MATERIAL_TYPE)(u32)mattype;
			loadMaterialDefaults(material,Reader);
			loadMaterialFallback(material,Reader);
			return true;
		}
	}
	return false;
}
void CMaterialSystem::loadMaterialFallback(SMaterial &material,io::IXMLReader *reader)
{
	stringw fallback = reader->getNodeName();
	while(reader->read())
	{
		switch(reader->getNodeType())
		{
			case EXN_ELEMENT_END:
				if(fallback == reader->getNodeName())
					return;
				break;
			case EXN_ELEMENT:
				loadMaterialAttribute(material,reader);
				if(skipElement(reader) && !reader->isEmptyElement())
					os::Printer::log("Material System: Sub-attributes not allowed. skipping.",ELL_WARNING);
				break;
		}
	}
}
void CMaterialSystem::loadMaterialAttribute(SMaterial &material,io::IXMLReader *reader)
{
	stringc element = reader->getNodeName();
	stringc name = reader->getAttributeValue(L"name");
	stringc value = reader->getAttributeValue(L"value");
	if(element == "float")
	{
		material.setValue(name.c_str(),(f32)atof(value.c_str()));
	}
	else if(element == "bool")
	{
		if(isdigit(value[0]))
			material.setValue(name.c_str(),(bool)(atoi(value.c_str()) != 0));
		else
			material.setValue(name.c_str(),(bool)(value == "true"));
		
	}
	else if(element == "int")
	{
		material.setValue(name.c_str(),atoi(value.c_str()));
	}
	else if(element == "color")
	{
		CColorAttribute attr(name.c_str(),SColorf());
		attr.setString(value.c_str());
		material.setValue(name.c_str(),attr.getColor());
	}
	else if(element == "colorf")
	{
		CColorfAttribute attr(name.c_str(),SColorf());
		attr.setString(value.c_str());
		material.setValue(name.c_str(),attr.getColor());
	}
	else if(element == "vector3d")
	{
		CVector3DAttribute attr(name.c_str(),vector3df());
		attr.setString(value.c_str());
		material.setValue(name.c_str(),attr.getColor());
	}
	else if(element == "position")
	{
		CPosition2DAttribute attr(name.c_str(),position2di());
		attr.setString(value.c_str());
		material.setValue(name.c_str(),attr.getPosition());
	}
	else if(element == "matrix")
	{
		CMatrixAttribute attr(name.c_str(),matrix4());
		attr.setString(value.c_str());
		material.setValue(name.c_str(),attr.getMatrix());
	}
	else if(element == "texture" ||element == "render_target")
	{
		loadTexture(material,reader);
	}
	else
	{
		stringc err = (stringc)"Attribute type '" + element + "' not supported. Ignoring.";
		os::Printer::log(err.c_str(),ELL_WARNING);
	}
}
void CMaterialSystem::loadMaterialDefaults(SMaterial &material,io::IXMLReader *reader)
{
	if(Defaults == NULL)
		return;
	for(u32 i = 0;i < Defaults->getAttributeCount();i++)
	{
		stringc name = Defaults->getAttributeName(i);
		switch(Defaults->getAttributeType(i))
		{
			case EAT_FLOAT:
				material.setValue(name.c_str(),Defaults->getAttributeAsFloat(i));
				break;
			case EAT_BOOL:
				material.setValue(name.c_str(),Defaults->getAttributeAsBool(i));
				break;
			case EAT_INT:
				material.setValue(name.c_str(),Defaults->getAttributeAsInt(i));
				break;
			case EAT_COLOR:
			case EAT_COLORF:
				material.setValue(name.c_str(),Defaults->getAttributeAsColor(i));
				break;
			case EAT_VECTOR3D:
				material.setValue(name.c_str(),Defaults->getAttributeAsVector3d(i));
				break;
			case EAT_POSITION2D:
				material.setValue(name.c_str(),Defaults->getAttributeAsPosition2d(i));
				break;
			case EAT_MATRIX:
				material.setValue(name.c_str(),Defaults->getAttributeAsMatrix(i));
				break;
		}
	}
}
void CMaterialSystem::loadTexture(SMaterial &material,io::IXMLReader *reader)
{
	u32 index = 0;
	STextureLayer key(material.MaterialType,reader->getAttributeValue(L"layer"));
	if(isdigit(key.TextureName[0]))
		index = clamp((u32)atoi(key.TextureName.c_str()),(u32)0,(u32)MATERIAL_MAX_TEXTURES);
	else
	{
		TLayerMap::Node *node = TextureLayers.find(key);
		if(node == NULL)
		{
			stringc err = (stringc)"No layer named '" + key.TextureName + "'.";
			os::Printer::log(err.c_str(),ELL_ERROR);
			skipElement(reader);
			return;
		}
		index = node->getValue();
	}
	SMaterialLayer &layer = material.TextureLayer[index];
	bool mipmaps = VideoDriver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	bool noalpha = VideoDriver->getTextureCreationFlag(ETCF_NO_ALPHA_CHANNEL);
	if(stringw(reader->getAttributeValue(L"nomipmaps")) == L"true")
		VideoDriver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS,false);
	if(stringw(reader->getAttributeValue(L"noalpha")) == L"true")
		VideoDriver->setTextureCreationFlag(ETCF_NO_ALPHA_CHANNEL,false);

	if(stringw(reader->getNodeName()) == L"render_target")
	{
		dimension2di rtd(reader->getAttributeValueAsInt(L"width"),reader->getAttributeValueAsInt(L"height"));
		if(rtd.Width < 16)
			rtd.Width = 256;
		if(rtd.Height < 16)
			rtd.Height = 256;
		dimension2di ortd = rtd.getOptimalSize();
		if(ortd != rtd)
		{
			stringc err = "Upsizing render texture to (";
			err += ortd.Width;
			err	+= ",";
			err	+= ortd.Height;
			err	+=  ")."; 
			os::Printer::log(err.c_str());
		}
		layer.Texture = VideoDriver->addRenderTargetTexture(ortd);
		if(layer.Texture == NULL)
		{
			stringc err = "Unable to create render target texture!"; 
			os::Printer::log(err.c_str(),ELL_ERROR);
		}
	}
	else
	{
		stringc file = reader->getAttributeValue(L"file");
		if(file[0] != '$')
			layer.Texture = VideoDriver->getTexture(file.c_str());
		else
			layer.Texture = getNamedTexture(file.subString(1,file.size()-1));
		
		if(layer.Texture == NULL)
		{
			stringc err = (stringc)"Unable to open texture '" + file + "'!"; 
			os::Printer::log(err.c_str(),ELL_ERROR);
		}
		else if(stringc(reader->getAttributeValue(L"create_normal_map")) == "true" && !isNormalMap(layer.Texture))
		{
			float amp = reader->getAttributeValueAsFloat(L"amplitude");
			if(amp == 0.0f)
				amp = 1;
			VideoDriver->makeNormalMapTexture(layer.Texture,amp);
		}
	}
	VideoDriver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS,mipmaps);
	VideoDriver->setTextureCreationFlag(ETCF_NO_ALPHA_CHANNEL,noalpha);
	if(!reader->isEmptyElement())
		loadTextureParams(material,layer,reader);

}
void CMaterialSystem::loadTextureParams(SMaterial &material,SMaterialLayer &layer,IXMLReader *reader)
{
	IAttributes *attr = new CAttributes(NULL);
	attr->read(reader,true,reader->getNodeName());
	for(u32 i =0;i < attr->getAttributeCount();i++)
	{
		switch(attr->getAttributeType(i))
		{
			case EAT_VECTOR3D:
			{
				matrix4 trans = layer.getTextureMatrix();
				vector3df vec = attr->getAttributeAsVector3d(i);
				if(!_stricmp("translation",attr->getAttributeName(i)))
					trans.setTranslation(vec);
				else if(!_stricmp("rotation",attr->getAttributeName(i)))
					trans.setRotationDegrees(vec);
				else if(!_stricmp("scale",attr->getAttributeName(i)))
					trans.setScale(vec);
				else
					goto UnknownProperty;

				layer.setTextureMatrix(trans);
				break;
			}
			case EAT_BOOL:
			{
				bool val = attr->getAttributeAsBool(i);
				if(!_stricmp("BilinearFilter",attr->getAttributeName(i)))
					layer.BilinearFilter = val;
				else if(!_stricmp("AnisotropicFilter",attr->getAttributeName(i)))
					layer.AnisotropicFilter = val;
				else if(!_stricmp("TrilinearFilter",attr->getAttributeName(i)))
					layer.TrilinearFilter = val;
				else
					goto UnknownProperty;
				break;
			}
			case EAT_INT:
			{
				s32 val = attr->getAttributeAsInt(i);
				if(!_stricmp("TextureWrap",attr->getAttributeName(i)))
					layer.TextureWrap = (E_TEXTURE_CLAMP)clamp(val,(s32)ETC_REPEAT,(s32)ETC_MIRROR);
				else
					goto UnknownProperty;
				break;
			}
			default:
				goto UnknownProperty;
		}
		continue;
UnknownProperty: //common error reporting, only acceptible use of goto's in this day and age.
		{
			stringc err = (stringc)"Unknown texture property '" + attr->getAttributeName(i) + "'.";
			os::Printer::log(err.c_str(),ELL_ERROR);
		}
	}

	attr->drop();
}
bool CMaterialSystem::checkFlags(IXMLReader *reader)
{
	for(u32 i = 0; i < reader->getAttributeCount();i++)
	{
		stringc name = reader->getAttributeName(i);
		stringc value = reader->getAttributeValue(i);
		c8 *sptr = (c8*)value.c_str();
		bool val = false;
		if( sptr[0] == '<')
		{
			sptr++;
			if(sptr[0] != '=')
				val = getSystemParam(name.c_str()) < atoi(sptr);
			else
			{
				sptr++;
				val = getSystemParam(name.c_str()) <= atoi(sptr);
			}
		}
		else if( sptr[0] == '>')
		{
			sptr++;
			if(sptr[0] != '=')
				val = getSystemParam(name.c_str()) > atoi(sptr);
			else
			{
				sptr++;
				val = getSystemParam(name.c_str()) >= atoi(sptr);
			}
		}
		else if( sptr[0] == '!')
		{
			sptr++;
			val = getSystemParam(name.c_str()) != atoi(sptr);
		}
		else
		{
			val = getSystemParam(name.c_str()) == atoi(sptr);
		}
		if(!val)
			return false;
	}
	return true;
}
void CMaterialSystem::setupDefaultMaterials()
{
	addTextureLayerName(EMT_SOLID,0,"base");
	
	addTextureLayerName(EMT_SOLID_2_LAYER,0,"base");
	addTextureLayerName(EMT_SOLID_2_LAYER,1,"overlay");

	addTextureLayerName(EMT_LIGHTMAP,0,"base");
	addTextureLayerName(EMT_LIGHTMAP,1,"lightmap");
	
	addTextureLayerName(EMT_LIGHTMAP_ADD,0,"base");
	addTextureLayerName(EMT_LIGHTMAP_ADD,1,"lightmap");
	
	addTextureLayerName(EMT_LIGHTMAP_M2,0,"base");
	addTextureLayerName(EMT_LIGHTMAP_M2,1,"lightmap");
	
	addTextureLayerName(EMT_LIGHTMAP_M4,0,"base");
	addTextureLayerName(EMT_LIGHTMAP_M4,1,"lightmap");

	addTextureLayerName(EMT_LIGHTMAP_LIGHTING,0,"base");
	addTextureLayerName(EMT_LIGHTMAP_LIGHTING,1,"lightmap");

	addTextureLayerName(EMT_LIGHTMAP_LIGHTING_M2,0,"base");
	addTextureLayerName(EMT_LIGHTMAP_LIGHTING_M2,1,"lightmap");

	addTextureLayerName(EMT_LIGHTMAP_LIGHTING_M4,0,"base");
	addTextureLayerName(EMT_LIGHTMAP_LIGHTING_M4,1,"lightmap");

	addTextureLayerName(EMT_DETAIL_MAP,0,"base");
	addTextureLayerName(EMT_DETAIL_MAP,1,"detail");

	addTextureLayerName(EMT_SPHERE_MAP,0,"spheremap");

	addTextureLayerName(EMT_REFLECTION_2_LAYER,0,"reflection");
	addTextureLayerName(EMT_REFLECTION_2_LAYER,1,"base");

	addTextureLayerName(EMT_TRANSPARENT_REFLECTION_2_LAYER,0,"reflection");
	addTextureLayerName(EMT_TRANSPARENT_REFLECTION_2_LAYER,1,"base");

	addTextureLayerName(EMT_TRANSPARENT_ADD_COLOR,0,"base");

	addTextureLayerName(EMT_TRANSPARENT_ALPHA_CHANNEL,0,"base");

	addTextureLayerName(EMT_TRANSPARENT_ALPHA_CHANNEL_REF,0,"base");

	addTextureLayerName(EMT_TRANSPARENT_VERTEX_ALPHA,0,"base");

	addTextureLayerName(EMT_NORMAL_MAP_SOLID,0,"base");
	addTextureLayerName(EMT_NORMAL_MAP_SOLID,1,"normalmap");
	
	addTextureLayerName(EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR,0,"base");
	addTextureLayerName(EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR,1,"normalmap");

	addTextureLayerName(EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA,0,"base");
	addTextureLayerName(EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA,1,"normalmap");

	addTextureLayerName(EMT_PARALLAX_MAP_SOLID,0,"base");
	addTextureLayerName(EMT_PARALLAX_MAP_SOLID,1,"parallaxmap");
	
	addTextureLayerName(EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR,0,"base");
	addTextureLayerName(EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR,1,"parallaxmap");

	addTextureLayerName(EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA,0,"base");
	addTextureLayerName(EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA,1,"parallaxmap");
		
}
void CMaterialSystem::updateStandardParams()
{
	setSystemParam("feature_render_to_target",VideoDriver->queryFeature(EVDF_RENDER_TO_TARGET));
	setSystemParam("feature_hw_tl",VideoDriver->queryFeature(EVDF_HARDWARE_TL));
	setSystemParam("feature_multitexture",VideoDriver->queryFeature(EVDF_MULTITEXTURE));
	setSystemParam("feature_bilinear_filter",VideoDriver->queryFeature(EVDF_BILINEAR_FILTER));
	setSystemParam("feature_mipmaps",VideoDriver->queryFeature(EVDF_MIP_MAP));
	
	setSystemParam("feature_vs_1_1",VideoDriver->queryFeature(EVDF_VERTEX_SHADER_1_1));
	setSystemParam("feature_vs_2_0",VideoDriver->queryFeature(EVDF_VERTEX_SHADER_2_0));
	setSystemParam("feature_vs_3_0",VideoDriver->queryFeature(EVDF_VERTEX_SHADER_3_0));

	setSystemParam("feature_ps_1_1",VideoDriver->queryFeature(EVDF_PIXEL_SHADER_1_1));
	setSystemParam("feature_ps_1_2",VideoDriver->queryFeature(EVDF_PIXEL_SHADER_1_2));
	setSystemParam("feature_ps_1_3",VideoDriver->queryFeature(EVDF_PIXEL_SHADER_1_3));
	setSystemParam("feature_ps_1_4",VideoDriver->queryFeature(EVDF_PIXEL_SHADER_1_4));
	setSystemParam("feature_ps_2_0",VideoDriver->queryFeature(EVDF_PIXEL_SHADER_2_0));
	setSystemParam("feature_ps_3_0",VideoDriver->queryFeature(EVDF_PIXEL_SHADER_3_0));

	setSystemParam("feature_arb_vp_1_0",VideoDriver->queryFeature(EVDF_ARB_VERTEX_PROGRAM_1));
	setSystemParam("feature_arb_fp_1_0",VideoDriver->queryFeature(EVDF_ARB_FRAGMENT_PROGRAM_1));

	setSystemParam("feature_glsl",VideoDriver->queryFeature(EVDF_ARB_GLSL));
	setSystemParam("feature_hlsl",VideoDriver->queryFeature(EVDF_HLSL));

	setSystemParam("feature_texture_nsquare",VideoDriver->queryFeature(EVDF_TEXTURE_NSQUARE));
	setSystemParam("feature_texture_npot",VideoDriver->queryFeature(EVDF_TEXTURE_NPOT));
}
void CMaterialSystem::setDefaultParams()
{
	DefaultParamsCreated = true;

	switch(VideoDriver->getDriverType())
	{
		case EDT_BURNINGSVIDEO:
			setSystemParam("renderer_burningsvideo",1);
			break;
		case EDT_SOFTWARE:
			setSystemParam("renderer_software",1);
			break;
		case EDT_DIRECT3D8:
			setSystemParam("renderer_d3d8",1);
			setSystemParam("renderer_d3d",1);
			break;
		case EDT_DIRECT3D9:
			setSystemParam("renderer_d3d9",1);
			setSystemParam("renderer_d3d",1);
			break;
		case EDT_OPENGL:
			setSystemParam("renderer_opengl",1);
			break;
	}
	updateStandardParams();
}


static const c8 *sFloatValues[] = 
{
	"Shininess",
	"MaterialTypeParam",
	"MaterialTypeParam2",
	"Thickness",
	NULL
};
static const c8 *sBoolValues[] = 
{
	"Wireframe",
	"PointCloud",
	"GouraudShading",
	"Lighting",
	"ZWriteEnable",
	"BackfaceCulling",
	"FrontfaceCulling",
	"BilinearFilter",
	"TrilinearFilter",
	"AnisotropicFilter",
	"FogEnable",
	"Lighting",
	"NormalizeNormals",
	NULL
};
static const c8 *sColorValues[] = 
{
	"AmbientColor",
	"DiffuseColor",
	"EmissiveColor",
	"SpecularColor",
	NULL
};
static const c8 *sIntValues[] = 
{
	"ZBuffer",
	"TextureWrap",
	NULL
};
E_ATTRIBUTE_TYPE IMaterialSystem::isBuiltinValue(const c8 *valuename,E_ATTRIBUTE_TYPE ignore)
{
	s32 i;
	if(ignore != EAT_FLOAT)
	{
		for(i = 0;sFloatValues[i] != NULL;i++)
		{
			if(!_stricmp(valuename,sFloatValues[i]))
				return EAT_FLOAT;
		}
	}
	if(ignore != EAT_BOOL)
	{
		for(i = 0;sBoolValues[i] != NULL;i++)
		{
			if(!_stricmp(valuename,sBoolValues[i]))
				return EAT_BOOL;
		}
	}
	if(ignore != EAT_COLOR)
	{
		for(i = 0;sColorValues[i] != NULL;i++)
		{
			if(!_stricmp(valuename,sColorValues[i]))
				return EAT_COLOR;		}
	}
	if(ignore != EAT_INT)
	{
		for(i = 0;sIntValues[i] != NULL;i++)
		{
			if(!_stricmp(valuename,sIntValues[i]))
				return EAT_INT;
		}
	}
	return EAT_UNKNOWN;
}

io::IAttributes * IMaterialSystem::createMaterialAttributes()
{
	return new CAttributes();
}

io::IAttributes *IMaterialSystem::copyAttributes(io::IAttributes *src,bool copy_builtin)
{
	if(src == NULL)
		return src;
	IAttributes *ret = createMaterialAttributes();
	
	for(u32 i = 0;i < src->getAttributeCount();i++)
	{
		stringc name = src->getAttributeName(i);
		if(!copy_builtin && isBuiltinValue(name.c_str()) != EAT_UNKNOWN)
			continue;
		switch(src->getAttributeType(i))
		{
			case EAT_FLOAT:
				ret->addFloat(name.c_str(),src->getAttributeAsFloat(i));
				break;
			case EAT_BOOL:
				ret->addBool(name.c_str(),src->getAttributeAsBool(i));
				break;
			case EAT_INT:
				ret->addInt(name.c_str(),src->getAttributeAsInt(i));
				break;
			case EAT_COLOR:
			case EAT_COLORF:
				ret->addColor(name.c_str(),src->getAttributeAsColor(i));
				break;
			case EAT_VECTOR3D:
				ret->addVector3d(name.c_str(),src->getAttributeAsVector3d(i));
				break;
			case EAT_POSITION2D:
				ret->addPosition2d(name.c_str(),src->getAttributeAsPosition2d(i));
				break;
			case EAT_MATRIX:
				ret->addMatrix(name.c_str(),src->getAttributeAsMatrix(i));
				break;
		}
	}
	return ret;
}

void CMaterialSystem::addNamedTexture(const c8 *name, ITexture *texture)
{
	/*TTextureMap::Node *node =  NamedTextures.find(name);
	if(node)
	{
		node->getValue()->drop();
	}
	texture->grab();*/
	NamedTextures.set(name,texture);
}
void CMaterialSystem::removeNamedTexture(const c8 *name)
{
	//TTextureMap::Node *node =  NamedTextures.find(name);
	//if(node)
	//{
		//node->getValue()->drop();
		NamedTextures.remove(name);
	//}
}
ITexture *CMaterialSystem::getNamedTexture(stringc name)
{
	TTextureMap::Node *node =  NamedTextures.find(name);
	if(!node)
		return NULL;
	else
		return node->getValue();
}

bool CMaterialSystem::isNormalMap(ITexture *texture)
{
	return texture->isNormalMap();
}

}
}