#ifndef __CORE_SHADER_CALLBACK_H_INCLUDED__
#define __CORE_SHADER_CALLBACK_H_INCLUDED__

#include "config.h"

#include <irrlicht.h>

#include <string>


class CoreShaderCallback : public irr::video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetMaterial(const irr::video::SMaterial& material)
	{
		this->material = &material;
	}

	virtual void OnSetConstants(irr::video::IMaterialRendererServices * services, irr::s32 userData)
	{
		irr::video::IVideoDriver * driver = services->getVideoDriver();
		irr::scene::ISceneManager * smgr = this->device->getSceneManager();

		services->setVertexShaderConstant("model", driver->getTransform(irr::video::ETS_WORLD).pointer(), 16);
		services->setVertexShaderConstant("view", driver->getTransform(irr::video::ETS_VIEW).pointer(), 16);
		services->setVertexShaderConstant("projection", driver->getTransform(irr::video::ETS_PROJECTION).pointer(), 16);

		irr::core::CMatrix4<irr::f32> normMat;
		driver->getTransform(irr::video::ETS_WORLD).getInverse(normMat);
		normMat.getTransposed(normMat);
		/*irr::f32 mpointer[9];
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				mpointer[3*i + j] = normMat[4*i + j];
			}
		}*/
		services->setPixelShaderConstant("normMat", normMat.pointer(), 16);

		irr::core::vector3df camPos = smgr->getActiveCamera()->getAbsolutePosition();
		services->setPixelShaderConstant("camPos", reinterpret_cast<irr::f32*>(&camPos), 3);

		irr::video::SColorf ambience = smgr->getAmbientLight();
		services->setPixelShaderConstant("ambience", reinterpret_cast<irr::f32*>(&ambience), 3);

		irr::video::SColorf shadowColor = smgr->getShadowColor();
		services->setPixelShaderConstant("shadowColor", reinterpret_cast<irr::f32*>(&shadowColor), 3);

		irr::video::SColorf ambient(this->material->AmbientColor);
		irr::video::SColorf diffuse(this->material->DiffuseColor);
		irr::video::SColorf specular(this->material->SpecularColor);
		services->setPixelShaderConstant("material.ambient", reinterpret_cast<irr::f32*>(&ambient), 3);
		services->setPixelShaderConstant("material.diffuse", reinterpret_cast<irr::f32*>(&diffuse), 3);
		services->setPixelShaderConstant("material.specular", reinterpret_cast<irr::f32*>(&specular), 3);
		bool lighting = this->material->Lighting;
		services->setPixelShaderConstant("material.lighting", &lighting, 1);
		irr::f32 shininess = this->material->Shininess;
		services->setPixelShaderConstant("material.shininess", &shininess, 1);

		irr::core::array<irr::scene::ISceneNode*> lightNodes;
		smgr->getSceneNodesFromType(irr::scene::ESNT_LIGHT, lightNodes);

		int plights = 0;
		int dlights = 0;
		int slights = 0;

		for(int i = 0; i < lightNodes.size(); i++)
		{
			irr::video::SLight light = ((irr::scene::ILightSceneNode*)lightNodes[i])->getLightData();
			std::string pnum = std::to_string(plights);
			std::string dnum = std::to_string(dlights);

			switch(light.Type) {
			case irr::video::ELT_POINT:
				if(plights < SHADER_MAX_POINT_LIGHTS) {
					services->setPixelShaderConstant(("plights[" + pnum + "].ambient").c_str(), reinterpret_cast<irr::f32*>(&light.AmbientColor), 3);
					services->setPixelShaderConstant(("plights[" + pnum + "].diffuse").c_str(), reinterpret_cast<irr::f32*>(&light.DiffuseColor), 3);
					services->setPixelShaderConstant(("plights[" + pnum + "].specular").c_str(), reinterpret_cast<irr::f32*>(&light.SpecularColor), 3);
					services->setPixelShaderConstant(("plights[" + pnum + "].radius").c_str(), reinterpret_cast<irr::f32*>(&light.Radius), 1);
					irr::core::vector3df pos = ((irr::scene::ILightSceneNode*)lightNodes[i])->getAbsolutePosition();
					services->setPixelShaderConstant(("plights[" + pnum + "].position").c_str(), reinterpret_cast<irr::f32*>(&pos), 3);
					services->setPixelShaderConstant(("plights[" + pnum + "].attenuation").c_str(), reinterpret_cast<irr::f32*>(&light.Attenuation), 3);
					plights++;
				}
				break;
			case irr::video::ELT_SPOT:
				break;
			case irr::video::ELT_DIRECTIONAL:
				if(dlights < SHADER_MAX_DIR_LIGHTS) {
					services->setPixelShaderConstant(("dlights[" + dnum + "].ambient").c_str(), reinterpret_cast<irr::f32*>(&light.AmbientColor), 3);
					services->setPixelShaderConstant(("dlights[" + dnum + "].diffuse").c_str(), reinterpret_cast<irr::f32*>(&light.DiffuseColor), 3);
					services->setPixelShaderConstant(("dlights[" + dnum + "].specular").c_str(), reinterpret_cast<irr::f32*>(&light.SpecularColor), 3);
					services->setPixelShaderConstant(("dlights[" + dnum + "].direction").c_str(), reinterpret_cast<irr::f32*>(&light.Direction), 3);
					dlights++;
				}
				break;
			default:
				break;
			}
		}

		services->setPixelShaderConstant("numplights", &plights, 1);
		services->setPixelShaderConstant("numdlights", &dlights, 1);
		services->setPixelShaderConstant("numslights", &slights, 1);
	}

	CoreShaderCallback(irr::IrrlichtDevice * dev) : device(dev) {
		this->device->grab();
	}

	~CoreShaderCallback() { this->device->drop(); }

private:

	const irr::video::SMaterial * material;

	irr::IrrlichtDevice * device;
};


#endif
