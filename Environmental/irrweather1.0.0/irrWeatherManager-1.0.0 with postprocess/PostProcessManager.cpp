#include "PostProcessManager.h"
#include "IPostProcess.h"
#include "ShaderPostProcess.h"

CPostProcessManager::CPostProcessManager(IrrlichtDevice* device)
 : Device(device)
{
	// store driver flags
	bool flagMipMaps = Device->getVideoDriver()->getTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS);
	bool flag32Bit = Device->getVideoDriver()->getTextureCreationFlag(video::ETCF_ALWAYS_32_BIT);

	// set new flags for rtt creation
	Device->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS);
	Device->getVideoDriver()->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT);

	// load the rtt and effect configuration
	loadRTTConfig("../../Data/config/rtt.xml");
	loadEffectConfig("../../Data/config/effect.xml");

	// prepare the depth material for depth pass
	DepthMaterial = new CShaderMaterial(Device, "Depth", "material/depth.vsh", "main", video::EVST_VS_1_1, "material/depth.psh", "main", video::EPST_PS_2_0, video::EMT_SOLID);
	DepthMaterial->setVertexShaderFlag(ESC_WORLD);
	DepthMaterial->setVertexShaderFlag(ESC_VIEW);
	DepthMaterial->setVertexShaderFlag(ESC_PROJECTION);

	// restore driver flags
	Device->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, flagMipMaps);
	Device->getVideoDriver()->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, flag32Bit);
}

CPostProcessManager::~CPostProcessManager()
{
	// drop all post process objects
	for(u32 i=0; i<EPPE_COUNT; i++)
	{
		for(u32 j=0; j<EffectChain[i].size(); j++)
		{
			IPostProcess* postProcess = EffectChain[i][j];
			if (postProcess)
				postProcess->drop();
		}
	}

	// and drop the depth material
	if (DepthMaterial)
		DepthMaterial->drop();
}

//void CPostProcessManager::SwapAuxBuffers()
//{
//	// swap the in and out buffers
//	video::ITexture* tmp = RenderTargetMap["auxIn"];
//	RenderTargetMap["auxIn"] = RenderTargetMap["auxOut"];
//	RenderTargetMap["auxOut"] = tmp;
//}

void CPostProcessManager::SwapAuxBuffers()
{
    video::ITexture* texin = RenderTargetMap["auxIn"];
    video::ITexture* texout = RenderTargetMap["auxOut"];
    RenderTargetMap["auxIn"] = texout;
    RenderTargetMap["auxOut"] = texin;
}

void CPostProcessManager::prepare()
{
	// set auxOut RTT as RenderTarget
	Device->getVideoDriver()->setRenderTarget(RenderTargetMap["auxOut"]);
}

void CPostProcessManager::render(E_POSTPROCESS_EFFECT effect)
{
	if (effect<EPPE_COUNT)
	{
		// first swap the in and out buffers
		if (EffectChain[effect].size())
			SwapAuxBuffers();

		// run through the effect chain
		for (u32 i=0; i<EffectChain[effect].size(); i++)
		{
			// retrieve the post process
			IPostProcess* postProcess = EffectChain[effect][i];

			// bind input buffer
			if( !postProcess->getRenderSource().empty())
				postProcess->getMaterial().setTexture(0, RenderTargetMap[postProcess->getRenderSource()]);

			// bind output buffer
			if( !postProcess->getRenderTarget().empty())
				Device->getVideoDriver()->setRenderTarget(RenderTargetMap[postProcess->getRenderTarget()]);
			else
				Device->getVideoDriver()->setRenderTarget(video::ERT_FRAME_BUFFER, true, true);

			// render the post process
			postProcess->render();
		}
	}
}

void CPostProcessManager::update()
{
	// render the final pass
	render(EPPE_FINAL_PASS);
}

void CPostProcessManager::clearDepthPass()
{
	// clear all nodes from the depth pass
	DepthPassNodes.clear();
}

void CPostProcessManager::addNodeToDepthPass(scene::ISceneNode *node)
{
	// add node to the depth pass array
	if(DepthPassNodes.binary_search(node) == -1)
		DepthPassNodes.push_back(node);
}

