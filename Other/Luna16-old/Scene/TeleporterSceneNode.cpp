#include "TeleporterSceneNode.h"
#include <ISceneManager.h>
#include <IVideoDriver.h>
#include <IBillboardTextSceneNode.h>
#ifdef _SOUND
#include <irrKlang.h>
#endif
#include "../core/Configuration.h"
#include "../core/Demo.h"
#include "WhirlSceneNode.h"

//#include <ISceneNodeAnimatorCameraFPS.h>
#include <ISceneNodeAnimatorCollisionResponse.h>

namespace irr
{
namespace scene
{

TeleporterSceneNode::TeleporterSceneNode(
	const Demo* const demo,
	const core::aabbox3d<f32>& placeA,
	const core::aabbox3d<f32>& placeB,
	const bool viceversa, const s32 id) :
	ISceneNode(demo->getSceneManager()->getRootSceneNode(), demo->getSceneManager(), id),
	demo(demo),
	nodesToWatch(nodesToWatch),
	placeA(placeA), placeB(placeB), viceversa(viceversa), dontTeleport(false), dontTeleportTimer(0.0f)
{
#ifdef _DEBUG
	setDebugName("TeleporterSceneNode");
#endif

	this->whirl[0] = new WhirlSceneNode(100, demo->getSceneManager()->getRootSceneNode(), demo->getSceneManager(), -1);
	this->whirl[0]->setPosition(placeA.getCenter());
	this->whirl[0]->setScale(irr::core::vector3df(30.0f, 30.0f, 30.0f));
//	this->whirl[0]->setDebugDataVisible(irr::scene::EDS_BBOX);

	this->whirl[1] = new WhirlSceneNode(100, demo->getSceneManager()->getRootSceneNode(), demo->getSceneManager(), -1);
	this->whirl[1]->setPosition(placeB.getCenter());
	this->whirl[1]->setScale(irr::core::vector3df(30.0f, 30.0f, 30.0f));

	this->Material.Lighting = false;


	this->text[0] = demo->getSceneManager()->addBillboardTextSceneNode(
		demo->getFont(), L"Teleporter", this->whirl[0], irr::core::dimension2df(120, 100), irr::core::vector3df(0, 5, 0));


	this->text[1] = demo->getSceneManager()->addBillboardTextSceneNode(
		demo->getFont(), L"Teleporter", this->whirl[1], irr::core::dimension2df(120, 100), irr::core::vector3df(0, 5, 0));

	this->setAutomaticCulling(irr::scene::EAC_OFF);
}

TeleporterSceneNode::~TeleporterSceneNode()
{
	this->text[0]->remove();
	this->text[1]->remove();

	this->whirl[0]->remove();
	this->whirl[1]->remove();
}

void TeleporterSceneNode::OnRegisterSceneNode()
{
	if (this->IsVisible)
		this->SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

const irr::core::aabbox3d<irr::f32>& TeleporterSceneNode::getBoundingBox() const
{
	return this->Box;
}

irr::u32 TeleporterSceneNode::getMaterialCount() const
{
	return 1;
}

irr::video::SMaterial& TeleporterSceneNode::getMaterial(irr::u32 i)
{
	return this->Material;
}

void TeleporterSceneNode::OnAnimate(irr::u32 timeMs)
{

	if (!this->dontTeleport)
	{
		irr::core::list<irr::scene::ISceneNode*>::Iterator it; //const irr::core::list<irr::scene::ISceneNode*>::ConstIterator it; only works on Linux, mingw on windows doesn't like it
		irr::core::list<irr::scene::ISceneNode*>::Iterator end;
	    for(it = this->nodesToWatch.begin(); it != end; ++it)
	    {
	    	irr::scene::ISceneNode* const node = (*it);

			if (this->placeA.isPointInside(node->getAbsolutePosition()))
			{
				//teleport from A to B
				teleport(node, this->placeB.getCenter());
			}

			else if (this->viceversa && placeB.isPointInside(node->getAbsolutePosition()))
			{
				//teleport from B to A
				teleport(node, this->placeA.getCenter());
			}
	    }
	}
	else
	{
		this->dontTeleportTimer -= this->demo->getElapsed();

		if (this->dontTeleportTimer <= 0)
		{
			this->dontTeleport = false;

			this->whirl[0]->setVisible(true);
			this->whirl[1]->setVisible(true);
		}
	}

	ISceneNode::OnAnimate(timeMs);
}

void TeleporterSceneNode::render()
{
	if (DebugDataVisible & scene::EDS_BBOX)
	{
		irr::video::IVideoDriver* const driver = this->SceneManager->getVideoDriver();

		driver->setMaterial(this->Material);
		driver->setTransform(irr::video::ETS_WORLD, core::matrix4());

		irr::video::SColor color(255, 255, 255, 0);
		if (this->dontTeleport)
			color.setGreen(0);


		irr::core::vector3df edges[8];

		//draw placeA
		driver->draw3DBox(this->placeA, color);

		this->placeA.getEdges(edges);

		const irr::core::vector3df& frontLeftA = edges[2];
		const irr::core::vector3df& frontRightA = edges[6];
		const irr::core::vector3df& rearLeftA = edges[0];
		const irr::core::vector3df& rearRightA = edges[4];
		driver->draw3DLine(frontLeftA, rearRightA, color);
		driver->draw3DLine(rearLeftA, frontRightA, color);

		//draw placeB
		driver->draw3DBox(this->placeB, color);

		this->placeB.getEdges(edges);

		const irr::core::vector3df& frontLeft = edges[2];
		const irr::core::vector3df& frontRight = edges[6];
		const irr::core::vector3df& rearLeft = edges[0];
		const irr::core::vector3df& rearRight = edges[4];
		driver->draw3DLine(frontLeft, rearRight, color);
		driver->draw3DLine(rearLeft, frontRight, color);
	}
}

void TeleporterSceneNode::addNodeToWatchList(irr::scene::ISceneNode* const node)
{
	this->nodesToWatch.push_back(node);
}

void TeleporterSceneNode::removeNodeFromWatchList(const irr::scene::ISceneNode* const node)
{
	irr::core::list<irr::scene::ISceneNode*>::Iterator it = this->nodesToWatch.begin();
	const irr::core::list<irr::scene::ISceneNode*>::Iterator end = this->nodesToWatch.end();
    for(; it != end; ++it)
    {
    	if (node == (*it))
    	{
    		this->nodesToWatch.erase(it);
    		break;
    	}
    }
}

void TeleporterSceneNode::teleport(ISceneNode* const node, const irr::core::vector3df& target)
{
	this->dontTeleport = true;
	this->dontTeleportTimer = 3.0f;

	node->setPosition(target);

	//update animators if any
	const irr::core::list<irr::scene::ISceneNodeAnimator*> anims = node->getAnimators();
	irr::core::list<irr::scene::ISceneNodeAnimator*>::ConstIterator it = anims.begin();
	const irr::core::list<irr::scene::ISceneNodeAnimator*>::ConstIterator end = anims.end();
    for(; it != end; ++it)
    {
    	irr::scene::ISceneNodeAnimator* const anim = (*it);
    	const irr::scene::ESCENE_NODE_ANIMATOR_TYPE& type = anim->getType();

    	switch (type)
    	{
    	case ESNAT_FLY_CIRCLE: break;
    	case ESNAT_FLY_STRAIGHT: break;
    	case ESNAT_FOLLOW_SPLINE: break;
    	case ESNAT_ROTATION: break;
    	case ESNAT_TEXTURE: break;
    	case ESNAT_DELETION: break;
    	case ESNAT_COLLISION_RESPONSE:
    	{
    		ISceneNodeAnimatorCollisionResponse* const col = (ISceneNodeAnimatorCollisionResponse*)anim;
//    		col->setTargetNode(node);
    	}
		break;
    	case ESNAT_CAMERA_FPS:
    	{
    		//ISceneNodeAnimatorCameraFPS* col = (ISceneNodeAnimatorCameraFPS*)anim;
    	}
    	break;

    	case ESNAT_CAMERA_MAYA: break;
    	case ESNAT_UNKNOWN: break;
    	default: break;
    	}

    }

    node->updateAbsolutePosition();

	this->whirl[0]->setVisible(false);
	this->whirl[1]->setVisible(false);

#ifdef _SOUND
	//play teleport sound
	if (demo->getSoundEngine() && demo->getConfiguration()->isSoundEnabled())
	{
		irrklang::ISound* snd = 0;
		snd = demo->getSoundEngine()->play3D("media/sounds/teleport.wav", this->placeA.getCenter(), false, true);
		if (snd)
		{
			snd->setMinDistance(500.0f);
			snd->setIsPaused(false);
			snd->drop();
			snd = 0;
		}

		snd = demo->getSoundEngine()->play3D("media/sounds/teleport.wav", this->placeB.getCenter(), false, true);
		if (snd)
		{
			snd->setMinDistance(500.0f);
			snd->setIsPaused(false);
			snd->drop();
			snd = 0;
		}
	}
#endif
}

} // end namespace scene
} // end namespace irr
