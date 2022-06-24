#ifndef __I_Q3MODELSCENENODE_H_INCLUDED__
#define __I_Q3MODELSCENENODE_H_INCLUDED__

#include <IAnimatedMeshSceneNode.h>
#include "IQ3ModelMesh.h"
#include "animation.h"
#include "skin.h"
#include "shader.h"

namespace irr
{
namespace scene
{

class IQ3ModelSceneNode : public ISceneNode
{
public:

        //! Constructor
        IQ3ModelSceneNode(ISceneNode* parent,
                          ISceneManager* mgr,
                          s32 id,
                          const core::vector3df& position = core::vector3df(0,0,0),
                          const core::vector3df& rotation = core::vector3df(0,0,0),
                          const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f))
             : ISceneNode(parent, mgr, id, position, rotation, scale)
        {
        }

        //! Destructor
        virtual ~IQ3ModelSceneNode() {}

		//! Sets the current frame number.
		/** From now on the animation is played from this frame.
		\param frame: Number of the frame to let the animation be started from.
		The frame number must be a valid frame number of the IMesh used by this
		scene node. Set IAnimatedMesh::getMesh() for details. */
		virtual void setCurrentFrame(u32 frame) = 0;

		//! Sets the frame numbers between the animation is looped.
		/** The default is 0 - MaximalFrameCount of the mesh.
		\param begin: Start frame number of the loop.
		\param end: End frame number of the loop.
		\return True if successful, false if not. */
		virtual bool setFrameLoop(s32 begin, s32 end) = 0;

		//! Sets the speed with witch the animation is played.
		/** \param framesPerSecond: Frames per second played. */
		virtual void setAnimationSpeed(f32 framesPerSecond) = 0;

		//! Sets looping mode which is on by default.
		/** If set to false, animations will not be played looped. */
		virtual void setLoopMode(bool playAnimationLooped) = 0;

		//! Creates shadow volume scene node as child of this node.
		/** The shadow can be rendered using the ZPass or the zfail
		method. ZPass is a little bit faster because the shadow volume
		creation is easier, but with this method there occur ugly
		looking artifacs when the camera is inside the shadow volume.
		These error do not occur with the ZFail method.
		\param id: Id of the shadow scene node. This id can be used to
		identify the node later.
		\param zfailmethod: If set to true, the shadow will use the
		zfail method, if not, zpass is used.
		\param infinity: Value used by the shadow volume algorithm to
		scale the shadow volume.
		\return Pointer to the created shadow scene node. This pointer
		should not be dropped. See IReferenceCounted::drop() for more
		information. */
		virtual IShadowVolumeSceneNode* addShadowVolumeSceneNode(s32 id=-1, bool zfailmethod=true, f32 infinity=10000.0f) = 0;


		//! Get a pointer to a joint in the mesh (from mesh tag elements).
		/** With this method it is possible to attach scene nodes to
		joints for example possible to attach a weapon to the left hand
		of an model. This example shows how:
		\code
		ISceneNode* hand = yourQ3ModelSceneNode->getJointNode("tag_weapon");
        hand->addChild(weaponSceneNode);
		\endcode
		Please note that the joint returned by this method may not exist
		before this call and the joints in the node were created by it.
		\param jointName: Name of the joint ("tag_head", "tag_torso", "tag_weapon", etc.).
		\return Pointer to the scene node which represents the joint
		with the specified name. Returns 0 if the name of the joint
		could not be found. */
		virtual ISceneNode* getJointNode(const c8* jointName)=0;

		//! Same as getJointNode(const c8* jointName), but based on tag index.
		virtual ISceneNode* getJointNode(u32 index) = 0;

		//! Same as getJointNode(const c8* jointName), but based on EMD3_TAG_TYPE.
		virtual ISceneNode* getJointNode(EMD3_TAG_TYPE type) = 0;

		//! Add a bone with another mesh, sealing the union with a tag.
		/** This union is established by the type of model. If the combination
		of model types is invalid, the union is not established and will return a NULL.
		Please note that the joint returned by this method may not exist
		before this call and the joints in the node were created by it.
		\param jointNode: Scene node to join.
		\return Pointer to the scene node which represents the joint
		with the specified name. Returns NULL if the union is not possible. */
		virtual ISceneNode* addJointToNode(IQ3ModelSceneNode* jointNode) = 0;


		//! Returns the current displayed frame number.
		virtual f32 getFrameNr() const = 0;

		//! Returns the current start frame number.
		virtual s32 getStartFrame() const = 0;

		//! Returns the current end frame number.
		virtual s32 getEndFrame() const = 0;

		//! Gets looping mode from current animation.
		/** If set to false, animations will not be played looped. */
		virtual bool isLoopedMode() = 0;

		//! Sets a callback interface which will be called if an animation playback has ended.
		/** Set this to 0 to disable the callback again.
		Please note that this will only be called when in non looped
		mode, see IQ3ModelSceneNode::setLoopMode(). */
		virtual void setAnimationEndCallback(IAnimationEndCallBack* callback=0) = 0;

		//! Sets a new mesh
		virtual void setMesh(IQ3ModelMesh* mesh) = 0;

		//! Returns the current mesh
		virtual IAnimatedMesh* getMesh(void) = 0;

		//! Returns the current Q3 mesh
		virtual IQ3ModelMesh* getQ3Mesh(void) = 0;

		//! render mesh ignoring it's transformation.
		/** Used with ragdolls. Culling is unaffected. */
		virtual void setRenderFromIdentity( bool On )=0;

		//! Sets animation config.
		virtual void setAnimationConfig(quake3::Animation * animation) = 0;

		//! Sets animation type.
		virtual void setAnimation(quake3::EMD3_MODEL_ANIMATION_TYPE animationType) = 0;

		//! Gets animation type.
		virtual quake3::EMD3_MODEL_ANIMATION_TYPE getAnimation() const = 0;

		//! Sets skin.
		virtual void setSkin(quake3::Skin * skin) = 0;

		//! Sets shader.
		virtual void setShader(u32 materialIndex, const quake3::SShader * shader, io::IFileSystem * fileSystem) = 0;

		//! Sets shaders from a list of shader.
		virtual void setShaderFromList(quake3::ShadersParser * shaders, io::IFileSystem * fileSystem) = 0;


}; // class IQ3ModelSceneNode


}

}



#endif // __I_Q3MODELSCENENODE_H_INCLUDED__
