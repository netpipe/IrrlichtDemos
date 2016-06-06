#include "IEmptyBillboardSceneNode.h"

namespace irr
{
namespace scene
{

	//! Constructor
	IEmptyBillboardSceneNode::IEmptyBillboardSceneNode(IBatchedBillboardSceneNode* batchParent, IBillboardSceneNode* node)
            : BatchParent(batchParent), Size(node->getSize()), IsInBatchVisible(node->isVisible()),
            ISceneNode(node->getParent(),batchParent->getSceneManager(),-1,node->getPosition(),node->getRotation(),node->getScale())
    {
        setAutomaticCulling(EAC_OFF);
        setDebugDataVisible(EDS_OFF);
        setVisible(false);
        updateAbsolutePosition();
	    node->getColor(TC,BC);
    }

    //! Destructor
    IEmptyBillboardSceneNode::~IEmptyBillboardSceneNode()
    {
        // delete all children
        removeAll();

        // delete all animators
        ISceneNodeAnimatorList::Iterator ait = Animators.begin();
        for (; ait != Animators.end(); ++ait)
            (*ait)->drop();

        if (TriangleSelector)
            TriangleSelector->drop();

        //BatchParent->removeBillboard(this,true);
    }

	//! Sets the size of the billboard.
	void IEmptyBillboardSceneNode::setSize(const core::dimension2d<f32>& size) {
	    BatchParent->updateBillboardSize(this,size);
	    Size=size;
	    bbox.MaxEdge = core::vector3df(Size.Width*0.5f,Size.Height*0.5f,0.0);
	    bbox.MinEdge = -bbox.MaxEdge;
    }

	//! Set the color of all vertices of the billboard
	/** \param overallColor: the color to set */
	void IEmptyBillboardSceneNode::setColor(const video::SColor & overallColor) {
	    BatchParent->updateBillboardColor(this,overallColor);
        TC = BC = overallColor;
	}

	//! Set the color of the top and bottom vertices of the billboard
	/** \param topColor: the color to set the top vertices
	\param bottomColor: the color to set the bottom vertices */
	void IEmptyBillboardSceneNode::setColor(const video::SColor & topColor, const video::SColor & bottomColor) {
	    BatchParent->updateBillboardColor(this,topColor,bottomColor);
	    TC = topColor;
	    BC = bottomColor;
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
    void IEmptyBillboardSceneNode::OnRegisterSceneNode()
    {
        if (IsInBatchVisible)
        {
            ISceneNodeList::Iterator it = Children.begin();
            for (; it != Children.end(); ++it)
                (*it)->OnRegisterSceneNode();
        }
    }

    //! OnAnimate() is called just before rendering the whole scene.
    /** Nodes may calculate or store animations here, and may do other useful things,
    depending on what they are. Also, OnAnimate() should be called for all
    child scene nodes here. This method will be called once per frame, independent
    of whether the scene node is visible or not.
    \param timeMs Current time in milliseconds. */
    void IEmptyBillboardSceneNode::OnAnimate(u32 timeMs)
    {
        if (IsInBatchVisible)
        {
            // animate this node with all animators

            ISceneNodeAnimatorList::Iterator ait = Animators.begin();
            while (ait != Animators.end())
                {
                // continue to the next node before calling animateNode()
                // so that the animator may remove itself from the scene
                // node without the iterator becoming invalid
                ISceneNodeAnimator* anim = *ait;
                ++ait;
                anim->animateNode(this, timeMs);
            }

            // update absolute position
            updateAbsolutePosition();

            // perform the post render process on all children

            ISceneNodeList::Iterator it = Children.begin();
            for (; it != Children.end(); ++it)
                (*it)->OnAnimate(timeMs);
        }
    }

    //! Removes this scene node from the scene
    /** If no other grab exists for this node, it will be deleted.
    */
    void IEmptyBillboardSceneNode::remove()
    {
        if (Parent)
            Parent->removeChild(this);
        BatchParent->removeBillboard(this);
    }

    //! Returns the material based on the zero based index i.
    /** To get the amount of materials used by this scene node, use
    getMaterialCount(). This function is needed for inserting the
    node into the scene hierarchy at an optimal position for
    minimizing renderstate changes, but can also be used to
    directly modify the material of a scene node.
    \param num Zero based index. The maximal value is getMaterialCount() - 1.
    \return The material at that index. */
    video::SMaterial& IEmptyBillboardSceneNode::getMaterial(u32 num)
    {
        return BatchParent->getMaterial(num);
    }

    //! Get amount of materials used by this scene node.
    /** \return Current amount of materials of this scene node. */
    u32 IEmptyBillboardSceneNode::getMaterialCount() const
    {
        return BatchParent->getMaterialCount();
    }

    //! Sets the relative scale of the scene node.
    /** \param scale New scale of the node, relative to its parent. */
    void IEmptyBillboardSceneNode::setScale(const core::vector3df& scale)
    {
        RelativeScale = scale;
        BatchParent->updateBillboardScale(this,scale);
    }

    //! Sets the rotation of the node relative to its parent.
    /** This only modifies the relative rotation of the node.
    \param rotation New rotation of the node in degrees. */
    void IEmptyBillboardSceneNode::setRotation(const core::vector3df& rotation)
    {
        RelativeRotation = rotation;
        BatchParent->updateBillboardRotation(this,rotation);
    }

    //! Sets the position of the node relative to its parent.
    /** Note that the position is relative to the parent.
    \param newpos New relative position of the scene node. */
    void IEmptyBillboardSceneNode::setPosition(const core::vector3df& newpos)
    {
        BatchParent->updateBillboardPosition(this,newpos-RelativeTranslation);
        RelativeTranslation = newpos;
    }

    //! Updates the absolute position based on the relative and the parents position
    /** Note: This does not recursively update the parents absolute positions, so if you have a deeper
        hierarchy you might want to update the parents first.*/
    void IEmptyBillboardSceneNode::updateAbsolutePosition()
    {
        ISceneNode::updateAbsolutePosition();
        BatchParent->updateBillboardAbsoluteTransformation(this,AbsoluteTransformation);
        //setPosition(core::vector3df(100.f));
    }

    //! Retrieve the scene manager for this node.
    /** \return The node's scene manager. */
    ISceneManager* IEmptyBillboardSceneNode::getSceneManager(void) const { return BatchParent->getSceneManager(); }

} // end namespace scene
} // end namespace irr
