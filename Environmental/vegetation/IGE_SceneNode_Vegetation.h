#pragma once

#include "irrlicht.h"
using namespace irr;
using namespace video;
using namespace core;
using namespace gui;
using namespace scene;
using namespace io;

namespace IGE
{
	// if color is below this value thn no node is present
	#define CUTOFF 50
	#define YCUTOFF 0

	// simple rendom number generators
	extern int getRandomInteger(int min, int max);
	extern float getRandomFloat(float min, float max);

	// simple structure to hold a template node and the YOffset to apply when positioning the node
	struct IGE_MeshTemplate
	{
		IGE_MeshTemplate(IMeshSceneNode* node, float yoffset) : m_Node(node), m_YOffset(yoffset) {}
		IMeshSceneNode* m_Node		= nullptr;
		float			m_YOffset	= 0.0f;
	};

	/*
	*  the scenenode class
	*   once the scenenode is instantiated
	*	add as many templates as desired (these are the different nodes to be used)
	*
	*
	*/
	class IGE_SceneNode_Vegetation : public ISceneNode
	{
	public:
		ITerrainSceneNode*			m_Terrain		= nullptr;							// the terrain scenenode we are attached to
		IImage*						m_LayoutImage	= nullptr;							// the layout image - used to determine whether a node/object is at any given position

		array<IGE_MeshTemplate>		m_MeshTemplate;										// list	of nodes being used
		array<ISceneNode*>			m_Nodes;											// list of visible nodes
		array<short int>			m_NodeType;											// list of node types (index into the MeshTemplates)
		array<vector3df>			m_NodePos;											// list of node positions
		array<vector3df>			m_NodeScale;										// list of node scales
		array<vector3df>			m_NodeRotation;										// list of node rotations
		array<bool>					m_IsNodePosFree;									// list of open and closed slots in the node list

		float						m_ViewDistance				= 1000;					// distance from camera at which nodes become visible
		f32							m_VisibleOffset				= 200.f;				// resize the camera frustum by this amount, you can use the maximal radius of your mesh to prevent close plane popping
		vector3df					m_LayoutRandomness			= vector3df(0,0,0);		// add some randomness to the layout procedure

		int							m_HelperForDeleting			= false;				// variable used during update
		bool						m_UseLight					= false;				// simple variable to track light use
		bool						m_UseFog					= false;				// simple variable to track fog use

		ICameraSceneNode*			m_CameraToUse				= 0;					// camera to use for determining node visibility
	public:
		// add a node template
		void				addMeshTemplate(stringc meshfilename, float YOffset);

		// using the terrain as the location base and the layout functions to determine position, rotation and scale of nodes
		void				processLayout();

		// clear all lists
		void				clear();

		// set which camera we use for determining node visibility
		void setCameraToUse(ICameraSceneNode* CameraToUse) { m_CameraToUse = CameraToUse; };

		// called each tick. determine which nodes should be visible based on the camera position and rotation
		void				update(vector3df center, ICameraSceneNode* camera);

		// use color information to determine the density, style, rotation and scale of the nodes
		virtual int			getDensityFromLayoutImageColor(SColor pixel);
		virtual int			getStyleFromLayoutImageColor(SColor pixel, int templatesize, vector3df pos);
		virtual vector3df   getRotationFromLayoutImageColor(SColor pixel, int templatesize, int style, vector3df pos);
		virtual vector3df   getScaleFromLayoutImageColor(SColor pixel, int templatesize, int style, vector3df pos);

		// simple getter functions
		float				getViewDistance()				{ return m_ViewDistance;	}
		bool				getUseLight()					{ return m_UseLight;		}
		bool				getUseFog()						{ return m_UseFog;			}

		// toggle light, fog and visibility
		virtual				void setViewDistance(float d)	{ m_ViewDistance = d;		}
		virtual				void setUseLight(bool v);
		virtual				void setUseFog(bool v);
		virtual				void setVisible(bool v);

	private:
		// is the point visible within the camera frustum
		bool				canBeSeenByCamPyramidPlanes(const SViewFrustum& frust, const vector3df& pos);

	public:

			//! constructor
			IGE_SceneNode_Vegetation(ITerrainSceneNode* parent, stringc layoutimagefilename, float viewdistance, vector3df layoutRandomness, bool uselight, bool usefog, ISceneManager* smgr, s32 id);

			//! destructor
			virtual ~IGE_SceneNode_Vegetation();

			//! frame
			virtual void OnRegisterSceneNode() ;

			//! renders the node.
			virtual void render() ;

			//! returns the axis aligned bounding box of this node
			virtual const core::aabbox3d<f32>& getBoundingBox() const ;

			//! Writes attributes of the scene node.
			virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options = 0) const ;

			//! Reads attributes of the scene node.
			virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options = 0) ;

			//! Returns type of the scene node
			virtual ESCENE_NODE_TYPE getType() const  { return ESNT_MESH; }

	protected:
			core::aabbox3d<f32>				Box;
	};

} // end namespace IGE