void CPostProcessManager::removeNodeFromDepthPass(scene::ISceneNode *node)
{
	// remove node from depth pass array
	s32 index = DepthPassNodes.binary_search(node);
	if(index != -1)
		DepthPassNodes.erase(index);
}

void CPostProcessManager::renderDepth(const video::SColor& defaultDepth)
{
	if(DepthPassNodes.size())
	{
		// animate and render the camera to ensure correct depth and normal information
		scene::ICameraSceneNode* camera = Device->getSceneManager()->getActiveCamera();
		if (camera)
		{
			camera->OnRegisterSceneNode();
			camera->OnAnimate(Device->getTimer()->getTime());
			camera->render();
			DepthMaterial->setPixelShaderConstant("MaxDistance", camera->getFarValue());
		}
		// set depth render target texture as render target
		Device->getVideoDriver()->setRenderTarget(RenderTargetMap["rttDepth"], true, true, defaultDepth);

		// render all nodes that are stored in the depth pass array
		for(u32 i=0; i<DepthPassNodes.size(); i++)
		{
			// get the scene node from the array
			scene::ISceneNode* node = DepthPassNodes[i];

			if (node->isVisible())
			{
				// save the scene node materials
				core::array<video::E_MATERIAL_TYPE> bufferMaterialList(node->getMaterialCount());
				bufferMaterialList.set_used(0);

				for(u32 j=0; j<node->getMaterialCount(); j++)
					bufferMaterialList.push_back(node->getMaterial(j).MaterialType);

				// apply the depth material
				node->setMaterialType(DepthMaterial->getMaterialType());

				// animate the node
				node->OnAnimate(Device->getTimer()->getTime());

				// render the node
				node->render();

				// reset the scene node to the original material
				for(u32 j=0; j<node->getMaterialCount(); j++)
					node->getMaterial(j).MaterialType = bufferMaterialList[j];
			}
		}
	}
}

CEffectChain& CPostProcessManager::getEffectChain(E_POSTPROCESS_EFFECT effect)
{
	// return the desired effect chain
	if (effect < EPPE_COUNT)
		return EffectChain[effect];
	else
		return EffectChain[EPPE_NO_EFFECT];
}

void CPostProcessManager::loadRTTConfig(const char* filename)
{
	// create a xml reader
	io::IrrXMLReader* xmlReader = io::createIrrXMLReader(filename);

	// we'll be looking for the rendertarget tag in the xml
	const core::stringc renderTargetTag(L"RenderTarget");

    while(xmlReader && xmlReader->read())
    {
		switch(xmlReader->getNodeType())
		{
		case io::EXN_ELEMENT:
			{
				// we are in the Setup section and we find a rendertarget to parse
				if (renderTargetTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					// get the rtt parameters
					core::stringc id = xmlReader->getAttributeValueSafe("id");
					u32 width = (u32) xmlReader->getAttributeValueAsInt("width");
					u32 height = (u32) xmlReader->getAttributeValueAsInt("height");
					f32 scale = (f32) xmlReader->getAttributeValueAsFloat("scale");
					video::ECOLOR_FORMAT colorFormat = (video::ECOLOR_FORMAT) xmlReader->getAttributeValueAsInt("colorFormat");

					// set width and height of the rtt
					if (scale > 0.0f)
					{
						width = (u32) (scale*Device->getVideoDriver()->getScreenSize().Width);
						height = (u32) (scale*Device->getVideoDriver()->getScreenSize().Height);
					}
					if (width==0 || height==0)
					{
						width = Device->getVideoDriver()->getScreenSize().Width;
						height = Device->getVideoDriver()->getScreenSize().Height;
					}
					// add the rendertarget with its properties and store it in the render target map
					video::ITexture* texture = Device->getVideoDriver()->addRenderTargetTexture(core::dimension2d<u32>(width, height), id, colorFormat);
					RenderTargetMap[id] = texture;
				}
			}
			break;

        default: break;
		}
	}
	delete xmlReader;
}

