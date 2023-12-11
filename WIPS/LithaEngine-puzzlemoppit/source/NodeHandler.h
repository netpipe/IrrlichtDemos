
#ifndef NODE_HANDLER_H
#define NODE_HANDLER_H

#include "INodeHandler.h"

class NodeHandler : public INodeHandler
{
	scene::ISceneNode *irrNode;
	bool removeOnDestruct;
	
public:
	NodeHandler(scene::ISceneNode *irrNode, bool removeOnDestruct)
	{
		this->irrNode = irrNode;
		this->removeOnDestruct = removeOnDestruct;
	}
	
	~NodeHandler()
	{
		if (removeOnDestruct)
			irrNode->remove();
	}
	
	scene::ISceneNode *GetIrrNode() { return irrNode; }
	
	
	// NOTE: Most of the IGraphic functions are not implemented yet.
	// As this is not really used for anything.
	/*
	u32 GetMaterialCount() { return irrNode->getMaterialCount(); }
	
	video::SMaterial &GetMaterial(u32 material, u16 pass = 0) { return irrNode->getMaterial(material); }
	
	void SetShader(u32 material, IShader *shader, u16 pass = 0) {}
	
	void SetAllShaders(IShader *shader, u16 pass = 0) {}
	
	void DisableMaterial(u32 material, u16 pass = 0) {}
	
	void DisableAllMaterials(u16 pass) {}
	*/
	
	void ReceiveRenderPosition(core::vector3df pos);
	void ReceiveRenderRotation(core::vector3df rot);
};

#endif




