
#include "IGE_SceneNode_Vegetation.h"

namespace IGE
{
	int getRandomInteger(int min, int max) { return rand() % (max - min + 1) + min; } //return int((double)rand() / (RAND_MAX + 1) * (max - min) + min);
	float getRandomFloat(float min, float max) { return float((double)rand() / (RAND_MAX + 1) * (max - min) + min); }

	//! constructor
	IGE_SceneNode_Vegetation::IGE_SceneNode_Vegetation(ITerrainSceneNode* parent, stringc layoutimagefilename, float viewdistance, vector3df layoutRandomness, bool uselight, bool usefog, 
		ISceneManager* smgr, s32 id)
		: ISceneNode(parent, smgr, id, vector3df(),vector3df(), vector3df(1,1,1)),m_Terrain(parent),m_ViewDistance(viewdistance),m_LayoutRandomness(layoutRandomness),m_UseLight(uselight),m_UseFog(usefog)
	{
#ifdef _DEBUG
		setDebugName("IGE_SceneNode_Vegetation");
#endif
		Box = parent->getBoundingBox();
		m_LayoutImage = smgr->getVideoDriver()->createImageFromFile(layoutimagefilename.c_str());
	}


	//! destructor
	IGE_SceneNode_Vegetation::~IGE_SceneNode_Vegetation()
	{
		if (m_LayoutImage) m_LayoutImage->drop(); m_LayoutImage = 0;
		clear();
	}


	//! frame
	void IGE_SceneNode_Vegetation::OnRegisterSceneNode()
	{
		if (IsVisible)
		{
			SceneManager->registerNodeForRendering(this, scene::ESNRP_TRANSPARENT);

			ISceneNode::OnRegisterSceneNode();
		}
	}


	//! renders the node.
	void IGE_SceneNode_Vegetation::render()
	{
		if (m_CameraToUse != 0)
		{
			update(m_CameraToUse->getPosition(), m_CameraToUse);
		}
		else
		{
			update(SceneManager->getActiveCamera()->getPosition(), SceneManager->getActiveCamera());
		}
	//	ISceneNodeList::Iterator it = Children.begin();
	//	for (; it != Children.end(); ++it)
	//		(*it)->render();
	}

	//! returns the axis aligned bounding box of this node
	const core::aabbox3d<f32>& IGE_SceneNode_Vegetation::getBoundingBox() const
	{
		return Box;
	}

	//! Writes attributes of the scene node.
	void IGE_SceneNode_Vegetation::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
	{
		ISceneNode::serializeAttributes(out, options);
	}


	//! Reads attributes of the scene node.
	void IGE_SceneNode_Vegetation::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
	{
		ISceneNode::deserializeAttributes(in, options);
	}

