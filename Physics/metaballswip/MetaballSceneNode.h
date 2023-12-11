#ifndef MetaballSceneNode_h
#define MetaballSceneNode_h

#include <irrlicht.h>

#pragma comment(lib, "Irrlicht.lib")

class MetaballSceneNodeFactory : public irr::scene::ISceneNodeFactory {
	public:
		MetaballSceneNodeFactory(irr::scene::ISceneManager *mgr);
		virtual irr::scene::ISceneNode *addSceneNode(irr::scene::ESCENE_NODE_TYPE type, irr::scene::ISceneNode *parent = 0);
		virtual irr::scene::ISceneNode *addSceneNode(const irr::c8 *typeName, irr::scene::ISceneNode *parent = 0);
		virtual irr::u32 getCreatableSceneNodeTypeCount() const;
		virtual const irr::c8 *getCreateableSceneNodeTypeName(irr::u32 idx) const;
		virtual irr::scene::ESCENE_NODE_TYPE getCreateableSceneNodeType(irr::u32 idx) const;
		virtual const irr::c8 *getCreateableSceneNodeTypeName(irr::scene::ESCENE_NODE_TYPE type) const;

	private:
		irr::scene::ESCENE_NODE_TYPE getTypeFromName(const irr::c8 *name) const;
		struct SSceneNodeTypePair {
			SSceneNodeTypePair(irr::scene::ESCENE_NODE_TYPE type, const irr::c8 *name) : Type(type), TypeName(name){}
			irr::scene::ESCENE_NODE_TYPE Type;
			irr::core::stringc TypeName;
		};
		irr::core::array<SSceneNodeTypePair> SupportedSceneNodeTypes;
		irr::scene::ISceneManager *Manager;
};

class MetaballSceneNode : public irr::scene::ISceneNode
{
	public:
		MetaballSceneNode(irr::scene::ISceneManager *mgr, irr::scene::ISceneNode *parent = 0, irr::s32 id = -1);
		virtual void OnRegisterSceneNode();
		virtual void render();

		virtual void addMetaball(irr::core::vector3df position, irr::video::SColor color);
		virtual void triangulateVolume();

		virtual void setThreshold(irr::f32 threshold);
		virtual void setBound(irr::f32 bound);
		virtual void setSampleWidth(irr::f32 width);

		virtual const irr::core::aabbox3d<irr::f32> &getBoundingBox() const;
		virtual irr::u32 getMaterialCount();
		virtual irr::video::SMaterial &getMaterial(irr::u32 i);
		virtual irr::scene::ESCENE_NODE_TYPE getType() const;

		virtual void serializeAttributes(irr::io::IAttributes *out, irr::io::SAttributeReadWriteOptions *options = 0) const;
		virtual void deserializeAttributes(irr::io::IAttributes *in, irr::io::SAttributeReadWriteOptions *options = 0);
		irr::core::array<irr::core::vector3df> Balls;
		irr::f32 SampleWidth;
	protected:
		irr::core::aabbox3d<irr::f32> Box;
		irr::video::SMaterial Material;
		irr::scene::SMeshBuffer* meshBuffer;

		irr::core::array<irr::video::SColor> Colors;
		irr::f32 Threshold;
		irr::f32 Bound;
		irr::core::array<irr::core::vector3df> PositionList;
		irr::core::array<irr::f32> PositionFloatList;
		irr::core::array<irr::f32> BallFloatList;
		irr::core::array<bool> ProximityList;

		void triangulate();
		inline irr::f32 calculateValue(irr::core::vector3df const& position);
		inline irr::video::SColor calculateColor(irr::core::vector3df const& position);
		inline irr::f32 valueFunction(irr::core::vector3df const& position, irr::core::vector3df const& center);
};

#endif
