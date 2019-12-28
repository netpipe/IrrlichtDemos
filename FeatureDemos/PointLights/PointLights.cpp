#include "PointLights.h"

/* CONSTRUCTOR */
CPointLights::CPointLights()
{
	// This creates a point of light we use as a "brand" to add values to the other lights.
	light = pScene->addLightSceneNode(0,vector3df(0),SColorf(0,0,0,0),0);
	// Light radius to zero, thus saving me a pass (the ambient pass) ;)

	shader = pGPU->addShaderMaterialFromFiles("shader/bump.vp","shader/bump.fp",
											  this,EMT_ONETEXTURE_BLEND,0);
}

/* DESTRUCTOR */
CPointLights::~CPointLights()
{
}

void CPointLights::addLight(SColor color,vector3df position,f32 radius)
{
	fakeLight l;

	// Fake light configuration.
	l.color = color;
	l.pos = position;
	l.radius = radius;

	// Billboard configuration.
	l.billboard = pScene->addBillboardSceneNode(0,dimension2d<f32>(5.0f,5.0f),
												position,-1,color,color);

	l.billboard->setMaterialTexture(0,pVideo->getTexture("media/glow.dds"));
	l.billboard->setMaterialFlag(EMF_ZWRITE_ENABLE,false);
	l.billboard->setMaterialFlag(EMF_LIGHTING,false);
	l.billboard->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	l.billboard->setAutomaticCulling(scene::EAC_FRUSTUM_BOX);
	l.billboard->setScale(vector3df(radius/2.05f));

	// Add to array !!!.
	lightConf.push_back(l);
}

/* UPDATE */
void CPointLights::render()
{
	for(u32 i=0; i<lightConf.size(); ++i)
	{
		light->getLightData().DiffuseColor = lightConf[i].color;
		light->getLightData().SpecularColor = lightConf[i].color;
		light->setPosition(lightConf[i].pos);
		radius = 0.5f/lightConf[i].radius*2.0f;

		if(!pScene->isCulled(lightConf[i].billboard))
        {
			pVideo->deleteAllDynamicLights();

			// Render all light.
			light->OnAnimate(device->getTimer()->getTime());
			light->OnRegisterSceneNode();
			light->render();
			light->updateAbsolutePosition();

			// Render all meshes.
			for(u32 j=0; j<meshArray.size(); ++j)
			{
				pVideo->setTransform(ETS_WORLD,meshArray[j]->getAbsoluteTransformation());

				for(u32 k=0; k<meshArray[j]->getMaterialCount(); ++k)
				{
					SMaterial material = meshArray[j]->getMaterial(k);
					material.Shininess = 20.0f;
					material.MaterialType = (E_MATERIAL_TYPE)shader;
					material.MaterialTypeParam = pack_textureBlendFunc(EBF_ONE,EBF_ONE_MINUS_SRC_COLOR);

					pVideo->setMaterial(material);
					pVideo->drawMeshBuffer(meshArray[j]->getMesh()->getMeshBuffer(k));
				}
			}
		}
	}
}

void CPointLights::OnSetConstants(IMaterialRendererServices* services,s32 userData)
{
	if(userData == 0)
		services->setPixelShaderConstant(reinterpret_cast<const f32*>(&radius),5,1);
}