void CPostProcessManager::loadEffectConfig(const char* filename)
{
	// create a xml reader
	io::IrrXMLReader* xmlReader = io::createIrrXMLReader(filename);

	// we'll be looking for these tags in the xml file
	const core::stringc effectTag("Effect");
	const core::stringc shaderPostProcessTag("ShaderPostProcess");
	const core::stringc renderSourceTag("RenderSource");
	const core::stringc renderTargetTag("RenderTarget");
	const core::stringc psConstant("PixelShaderConstant");
	const core::stringc vsConstant("VertexShaderConstant");
	const core::stringc textureTag("Texture");

	// each effect chain is a sequence of postprocesses
	CEffectChain* currentEffectChain = NULL;
	CShaderPostProcess* currentPostProcess = NULL;

    while(xmlReader && xmlReader->read())
    {
		switch(xmlReader->getNodeType())
		{
		case io::EXN_ELEMENT:
			{
				// we are in the Setup section and we find a effect to parse
				if (effectTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					// get the E_POSTPROCESS_EFFECT parameter
					s32 id = xmlReader->getAttributeValueAsInt("id");
					core::stringc name = xmlReader->getAttributeValueSafe("name");

					// get the correct effect chain and reset the postprocess
					currentEffectChain = NULL;
					currentPostProcess = NULL;
					if (id>=0 && id<EPPE_COUNT)
					{
						E_POSTPROCESS_EFFECT effect = (E_POSTPROCESS_EFFECT) id;
						currentEffectChain = &EffectChain[effect];
						currentEffectChain->setName(name);
					}
				}

				// we are in the shader post process section and have a valid currentEffect
				if (shaderPostProcessTag.equals_ignore_case(xmlReader->getNodeName()) && currentEffectChain)
				{
					// get the postprocess name
					core::stringc name = xmlReader->getAttributeValueSafe("name");

					// get vertex shader config
					core::stringc vsFile = xmlReader->getAttributeValueSafe("vsFile");
					core::stringc vsEntry = "main";
					if (xmlReader->getAttributeValue("vsEntry"))
						vsEntry = xmlReader->getAttributeValueSafe("vsEntry");
					video::E_VERTEX_SHADER_TYPE vsType = (video::E_VERTEX_SHADER_TYPE) xmlReader->getAttributeValueAsInt("vsType");

					// get pixel shader config
					core::stringc psFile = xmlReader->getAttributeValueSafe("psFile");
					core::stringc psEntry = "main";
					if (xmlReader->getAttributeValue("psEntry"))
						psEntry = xmlReader->getAttributeValueSafe("psEntry");
					video::E_PIXEL_SHADER_TYPE psType = (video::E_PIXEL_SHADER_TYPE) xmlReader->getAttributeValueAsInt("psType");
					video::E_MATERIAL_TYPE baseMaterial = (video::E_MATERIAL_TYPE) xmlReader->getAttributeValueAsInt("baseMaterial");

					// get additional built in shader constants for vertexshader
					bool vsUseElapsedTime = xmlReader->getAttributeValueAsInt("vsUseElapsedTime") != 0;
					bool vsUseRandom = xmlReader->getAttributeValueAsInt("vsUseRandom") != 0;
					bool vsUseBufferWidth = xmlReader->getAttributeValueAsInt("vsUseBufferWidth") != 0;
					bool vsUseBufferHeight = xmlReader->getAttributeValueAsInt("vsUseBufferHeight") != 0;
					bool vsUseProjection = xmlReader->getAttributeValueAsInt("vsUseProjection") != 0;
					bool vsUseView = xmlReader->getAttributeValueAsInt("vsUseView") != 0;
					bool vsUseWorld = xmlReader->getAttributeValueAsInt("vsUseWorld") != 0;
					bool vsUseWorldView = xmlReader->getAttributeValueAsInt("vsUseWorldView") != 0;
					bool vsUseWorldViewProj = xmlReader->getAttributeValueAsInt("vsUseWorldViewProj") != 0;

					// get additional built in shader constants for pixelshader
					bool psUseElapsedTime = xmlReader->getAttributeValueAsInt("psUseElapsedTime") != 0;
					bool psUseRandom = xmlReader->getAttributeValueAsInt("psUseRandom") != 0;
					bool psUseBufferWidth = xmlReader->getAttributeValueAsInt("psUseBufferWidth") != 0;
					bool psUseBufferHeight = xmlReader->getAttributeValueAsInt("psUseBufferHeight") != 0;
					bool psUseProjection = xmlReader->getAttributeValueAsInt("psUseProjection") != 0;
					bool psUseView = xmlReader->getAttributeValueAsInt("psUseView") != 0;
					bool psUseWorld = xmlReader->getAttributeValueAsInt("psUseWorld") != 0;
					bool psUseWorldView = xmlReader->getAttributeValueAsInt("psUseWorldView") != 0;
					bool psUseWorldViewProj = xmlReader->getAttributeValueAsInt("psUseWorldViewProj") != 0;
                    bool psUseMousePositionX = xmlReader->getAttributeValueAsInt("psUseMousePositionX") != 0;
					bool psUseMousePositionY = xmlReader->getAttributeValueAsInt("psUseMousePositionY") != 0;

					// create a new shader post process material
					currentPostProcess = new CShaderPostProcess(Device, name, vsFile, vsEntry, vsType, psFile, psEntry, psType, baseMaterial);

					// set pixel shader flags
					currentPostProcess->getShaderMaterial()->setPixelShaderFlag(ESC_TIME, psUseElapsedTime);
					currentPostProcess->getShaderMaterial()->setPixelShaderFlag(ESC_RANDOM, psUseRandom);
					currentPostProcess->getShaderMaterial()->setPixelShaderFlag(ESC_BUFFERWIDTH, psUseBufferWidth);
					currentPostProcess->getShaderMaterial()->setPixelShaderFlag(ESC_BUFFERHEIGHT, psUseBufferHeight);
					currentPostProcess->getShaderMaterial()->setPixelShaderFlag(ESC_PROJECTION, psUseProjection);
					currentPostProcess->getShaderMaterial()->setPixelShaderFlag(ESC_VIEW, psUseView);
					currentPostProcess->getShaderMaterial()->setPixelShaderFlag(ESC_WORLD, psUseWorld);
					currentPostProcess->getShaderMaterial()->setPixelShaderFlag(ESC_WORLDVIEW, psUseWorldView);
					currentPostProcess->getShaderMaterial()->setPixelShaderFlag(ESC_WORLDVIEWPROJ, psUseWorldViewProj);
                    currentPostProcess->getShaderMaterial()->setPixelShaderFlag(ESC_MOUSEPOSITIONX, psUseMousePositionX);
					currentPostProcess->getShaderMaterial()->setPixelShaderFlag(ESC_MOUSEPOSITIONY, psUseMousePositionY);

					// set vertex shader flags
					currentPostProcess->getShaderMaterial()->setVertexShaderFlag(ESC_TIME, vsUseElapsedTime);
					currentPostProcess->getShaderMaterial()->setVertexShaderFlag(ESC_RANDOM, vsUseRandom);
					currentPostProcess->getShaderMaterial()->setVertexShaderFlag(ESC_BUFFERWIDTH, vsUseBufferWidth);
					currentPostProcess->getShaderMaterial()->setVertexShaderFlag(ESC_BUFFERHEIGHT, vsUseBufferHeight);
					// set buffer size for DirectX vertex shaders
					// (vertexshader performs 0.5 texel offset for correct texture sampling,
					// this is not necessary for OpenGL)
					if(Device->getVideoDriver()->getDriverType() == video::EDT_DIRECT3D9)
					{
						currentPostProcess->getShaderMaterial()->setVertexShaderFlag(ESC_BUFFERWIDTH);
						currentPostProcess->getShaderMaterial()->setVertexShaderFlag(ESC_BUFFERHEIGHT);
					}

					currentPostProcess->getShaderMaterial()->setVertexShaderFlag(ESC_PROJECTION, vsUseProjection);
					currentPostProcess->getShaderMaterial()->setVertexShaderFlag(ESC_VIEW, vsUseView);
					currentPostProcess->getShaderMaterial()->setVertexShaderFlag(ESC_WORLD, vsUseWorld);
					currentPostProcess->getShaderMaterial()->setVertexShaderFlag(ESC_WORLDVIEW, vsUseWorldView);
					currentPostProcess->getShaderMaterial()->setVertexShaderFlag(ESC_WORLDVIEWPROJ, vsUseWorldViewProj);

					// push back the post process into the effect chain
					currentEffectChain->push_back(currentPostProcess);
				}

				// read vertex shader constants from the xml-file
				if (vsConstant.equals_ignore_case(xmlReader->getNodeName()) &&
					currentPostProcess)
				{
					// add the defined constants to the postprocess
					core::stringc name = xmlReader->getAttributeValueSafe("name");
					f32 value = xmlReader->getAttributeValueAsFloat("value");
					currentPostProcess->getShaderMaterial()->setVertexShaderConstant(name, value);
				}

				// read pixel shader constants from the xml-file
				if (psConstant.equals_ignore_case(xmlReader->getNodeName()) &&
					currentPostProcess)
				{
					// add the defined constants to the postprocess
					core::stringc name = xmlReader->getAttributeValueSafe("name");
					f32 value = xmlReader->getAttributeValueAsFloat("value");
					currentPostProcess->getShaderMaterial()->setPixelShaderConstant(name, value);
				}

				// read input texture properties from the xml-file
				if (textureTag.equals_ignore_case(xmlReader->getNodeName()) &&
					currentPostProcess)
				{
					// read texture properties
					u32 index = (u32) xmlReader->getAttributeValueAsInt("index");
					core::stringc texPath = xmlReader->getAttributeValueSafe("path");
					core::stringc texName = xmlReader->getAttributeValueSafe("name");
					video::E_TEXTURE_CLAMP texClamp = (video::E_TEXTURE_CLAMP)xmlReader->getAttributeValueAsInt("textureClamp");
					s8 texLODBias = (s8) xmlReader->getAttributeValueAsInt("lodBias");
					bool bilinearFilter = true;
					bool trilinearFilter = false;
					bool anisotropicFilter = false;
					if (xmlReader->getAttributeValue("bilinearFilter"))
						bilinearFilter = xmlReader->getAttributeValueAsInt("bilinearFilter") != 0;
					if (xmlReader->getAttributeValue("trilinearFilter"))
						trilinearFilter = xmlReader->getAttributeValueAsInt("trilinearFilter") != 0;
					if (xmlReader->getAttributeValue("anisotropicFilter"))
						anisotropicFilter = xmlReader->getAttributeValueAsInt("anisotropicFilter") != 0;

					if(index < video::MATERIAL_MAX_TEXTURES)
					{
						// set texture and clamp
						if(!texPath.empty())
							currentPostProcess->getMaterial().TextureLayer[index].Texture = Device->getVideoDriver()->getTexture(texPath);

						// set texture properties
						currentPostProcess->getMaterial().TextureLayer[index].TextureWrapU = texClamp;
						currentPostProcess->getMaterial().TextureLayer[index].TextureWrapV = texClamp;
						currentPostProcess->getMaterial().TextureLayer[index].LODBias = texLODBias;
						currentPostProcess->getMaterial().TextureLayer[index].BilinearFilter = bilinearFilter;
						currentPostProcess->getMaterial().TextureLayer[index].TrilinearFilter = trilinearFilter;
						currentPostProcess->getMaterial().TextureLayer[index].AnisotropicFilter = anisotropicFilter == true ? 0xFF : 0;

						// set texture name (used for glsl)
						if(!texName.empty())
							currentPostProcess->getShaderMaterial()->setTextureName(index, texName);
					}
				}

				// read render target for the postprocess from the xml-file
				if (renderSourceTag.equals_ignore_case(xmlReader->getNodeName()) &&
					currentEffectChain && currentPostProcess)
				{
					// set render target of the postprocess
					core::stringc texPath = xmlReader->getAttributeValueSafe("path");
					currentPostProcess->setRenderSource(texPath);
				}

				// read render target for the postprocess from the xml-file
				if (renderTargetTag.equals_ignore_case(xmlReader->getNodeName()) &&
					currentEffectChain && currentPostProcess)
				{
					// set render target of the postprocess
					core::stringc texPath = xmlReader->getAttributeValueSafe("path");
					currentPostProcess->setRenderTarget(texPath);
				}
				break;
			}

        default: break;
		}
	}
	delete xmlReader;
}
