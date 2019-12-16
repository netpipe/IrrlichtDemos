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

#include "graphics/Light.h"
#include "graphics/GraphicsEngine.h"

#include <Horde3D.h>

namespace peak
{
	Light::Light(std::string name, std::string material , SceneNode *parent)
	{
		ResHandle mathandle = Horde3D::addResource(ResourceTypes::Material, material.c_str(), 0);
		node = Horde3D::addLightNode(RootNode, name.c_str(), mathandle, "LIGHTING", "SHADOWMAP" );
		Horde3D::setNodeTransform(node, 0, 0, 0, 0, 0, 0, 1, 1, 1);
		if (parent)
		{
			setParent(parent);
		}
		printf("Light created!\nLight created!\nLight created!\n");
	}
	Light::~Light()
	{
	}
	
	void Light::setRadius(float radius)
	{
		Horde3D::setNodeParamf(node, LightNodeParams::Radius, radius);	
		this->radius = radius;
	}	
	float Light::getRadius()
	{
		return radius;
	}
			
	void Light::setColor(Color color)
	{
		this->color = color;
		// Prevent divide by zero
		float currcolor = ((float)(color.getRed()) + 0.001f) / 255;
		// Color cannot be more than 255
		if (currcolor > 1.0f)
			currcolor = 1.0f;		
		Horde3D::setNodeParamf(node, LightNodeParams::Col_R, currcolor);
		// Prevent divide by zero
		currcolor = ((float)(color.getGreen()) + 0.001f) / 255;
		// Color cannot be more than 255
		if (currcolor > 1.0f)
			currcolor = 1.0f;
		Horde3D::setNodeParamf(node, LightNodeParams::Col_G, currcolor);
		// Prevent divide by zero
		currcolor = ((float)(color.getBlue()) + 0.001f) / 255;
		// Color cannot be more than 255
		if (currcolor > 1.0f)
			currcolor = 1.0f;
		Horde3D::setNodeParamf(node, LightNodeParams::Col_B, currcolor);
	}
	Color Light::getColor()
	{
		return color;
	}
	
			
	void Light::setShadowMapCount(int count)
	{
		this->count = count;
	}
	int Light::getShadowMapCount()
	{
		return count;
	}
			
	void Light::setShadowSplitLambda(float splitlambda)
	{
		this->splitlambda = splitlambda;
	}
	float Light::getShadowSplitLambda()
	{
		return splitlambda;
	}
			
	void Light::setShadowMapBias(float bias)
	{
		this->bias;
	}
	float Light::getShadowMapBias()
	{
		return bias;
	}
	// TODO: Implement the functions above		
	void Light::setFOV(float fieldofview)
	{
		fov = fieldofview;
	}
	float Light::getFOV()
	{
		return fov;
	}
	
	
	void Light::setType(LightType type)
	{
	}
	LightType Light::getType(void)
	{
	}
}

