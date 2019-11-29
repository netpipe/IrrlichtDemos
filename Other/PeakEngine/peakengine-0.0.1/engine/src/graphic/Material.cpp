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

#include "graphic/Material.h"
#include "core/Util.h"
#include "core/IniFile.h"
#include "core/Logger.h"

namespace peak
{
	Material::Material()
	{
		type = EMT_Solid;
		texcount = 0;
		wireframe = false;
		lighting = true;
	}
	Material::~Material()
	{
	}
	
	bool Material::load(std::string name)
	{
		IniFile file;
		if (!file.load(std::string("materials/") + name + ".mat"))
		{
			LERROR("Could not load entity file \"%s\"\n", (std::string("entities/") + name + "/entity.ini").c_str());
			return false;
		}
		std::string typestring = file.getItemValue("material", "type");
		if (typestring == "solid")
		{
			type = EMT_Solid;
		}
		else if (typestring == "add")
		{
			type = EMT_Add;
		}
		else if (typestring == "parallax")
		{
			type = EMT_ParallaxMap;
		}
		else if (typestring == "normal")
		{
			type = EMT_NormalMap;
		}
		else if (typestring == "detail")
		{
			type = EMT_DetailMap;
		}
		else
		{
			type = EMT_Solid;
			LERROR("Unknown material type (%s).\n", typestring.c_str());
		}
		setTextureCount(file.getItemValueI("material", "texturecount"));
		for (int i = 0; i < texcount; i++)
		{
			textures[i] = file.getItemValue("material", std::string("texture") + toString(i));
			if (file.getItemIndex("material", std::string("scale") + toString(i)) != -1)
			{
				texturescale[i] = file.getItemValue2D("material", std::string("scale") + toString(i));
			}
		}
		if (file.getItemIndex("material", "lighting") != -1)
		{
			lighting = file.getItemValueB("material", "lighting");
		}
		else
		{
			lighting = true;
		}
		if (file.getItemIndex("material", "backfaces") != -1)
		{
			backfaces = file.getItemValueB("material", "backfaces");
		}
		else
		{
			backfaces = false;
		}
		return true;
	}
	
	void Material::setType(MaterialType type)
	{
		this->type = type;
	}
	MaterialType Material::getType(void)
	{
		return type;
	}
	
	void Material::setTextureCount(int count)
	{
		int oldcount = texcount;
		texcount = count;
		textures.resize(texcount);
		texturescale.resize(texcount);
		for (int i = oldcount; i < texcount; i++)
		{
			texturescale[i] = Vector2D(1, 1);
		}
	}
	int Material::getTextureCount(void)
	{
		return texcount;
	}
	void Material::setTexture(int index, std::string texture)
	{
		textures[index] = texture;
	}
	std::string Material::getTexture(int index)
	{
		return textures[index];
	}
	void Material::setTextureScale(int index, Vector2D scale)
	{
		texturescale[index] = scale;
	}
	Vector2D Material::getTextureScale(int index)
	{
		return texturescale[index];
	}
	
	void Material::setWireframe(bool wireframe)
	{
		this->wireframe = wireframe;
	}
	bool Material::isWireframe(void)
	{
		return wireframe;
	}
	
	void Material::setLighting(bool lighting)
	{
		this->lighting = lighting;
	}
	bool Material::getLighting(void)
	{
		return lighting;
	}
	
	void Material::setBackfaceCulling(bool culling)
	{
		backfaces = !culling;
	}
	bool Material::getBackfaceCulling(void)
	{
		return !backfaces;
	}
}

