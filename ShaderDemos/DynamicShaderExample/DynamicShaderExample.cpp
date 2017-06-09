/*	DynamicShaderExample
*	Masterhawk studios 2011 - http://masterhawk.dyndns.org/wordpress
*
*	Description:
*	This is a simple example how to use the "Dynamic Shaders for Irrlicht"
*	by Masterhawk studios in your own application. This example shows how
*	to use all provided features, especially the glass refraction, since
*	it needs your application to support RTTs
*/

//////////////////////////////////
// DON'T FORGET TO LINK 		//
//								//
// IRRLICHT.LIB					//
// IRRLICHT-INCLUDE-DIRECTORY	//
// TINYXML.LIB					//
// TINYXML-INCLUDE-DIRECTORY	//
//								//
// TO YOUR PROJECT				//
//////////////////////////////////

//stdafx inlcudes the irrlicht.h inclusion
//#include "stdafx.h"
#include <irrlicht.h>
//don't forget to include the CShader source
#include "CShader.h"


//simple Irrlicht setup stuff
using namespace irr;

IrrlichtDevice* device = 0;
video::IVideoDriver* driver = 0;
scene::ISceneManager* smgr = 0;
scene::ICameraSceneNode* cam = 0;


//Now we have to set up a simple Render-To-Texture management
//-----------------------------------------------------------
//scenenode_for_rtt_view:
//Here all scenenodes get stored which use a shader with RTT requirement.
//This is currently just the glass refraction shader
core::array<scene::ISceneNode*> scenenodes_for_rtt_view;

//tex_rtt_view:
//The texture which holds the texture render from the view position.
//There's currently no other RTT-type supported. Each type would need
//its own texture.
video::ITexture* tex_rtt_view = 0;


//Adds a scenenode to the RTT-queue if it has a RTT-requirement
//These information are all provided and handled by the DynamicShader files.
void registerSceneNodeForRTT(scene::ISceneNode* node, S_RTT_Info* rtt_info)
{
	if(node && rtt_info)
	{
		switch(rtt_info->type)
		{
		case ERT_VIEW:
		{
			if(scenenodes_for_rtt_view.size() <= 0)
				tex_rtt_view = driver->addRenderTargetTexture( core::dimension2d<u32>(256,256) );

			scenenodes_for_rtt_view.push_back(node);
			node->setMaterialTexture(rtt_info->tex_channel,tex_rtt_view);
		}
		}
	}
}

int main()
{
	//again simple Irrlicht setup/loading stuff
	device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800,600));
	if(!device) return -1;

	driver = device->getVideoDriver();
	if(!driver) return -2;

	smgr = device->getSceneManager();
	if(!smgr) return -3;

	cam = smgr->addCameraSceneNodeFPS();

	device->setWindowCaption(L"Dynamic Shader Example - Masterhawk studios 2011");


	//loading the angel model
	scene::ISceneNode* glass_angel = smgr->addMeshSceneNode(smgr->getMesh("./meshes/statue_angel.obj"));
	//setting up the environment texture for the simulated reflection.
	//In IrrShaderViewer this is achieved by the settings xml files.
	glass_angel->setMaterialTexture(0,driver->getTexture("./media/House.jpg"));

	//disable lighting. This shader doesn't need a light source
	glass_angel->setMaterialFlag(video::EMF_LIGHTING,false);

	//loading the shader-defintion and create a shader. Don't forget to delete
	//the pointer!
	CShader* glass_shader = new CShader(device,"./shaders/glassrefraction.xml");

	//assigning the shader to the scenenode
	if(glass_shader)
		glass_angel->setMaterialType((video::E_MATERIAL_TYPE)glass_shader->getMaterial());

	//register the scenenode for RTT handling. This is made for every scenenode, no
	//matter if its shader requires RTT support or not. The registerSceneNodeForRTT()
	//takes care of this
	registerSceneNodeForRTT(glass_angel,glass_shader->getRTTInfo(ERT_VIEW));

	//adding a skybox so you can see a transparency effect
	smgr->addSkyBoxSceneNode(
		driver->getTexture("./media/irrlicht2_up.jpg"),
		driver->getTexture("./media/irrlicht2_dn.jpg"),
		driver->getTexture("./media/irrlicht2_lf.jpg"),
		driver->getTexture("./media/irrlicht2_rt.jpg"),
		driver->getTexture("./media/irrlicht2_ft.jpg"),
		driver->getTexture("./media/irrlicht2_bk.jpg"));

	while(device->run())
	{
		driver->beginScene(true, true, video::SColor(255,100,100,100));

		//creating the RTT-VIEW texture for every frame
		if(scenenodes_for_rtt_view.size() > 0)
		{
			for(s32 i=0; i<scenenodes_for_rtt_view.size(); i++)
				scenenodes_for_rtt_view[i]->setVisible(false);

			driver->setRenderTarget(tex_rtt_view);
			smgr->drawAll();
			driver->setRenderTarget(0);

			for(s32 i=0; i<scenenodes_for_rtt_view.size(); i++)
				scenenodes_for_rtt_view[i]->setVisible(true);
		}

		//just the normal rendering
		smgr->drawAll();
		driver->endScene();
	}

	device->drop();

	//deleting the shader
	delete glass_shader;

	return 0;
}

