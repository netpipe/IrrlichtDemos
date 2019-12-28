#ifndef MetaballSceneNode_h
#define MetaballSceneNode_h

#include <deque>
using namespace std;

#include <irrlicht.h>
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")

class MetaballSceneNodeFactory : public ISceneNodeFactory {
	public:
		MetaballSceneNodeFactory(ISceneManager *mgr);
		virtual ISceneNode *addSceneNode(ESCENE_NODE_TYPE type, ISceneNode *parent = 0);
		virtual ISceneNode *addSceneNode(const c8 *typeName, ISceneNode *parent = 0);
		virtual u32 getCreatableSceneNodeTypeCount() const;
		virtual const c8 *getCreateableSceneNodeTypeName(u32 idx) const;
		virtual ESCENE_NODE_TYPE getCreateableSceneNodeType(u32 idx) const;
		virtual const c8 *getCreateableSceneNodeTypeName(ESCENE_NODE_TYPE type) const;

	private:
		ESCENE_NODE_TYPE getTypeFromName(const c8 *name) const;
		struct SSceneNodeTypePair {
			SSceneNodeTypePair(ESCENE_NODE_TYPE type, const c8 *name) : Type(type), TypeName(name){}
			ESCENE_NODE_TYPE Type;
			stringc TypeName;
		};
		array<SSceneNodeTypePair> SupportedSceneNodeTypes;
		ISceneManager *Manager;
};

class MetaballSceneNode : public ISceneNode {
	public:
		MetaballSceneNode(ISceneManager *mgr, ISceneNode *parent = 0, s32 id = -1);
		virtual void OnRegisterSceneNode();
		virtual void render();

		virtual void addMetaball(vector3df position, SColor color);
		virtual void triangulateVolume();

		virtual void setThreshold(f32 threshold);
		virtual void setBound(f32 bound);
		virtual void setSampleWidth(f32 width);

		virtual const aabbox3d<f32> &getBoundingBox() const;
		virtual u32 getMaterialCount();
		virtual SMaterial &getMaterial(u32 i);
		virtual ESCENE_NODE_TYPE getType() const;

		virtual void serializeAttributes(IAttributes *out, SAttributeReadWriteOptions *options = 0) const;
		virtual void deserializeAttributes(IAttributes *in, SAttributeReadWriteOptions *options = 0);
		deque<vector3df> Balls;
	protected:
		aabbox3d<f32> Box;
		SMaterial Material;
		SMeshBuffer* meshBuffer;
		
		deque<SColor> Colors;
		f32 Threshold;
		f32 Bound;
		f32 SampleWidth;

		void triangulate(vector3df position, f32 scale);
		f32 calculateValue(vector3df position);
		SColor calculateColor(vector3df position);
		f32 valueFunction(vector3df position, vector3df center);
};

#endif