	// add a new template to the list
	void IGE_SceneNode_Vegetation::addMeshTemplate(stringc meshfilename, float YOffset)
	{
		// load the mesh
		IMesh* Mesh = SceneManager->getMesh(meshfilename.c_str());

		// if the mesh is valid we will use it
		if (Mesh)
		{
			IMeshSceneNode* node = SceneManager->addMeshSceneNode(Mesh);
			node->setPosition(vector3df(0, -99999, 0));
			node->getMesh()->setHardwareMappingHint(EHM_STATIC);
			node->setName("plsDoNotDeleteMe");
			node->setMaterialFlag(EMF_FOG_ENABLE, m_UseFog);
			node->setMaterialFlag(EMF_LIGHTING, m_UseLight);
			node->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL_REF);

			// add the mesh to the list
			IGE_MeshTemplate t(node, YOffset);
			m_MeshTemplate.push_back(t);
		}

	}

	// clear everything
	void IGE_SceneNode_Vegetation::clear()
	{
		// clear the visible nodes / objects
		if (!m_Nodes.empty())
		{
			for (u32 x = 0; x < m_Nodes.size(); x++)
			{
				if (m_Nodes[x] != NULL)
				{
					m_Nodes[x]->remove();
					m_Nodes[x] = nullptr;
				}
			}
		}

		// clear all of the lists
		m_Nodes.clear();
		m_Nodes.reallocate(0, true);
		m_NodePos.clear();
		m_NodePos.reallocate(0, true);
		m_NodeScale.clear();
		m_NodeScale.reallocate(0, true);
		m_NodeRotation.clear();
		m_NodeRotation.reallocate(0, true);
		m_NodeType.clear();
		m_NodeType.reallocate(0, true);
		m_IsNodePosFree.clear();
		m_IsNodePosFree.reallocate(0, true);
	}

	void IGE_SceneNode_Vegetation::processLayout()
	{
		// if we dont have any node / object efinitions to use then bail
		if (!(m_MeshTemplate.size() > 0))
		{
			printf("WARNING! IGE_SceneNode_Vegetation::processLayout() no plane templates found!\n");
			return;
		}

		// if we dont have a terrain then bail
		if (!m_Terrain)
		{
			printf("ERROR! IGE_SceneNode_Vegetation::processLayout() no terrain object found!\n");
			return;
		}

		// make sure that we have a layout image else bail
		if (!m_LayoutImage)
		{
			printf("ERROR! IGE_SceneNode_Vegetation::processLayout() no layout image\n");
			return;
		}

		// get the edges of the terrain scenenode
		aabbox3d<f32> terrainBox = m_Terrain->getBoundingBox();
		Box = terrainBox;
		float minX = terrainBox.MinEdge.X;
		float minZ = terrainBox.MinEdge.Z;

		m_Terrain->updateAbsolutePosition();
		float terrainX = m_Terrain->getPosition().X;
		float terrainZ = m_Terrain->getPosition().Z;

		// get the parsing scale of the layout image vs the terrain sizes
		float scaleX = terrainBox.getExtent().X / m_LayoutImage->getDimension().Width;
		float scaleZ = terrainBox.getExtent().Z / m_LayoutImage->getDimension().Height;

		float density = 1.0f;

		// based on the density, run throught the lists
		for (float i = minX; i < minX + terrainBox.getExtent().X; i+=density)
		{
			for (float ii = minZ; ii < minZ + terrainBox.getExtent().Z; )
			{
				// randomize the position a bit to prevent straight lines
				vector3df randomness(
					getRandomFloat(-m_LayoutRandomness.X, m_LayoutRandomness.X),
					getRandomFloat(-m_LayoutRandomness.Y, 0),
					getRandomFloat(-m_LayoutRandomness.Z, m_LayoutRandomness.Z));

				// get the height of the terrain at the current position
				float height = m_Terrain->getHeight(i + randomness.X, ii + randomness.Z);
				//if (height > -99990)
				{
					// get the layout image pixel color at the point relative to the terrain
					int x2 = (int)(i / scaleX);
					int y2 = (int)(ii / scaleZ);
					SColor pixel = m_LayoutImage->getPixel(x2, y2);

					vector3df pos(terrainX + i + randomness.X, height + randomness.Y, terrainZ + ii + randomness.Z);
					
					// let the user select a style based on the color
					int         style		= getStyleFromLayoutImageColor(pixel, m_MeshTemplate.size(), pos);
					vector3df   scale		= getScaleFromLayoutImageColor(pixel, m_MeshTemplate.size(), style, pos);
					vector3df   rotation	= getRotationFromLayoutImageColor(pixel, m_MeshTemplate.size(), style, pos);
				
					density = getDensityFromLayoutImageColor(pixel);
					ii += density;

					// if there is a style then a node should be created at this position
					// add at least a NULL to each list to make sure the lists sizes are matched
					if (style != -2)
					{
						pos.Y -= m_MeshTemplate[style].m_YOffset;

						m_Nodes.push_back(nullptr);
						m_NodeType.push_back(style);
						m_NodePos.push_back(pos);
						m_NodeScale.push_back(scale);
						m_NodeRotation.push_back(rotation);
						m_IsNodePosFree.push_back(true);
					}
				}
			}
		}
	}

	// check if a point is inside the viewfromcamera view frustrum 
	bool IGE_SceneNode_Vegetation::canBeSeenByCamPyramidPlanes(const SViewFrustum& frust, const vector3df& pos)
	{
		for (s32 i = 0; i < scene::SViewFrustum::VF_PLANE_COUNT; ++i)
		{
			if (frust.planes[i].classifyPointRelation(pos) == core::ISREL3D_FRONT)
				return false;
		}
		return true;
	}

	// called each frame to update what is visible and what is not
	void IGE_SceneNode_Vegetation::update(vector3df center, ICameraSceneNode* camera)
	{
		if (!isVisible()) return;

		if (m_HelperForDeleting == 0) m_HelperForDeleting = 1; else m_HelperForDeleting = 0;

		// make the frustum a little bigger to stop popping near the front plane
		SViewFrustum origFrustum = *camera->getViewFrustum();
		SViewFrustum frustum = origFrustum;
		for (int i = 0; i < scene::SViewFrustum::VF_PLANE_COUNT; ++i)
		{
			frustum.planes[i].recalculateD(frustum.planes[i].getMemberPoint() + frustum.planes[i].Normal * m_VisibleOffset);
		}
		frustum.recalculateBoundingBox();

		for (u32 i = m_HelperForDeleting; i < m_NodePos.size(); i += 2)
		{
			line3d<f32> ray;
			ray.start = center;
			ray.end = m_NodePos[i];
			float d = ray.getLength();
			if (m_IsNodePosFree[i] && d < m_ViewDistance && canBeSeenByCamPyramidPlanes(frustum, m_NodePos[i]))
			{
				if (m_MeshTemplate[m_NodeType[i]].m_Node != nullptr)
				{
					m_Nodes[i] = m_MeshTemplate[m_NodeType[i]].m_Node->clone();
					m_Nodes[i]->setPosition(m_NodePos[i]);
					m_Nodes[i]->setRotation(m_NodeRotation[i]);
					m_Nodes[i]->setScale(m_NodeScale[i]);
					m_Nodes[i]->setMaterialFlag(EMF_LIGHTING, m_UseLight);
					m_Nodes[i]->setMaterialFlag(EMF_FOG_ENABLE, m_UseFog);
					m_IsNodePosFree[i] = false;
				}
			}
			else
			{
				if (d > m_ViewDistance || !canBeSeenByCamPyramidPlanes(frustum, m_NodePos[i]))
				{
					if (m_Nodes[i] != nullptr) m_Nodes[i]->remove(); m_Nodes[i] = nullptr;
					m_IsNodePosFree[i] = true;
				}
			}
		}
	}

	int	IGE_SceneNode_Vegetation::getDensityFromLayoutImageColor(SColor pixel)
	{
		return 1;
	}

	int	IGE_SceneNode_Vegetation::getStyleFromLayoutImageColor(SColor pixel, int templatesize, vector3df pos)
	{ 
		return (getRandomInteger(1,templatesize))-1;
	}

	vector3df   IGE_SceneNode_Vegetation::getRotationFromLayoutImageColor(SColor pixel, int templatesize, int style, vector3df pos)
	{ 
		return vector3df(0, getRandomFloat(0, 360), 0);
	}

	vector3df   IGE_SceneNode_Vegetation::getScaleFromLayoutImageColor(SColor pixel, int templatesize, int style, vector3df pos)
	{ 
		return vector3df(getRandomFloat(1, 3), getRandomFloat(1, 3), getRandomFloat(1, 3));
	}

	void  IGE_SceneNode_Vegetation::setUseLight(bool v)
	{
		m_UseLight = v;
		for (u32 i = 0; i < m_Nodes.size(); i++)
			if (m_Nodes[i] != nullptr)
				m_Nodes[i]->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, v);

		for (u32 i = 0; i < m_MeshTemplate.size(); i++)
			m_MeshTemplate[i].m_Node->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, v);
	}

	void  IGE_SceneNode_Vegetation::setUseFog(bool v)
	{
		m_UseFog = v;
		for (u32 i = 0; i < m_Nodes.size(); i++)
			if (m_Nodes[i] != nullptr)
				m_Nodes[i]->setMaterialFlag(E_MATERIAL_FLAG::EMF_FOG_ENABLE, v);

		for (u32 i = 0; i < m_MeshTemplate.size(); i++)
				m_MeshTemplate[i].m_Node->setMaterialFlag(E_MATERIAL_FLAG::EMF_FOG_ENABLE, v);
	}

	void  IGE_SceneNode_Vegetation::setVisible(bool v)
	{
		if (isVisible() == v) return;
		ISceneNode::setVisible(v);
		for (u32 i = 0; i < m_Nodes.size(); i++)
			if (m_Nodes[i] != nullptr)
				m_Nodes[i]->setVisible(v);
		
		for (u32 i = 0; i < m_MeshTemplate.size(); i++)
				m_MeshTemplate[i].m_Node->setVisible(v);
	}


} // end namspace IGE


