#ifndef __I_EMPTY_BILLBOARD_SCENE_NODE_H_INCLUDED__
#define __I_EMPTY_BILLBOARD_SCENE_NODE_H_INCLUDED__

#include "IBillboardSceneNode.h"

namespace irr
{
namespace scene
{
class IEmptyBillboardSceneNode;
}
}

#include "IBatchedBillboardSceneNode.h"

namespace irr
{
namespace scene
{


class IEmptyBillboardSceneNode : public ISceneNode
{
    core::dimension2d<f32> Size;
    core::aabbox3df bbox;
    video::SColor TC,BC;
    bool IsInBatchVisible;
    IBatchedBillboardSceneNode* BatchParent;

public:

	//! Constructor
	IEmptyBillboardSceneNode(IBatchedBillboardSceneNode* batchParent, IBillboardSceneNode* node);

    //! Destructor
    virtual ~IEmptyBillboardSceneNode();

	//! Sets the size of the billboard.
	virtual void setSize(const core::dimension2d<f32>& size);

	//! Returns the size of the billboard.
	virtual const core::dimension2d<f32>& getSize() {return Size;}

	//! Set the color of all vertices of the billboard
	/** \param overallColor: the color to set */
	virtual void setColor(const video::SColor & overallColor);

	//! Set the color of the top and bottom vertices of the billboard
	/** \param topColor: the color to set the top vertices
	\param bottomColor: the color to set the bottom vertices */
	virtual void setColor(const video::SColor & topColor, const video::SColor & bottomColor);

	//! Gets the color of the top and bottom vertices of the billboard
	/** \param topColor: stores the color of the top vertices
	\param bottomColor: stores the color of the bottom vertices */
	virtual void getColor(video::SColor & topColor, video::SColor & bottomColor) const {
	    topColor = TC;
	    bottomColor = BC;
	}

    //! This method is called just before the rendering process of the whole scene.
    /** Nodes may register themselves in the render pipeline during this call,
    precalculate the geometry which should be renderered, and prevent their
    children from being able to register themselves if they are clipped by simply
    not calling their OnRegisterSceneNode method.
    If you are implementing your own scene node, you should overwrite this method
    with an implementation code looking like this:
    \code
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
    \endcode
    */
    virtual void OnRegisterSceneNode();

    //! OnAnimate() is called just before rendering the whole scene.
    /** Nodes may calculate or store animations here, and may do other useful things,
    depending on what they are. Also, OnAnimate() should be called for all
    child scene nodes here. This method will be called once per frame, independent
    of whether the scene node is visible or not.
    \param timeMs Current time in milliseconds. */
    virtual void OnAnimate(u32 timeMs);

    //! Renders the node.
    virtual void render() {}
    virtual const core::aabbox3d<f32>& getBoundingBox() const {return bbox;}

    virtual bool isInBatchVisible() const {return IsInBatchVisible;}

    //! Returns whether the node should be visible (if all of its parents are visible).
    /** This is only an option set by the user, but has nothing to
    do with geometry culling
    \return The requested visibility of the node, true means
    visible (if all parents are also visible). */
    virtual bool isVisible() const {return false;}

    //! Check whether the node is truly visible, taking into accounts its parents' visibility
    /** \return true if the node and all its parents are visible,
    false if this or any parent node is invisible. */
    virtual bool isTrulyVisible() const {return false;}

    //! Sets if the node should be visible or not.
    /** All children of this node won't be visible either, when set
    to false. Invisible nodes are not valid candidates for selection by
    collision manager bounding box methods.
    \param isVisible If the node shall be visible. */
    virtual void setVisible(bool isVisible) {IsInBatchVisible = isVisible;}


    //! Removes this scene node from the scene
    /** If no other grab exists for this node, it will be deleted.
    */
    virtual void remove();

    //! Returns the material based on the zero based index i.
    /** To get the amount of materials used by this scene node, use
    getMaterialCount(). This function is needed for inserting the
    node into the scene hierarchy at an optimal position for
    minimizing renderstate changes, but can also be used to
    directly modify the material of a scene node.
    \param num Zero based index. The maximal value is getMaterialCount() - 1.
    \return The material at that index. */
    virtual video::SMaterial& getMaterial(u32 num);

    //! Get amount of materials used by this scene node.
    /** \return Current amount of materials of this scene node. */
    virtual u32 getMaterialCount() const;

    //! Sets all material flags at once to a new value.
    /** Useful, for example, if you want the whole mesh to be
    affected by light.
    \param flag Which flag of all materials to be set.
    \param newvalue New value of that flag. */
    void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue)
    {
        //Can't set, because the batch would be all wrong
    }

    //! Sets the texture of the specified layer in all materials of this scene node to the new texture.
    /** \param textureLayer Layer of texture to be set. Must be a
    value smaller than MATERIAL_MAX_TEXTURES.
    \param texture New texture to be used. */
    void setMaterialTexture(u32 textureLayer, video::ITexture* texture)
    {
        //Can't set, because the batch would be all wrong
    }

    //! Sets the material type of all materials in this scene node to a new material type.
    /** \param newType New type of material to be set. */
    void setMaterialType(video::E_MATERIAL_TYPE newType)
    {
        //Can't set, because the batch would be all wrong
    }

    //! Sets the relative scale of the scene node.
    /** \param scale New scale of the node, relative to its parent. */
    virtual void setScale(const core::vector3df& scale);

    //! Sets the rotation of the node relative to its parent.
    /** This only modifies the relative rotation of the node.
    \param rotation New rotation of the node in degrees. */
    virtual void setRotation(const core::vector3df& rotation);

    //! Sets the position of the node relative to its parent.
    /** Note that the position is relative to the parent.
    \param newpos New relative position of the scene node. */
    virtual void setPosition(const core::vector3df& newpos);

    //! Updates the absolute position based on the relative and the parents position
    /** Note: This does not recursively update the parents absolute positions, so if you have a deeper
        hierarchy you might want to update the parents first.*/
    virtual void updateAbsolutePosition();

    //! Retrieve the scene manager for this node.
    /** \return The node's scene manager. */
    virtual ISceneManager* getSceneManager(void) const;
};

} // end namespace scene
} // end namespace irr


#endif
