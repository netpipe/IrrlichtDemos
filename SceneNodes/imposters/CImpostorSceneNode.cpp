
#include "CImpostorSceneNode.h"
#include "ISceneCollisionManager.h"
#include "IVideoDriver.h"
#include "ICameraSceneNode.h"
#include "SViewFrustum.h"
#include "ISceneManager.h"

#include "ITimer.h"

// for debug text
#include "IGUIEnvironment.h"
#include "IGUIFont.h"

#define EDS_IMPOSTOR_STRESS    EDS_MESH_WIRE_OVERLAY
#define EDS_IMPOSTOR_INFO      EDS_MESH_WIRE_OVERLAY


namespace irr
{
namespace scene
{

const u32 MaxSize=256;
const u32 MinSize=256;
const u32 TextureWidth=512;
const f32 Threshold=0.5f; // you must be this tall to ride the render queue

const u32 QueueMaxItemsPerPass=1;
const u32 QueueMaxMS=5;
const u32 MinTime=30;

//#define GENERATE_METHOD_1  // keep view transform when rendering to texture
#define GENERATE_METHOD_2 // target the object when rendering to texture

CImpostorSceneNode::CImpostorSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id, ITimer* timer)
: ISceneNode(parent, mgr, id),
	LocalManager(0), DebugFont(0), Timer(timer),
	// stats
	RenderCount(0), CacheHits(0), CacheMisses(0)
{
	// grab timer
	if (timer)
		timer->grab();

	// are textures flipped?
	YDirection = SceneManager->getVideoDriver()->getDriverType() == video::EDT_OPENGL ? -1.0f : 1.0f;

	// create new scene manager for rendering
	LocalManager = mgr->createNewSceneManager();

	// add the camera for rendering impostors
	LocalManager->addCameraSceneNode();

	// create the work buffer
	WorkTexture =
		LocalManager->getVideoDriver()->addRenderTargetTexture(
			core::dimension2du(TextureWidth, TextureWidth), "ImpostorWorkBuffer");

	// grab the font
	DebugFont = SceneManager->getGUIEnvironment()->getBuiltInFont();
	if (DebugFont)
		DebugFont->grab();

	// turn auto-culling off
	setAutomaticCulling(EAC_OFF);

	// set up the material
	Material.Lighting=false;
	Material.ZWriteEnable=true;
	Material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;

	//Material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;


	//Material.MaterialTypeParam =video::pack_texureBlendFunc(video::EBF_ONE, video::EBF_ONE_MINUS_SRC_ALPHA, video::EMFN_MODULATE_1X);
	//Material.MaterialType = video::EMT_ONETEXTURE_BLEND;


	Material.TextureLayer[0].TextureWrapU = video::ETC_CLAMP;
}

CImpostorSceneNode::~CImpostorSceneNode()
{
	// drop timer
	if (Timer)
		Timer->drop();

	// drop render targets and mesh buffers
	for (u32 i=0; i< Buffers.size(); ++i)
	{
		if (Buffers[i].Texture)
			Buffers[i].Texture->drop();
		if (Buffers[i].MeshBuffer)
			Buffers[i].MeshBuffer->drop();
	}
	// drop workspace texture
	if (WorkTexture)
		WorkTexture->drop();

	// drop scene manager
	if (LocalManager)
		LocalManager->drop();

	// drop debug font
	if (DebugFont)
		DebugFont->drop();
}

s32 CImpostorSceneNode::getTextureSizeFromSurfaceSize(s32 size) const
{
	s32 ts = MinSize;

	while(ts < size && ts < MaxSize)
		ts <<= 1;

	while( ts >= MinSize && (ts > size || ts > MaxSize))
		ts >>= 1;

	return ts;
}

void CImpostorSceneNode::OnAnimate(u32 timeMs)
{
	// animate children
	ISceneNode::OnAnimate(timeMs);
	// process render queue
	doRenderQueue();
}

void CImpostorSceneNode::OnRegisterSceneNode()
{
	if (!IsVisible)
		return;

	RenderCount++;

	// in here we:
	// decide which nodes need updates, add them to the queue
	// process the render queue
	//  allocate textures and mesh buffers if required
	// perform clean up tasks:
	//  garbage collection- free up spaces
	//  reorganise


	// get collision manager
	ISceneCollisionManager* colmgr = SceneManager->getSceneCollisionManager();

	// and the active camera
	ICameraSceneNode* cam = SceneManager->getActiveCamera();
	core::aabbox3df camBB = cam->getViewFrustum()->getBoundingBox();
	u32 now = Timer->getRealTime();

	// loop through all impostor nodes
	u32 i;
	for (i=0; i < Impostors.size(); ++i)
	{
		SNodeLink &Imp = Impostors[i];
		ISceneNode *n = Imp.Node;

		// skip invisible and culled nodes
		if (!n->isVisible() || !camBB.intersectsWithBox(n->getTransformedBoundingBox()))
		{
			//Culled++;
			Imp.IsActive = false;
			continue;
		}

		updatePosAndVector(Imp);

		// now we have the screen position...
		core::rect<s32> r = SceneManager->getVideoDriver()->getViewPort();

		if (!Imp.NewPos.isValid() || !r.isRectCollided(Imp.NewPos) )
		{
			// culled
			// Culled++;
			continue;
		}

		core::dimension2di newSize = Imp.NewPos.getSize();

		// Change in rotation: a.length >= a.dotProduct(b) <= -a.length
		f32 diff = 0;

		// far plane = never update, near plane = always update
		f32 far = cam->getFarValue();
		far *= far;
		f32 dist = Imp.Node->getAbsolutePosition().getDistanceFromSQ(cam->getAbsolutePosition());
		dist = 1.0 - (far/dist); // value between 0 and 1
		diff = dist;
		Imp.Score = dist;

		//s32 a = core::max_<s32>(Imp.NewPos.getWidth(), Imp.NewPos.getHeight());
		//diff *= f32(a);

		bool reRender = diff > Threshold;
		//reRender =true;

		reRender = reRender ||
			( !Imp.IsQueued && Imp.BufferID != -1 &&
			Buffers[Imp.BufferID].SlotSize < getTextureSizeFromSurfaceSize(core::max_<s32>(newSize.Width, newSize.Height)));

		// so small that we don't care about it
		if (newSize.Width < 4 || newSize.Height < 4)
		{
			Imp.IsActive = false;
			// object was culled
			//Culled++;
			continue;
		}

		// too large to fit in a texture slot
		if (newSize.Width > MaxSize || newSize.Height > MaxSize)
		{
			// get rid
			releaseSlot(Imp);
			Imp.IsActive = false;
		}
		else
		{
			Imp.IsActive = true;
		}

		if (Imp.IsActive && Imp.BufferID == -1 && !Imp.IsQueued )
			reRender = true;

		if (Imp.IsActive)
		{
			// impostor replaces the node
			if (reRender && now > Imp.Time + MinTime)
			{

				if (!Imp.IsQueued)
				{
					CacheMisses++;

					Imp.IsQueued = true;

					SRenderQueueItem q;
					q.Node = &Imp;
					//q.Value = val;

					RenderQueue.push_back(q);
				}
				else
				{
					//QueueHits++;
				}
			}
			else
			{
				// don't re-render the impostor texture, only draw it
				CacheHits++;
			}
		}
		if (!Imp.IsActive) // || ( Imp.BufferID == -1 && Imp.IsQueued))
		{
			// original node is visible
			n->OnRegisterSceneNode();
			// cache miss
			CacheMisses++;
		}
	}

	SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT);
}
void CImpostorSceneNode::doRenderQueue()
{

	u32 currentTime = Timer->getRealTime(),
		endTime = currentTime + QueueMaxMS,
		startTime = currentTime;

	RenderQueue.sort();
	s32 j= s32(RenderQueue.size()) - 1;
	s32 queueEnd = core::max_<s32>(-1, j - QueueMaxItemsPerPass);
	for (; j > queueEnd && currentTime <= endTime; --j)
	{
		renderNode(*(RenderQueue[j].Node));
		RenderQueue[j].Node->IsQueued = false;
		currentTime = Timer->getRealTime();
	}
	RenderQueue.set_used(j+1);
}

void CImpostorSceneNode::updatePosAndVector(SNodeLink& Imp)
{
	// get bounding box
	const core::aabbox3df &box = Imp.Node->getBoundingBox();
	const core::matrix4& m = Imp.Node->getAbsoluteTransformation();

	// edges for box calculations
	core::vector3df edges[8];

	// get screen position of box
	box.getEdges(edges);

	u32 j=0;
	for (j=0; j < 8; ++j)
		m.transformVect(edges[j]);

	Imp.NewPos.UpperLeftCorner = Imp.NewPos.LowerRightCorner =
		SceneManager->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(
			edges[0], SceneManager->getActiveCamera());

	for (j=1; j < 8; ++j)
		Imp.NewPos.addInternalPoint(
			SceneManager->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(
				edges[j], SceneManager->getActiveCamera()));

	Imp.NewVec =
	#if defined(GENERATE_METHOD_1)
		SceneManager->getActiveCamera()->getTarget().normalize();
	#elif defined(GENERATE_METHOD_2)
		(Imp.Node->getAbsolutePosition() - SceneManager->getActiveCamera()->getAbsolutePosition()).normalize();
	#endif

	m.rotateVect(Imp.NewVec);
}

void CImpostorSceneNode::renderNode(SNodeLink& Imp)
{
	ISceneNode* n = Imp.Node;

	updatePosAndVector(Imp);

	// remember old viewport and render target
	core::rect<s32> oldView = SceneManager->getVideoDriver()->getViewPort();

	if (!oldView.isRectCollided(Imp.NewPos))
		return;

	ICameraSceneNode* cam = SceneManager->getActiveCamera();
	cam->updateAbsolutePosition();

	core::vector3df camP = cam->getAbsolutePosition();
	f32 distance = camP.getDistanceFrom(n->getTransformedBoundingBox().getCenter());

	// project into screen

	core::vector3df pUL = SceneManager->getSceneCollisionManager()->getRayFromScreenCoordinates(Imp.NewPos.UpperLeftCorner, cam).getVector();
	pUL.setLength(distance);
	core::vector3df pLR = SceneManager->getSceneCollisionManager()->getRayFromScreenCoordinates(Imp.NewPos.LowerRightCorner, cam).getVector();
	pLR.setLength(distance);
	core::vector3df pUR = SceneManager->getSceneCollisionManager()->getRayFromScreenCoordinates(core::position2di(Imp.NewPos.LowerRightCorner.X, Imp.NewPos.UpperLeftCorner.Y), cam).getVector();
	pUR.setLength(distance);
	core::vector3df pLL = SceneManager->getSceneCollisionManager()->getRayFromScreenCoordinates(core::position2di(Imp.NewPos.UpperLeftCorner.X, Imp.NewPos.LowerRightCorner.Y), cam).getVector();
	pLL.setLength(distance);

	Imp.BilPos1 = camP + pUL;
	Imp.BilPos2 = camP + pLR;
	Imp.BilPos3 = camP + pUR;
	Imp.BilPos4 = camP + pLL;

	// translate and scale, but don't rotate
	core::matrix4 invMat = n->getAbsoluteTransformation();
	invMat.makeInverse();
	Imp.BilPos1 *= invMat.getScale();
	invMat.translateVect(Imp.BilPos1);
	Imp.BilPos2 *= invMat.getScale();
	invMat.translateVect(Imp.BilPos2);
	Imp.BilPos3 *= invMat.getScale();
	invMat.translateVect(Imp.BilPos3);
	Imp.BilPos4 *= invMat.getScale();
	invMat.translateVect(Imp.BilPos4);

	Imp.ScreenPos = Imp.NewPos;
	Imp.RotVec = Imp.NewVec;
	video::ITexture* rt = 0; // SceneManager->getVideoDriver()->getRenderTarget();

	// set up the camera and viewport for rendering
	ISceneManager* oldManager = n->getSceneManager();

	// set up the camera
	ICameraSceneNode* cam2= LocalManager->getActiveCamera();

	cam2->setPosition(cam->getAbsolutePosition());

	core::vector3df v = n->getTransformedBoundingBox().getCenter();
	cam2->setTarget(cam->getUpVector());
	cam2->setTarget(cam->getTarget());
	cam2->updateAbsolutePosition();

	f32 scaleW = f32(oldView.getWidth()) / f32(Imp.ScreenPos.getWidth());
	f32 scaleH = f32(oldView.getHeight()) / f32(Imp.ScreenPos.getHeight());

	//f32 transW = f32(Impostors[i].ScreenPos.getWidth()/2) / (f32(oldView.getWidth()) - f32(Impostors[i].ScreenPos.getCenter().X));
	//f32 transH = f32(Impostors[i].ScreenPos.getHeight()/2) / (f32(oldView.getHeight()) - f32(Impostors[i].ScreenPos.getCenter().X));

	f32 transW = (f32(oldView.getCenter().X) - f32(Imp.ScreenPos.getCenter().X)) / f32(Imp.ScreenPos.getWidth());
	f32 transH = (f32(oldView.getCenter().Y) -  f32(Imp.ScreenPos.getCenter().Y)) / f32(Imp.ScreenPos.getHeight());

	core::matrix4 proj(cam->getProjectionMatrix());
	core::matrix4 zoom, trans;

	Imp.Time = Timer->getRealTime();

	zoom.setScale(core::vector3df(scaleW, scaleH, 1.0));
	trans.setTranslation(core::vector3df(transW*2,-transH*2,0.0));


	proj = zoom * proj;
#if defined(GENERATE_METHOD_1)
	proj = trans * proj;
#endif




	// set the correct render target and viewport
	setTarget(Imp);

	// draw the scene
	cam2->setProjectionMatrix(proj);
	E_CULLING_TYPE culltype = EAC_FRUSTUM_BOX;
	n->setAutomaticCulling(EAC_OFF);
	//cam2->render();
//	n->setSceneManager(LocalManager);
	n->OnRegisterSceneNode();
	LocalManager->drawAll();

	n->setAutomaticCulling(culltype);

	//s32 numberzzz = LocalManager->getParameters()->getAttributeAsInt("culled");

	// copy work buffer back
	s32 slot = Buffers[Imp.BufferID].SlotSize;
	//SceneManager->getGUIEnvironment()->getBuiltInFont()->draw(L"HI THERE!", core::rect<s32>(0, 0, slot, slot), video::SColor(), true, true);
	SceneManager->getVideoDriver()->setRenderTarget(Buffers[ Imp.BufferID].Texture, false, true);
	//SceneManager->getVideoDriver()->setRenderTarget(0);



	//LocalManager->getVideoDriver()->setViewPort( core::rect<s32>(0,0,TextureWidth,TextureWidth) );

	s32 d = TextureWidth / slot;
	s32 x = (Imp.SlotID % d) * slot;
	s32 y = (Imp.SlotID / d) * slot;

	//LocalManager->getVideoDriver()->setViewPort( core::rect<s32>(0,0,TextureWidth,TextureWidth) );
/*
	SceneManager->getVideoDriver()->draw2DRectangle(
		video::SColor(127,0,0,0),
		core::rect<s32>(x,y,x+slot,y+slot));
	SceneManager->getVideoDriver()->draw2DRectangle(
		video::SColor(100,0,0,0),
		core::rect<s32>(x,y,x+slot,y+slot));
	SceneManager->getVideoDriver()->draw2DRectangle(
		video::SColor(100,0,0,0),
		core::rect<s32>(x,y,x+slot,y+slot));
*/
	//SceneManager->getVideoDriver()->setTransform

	core::rect<s32> clipRect(x,y, x+slot,y+slot);

	video::SMaterial m;
	m.MaterialTypeParam =video::pack_textureBlendFunc(video::EBF_ONE, video::EBF_ONE_MINUS_DST_ALPHA, video::EMFN_MODULATE_1X);
	m.MaterialType = video::EMT_ONETEXTURE_BLEND;
	m.Lighting=false;
	m.ZBuffer = false;
	m.ZWriteEnable = false;
	m.TextureLayer[0].TextureWrapU = video::ETC_CLAMP;
	m.setTexture(0, WorkTexture);

	video::S3DVertex Vertices[6];

	Vertices[0] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,255,255,255), 0.0f, YDirection);
	Vertices[1] = video::S3DVertex(-1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,255,255,255), 0.0f, 0.0f);
	Vertices[2] = video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,255,255,255), 1.0f, 0.0f);
	Vertices[3] = video::S3DVertex( 1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,255,255,255), 1.0f, YDirection);
	Vertices[4] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,255,255,255), 0.0f, YDirection);
	Vertices[5] = video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,255,255,255), 1.0f, 0.0f);

	const u16 i1[] = {0,1,2,3,4,5};
	const u16 i2[] = {0,2,1,3,5,4};
	const u16* indices = i1;

	core::matrix4 matrix, matrix2;
	if (YDirection == -1.0f)
	{
		matrix2.setScale(core::vector3df(1.0f,-1.0f,1.0f));
		indices = i2;
	}
	LocalManager->getVideoDriver()->setViewPort( clipRect );
	LocalManager->getVideoDriver()->setMaterial(m);
	SceneManager->getVideoDriver()->setTransform(video::ETS_WORLD, matrix);
	SceneManager->getVideoDriver()->setTransform(video::ETS_PROJECTION, matrix2);
	SceneManager->getVideoDriver()->setTransform(video::ETS_VIEW, matrix);
	SceneManager->getVideoDriver()->drawIndexedTriangleList(&Vertices[0], 6, &indices[0], 2);
	LocalManager->getVideoDriver()->setViewPort(core::rect<s32>(0,0,TextureWidth, TextureWidth));

	//SceneManager->getVideoDriver()->draw2DImage(
	//	WorkTexture,
	//	core::position2di(x,y),
	//	core::rect<s32>(0,0,slot, slot),
	//	&clipRect,
	//	video::SColor(255,255,255,255),true);

	if (DebugDataVisible & EDS_IMPOSTOR_INFO && DebugFont)
	{
		core::stringw text;
		video::SColor col(255,255,0,0);
		core::rect<s32> clip(x, y, x+slot, 0);
		s32 third = s32(f32(slot)*0.5f);

		text=L"Buf: "; text+=Imp.BufferID;
		clip.UpperLeftCorner.Y = y+third;
		clip.LowerRightCorner.Y = y+ third*2;
		DebugFont->draw(text.c_str(), clip, col, true, true, &clip);

		text=L"Region: "; text+=Imp.SlotID;
		clip.UpperLeftCorner.Y = y+third*2;
		clip.LowerRightCorner.Y = y+slot;
		DebugFont->draw(text.c_str(), clip, col, true, true, &clip);
	}


	//core::rect<s32> blaView = SceneManager->getVideoDriver()->getViewPort();

	//SceneManager->getVideoDriver()->setTransform(video::ETS_WORLD, n->getAbsoluteTransformation());
	//SceneManager->getVideoDriver()->draw3DBox(n->getBoundingBox());

	// restore states
	SceneManager->getVideoDriver()->setRenderTarget(0, false, false);
	LocalManager->getVideoDriver()->setViewPort(oldView);

//	n->setSceneManager(oldManager);
}

void CImpostorSceneNode::setTarget(SNodeLink& Imp)
{
	s32 realsize = getTextureSizeFromSurfaceSize(
		core::max_<s32>(Imp.ScreenPos.getWidth(), Imp.ScreenPos.getHeight()));

	bool newTex = false;

	if (Imp.BufferID == -1)
	{
		getSlot(Imp, realsize);
	}
	else
	if (Buffers[Imp.BufferID].SlotSize != realsize)
	{
		getSlot(Imp, realsize);
	}

	//printf("setting texture at address %x size=%d\n", Textures[Impostors[i].BufferID].Texture, realsize);
	LocalManager->getVideoDriver()->setRenderTarget(WorkTexture, true, true);
	//LocalManager->getVideoDriver()->setViewPort(core::rect<s32>(0,0,realsize, realsize));
}

void CImpostorSceneNode::releaseSlot(SNodeLink& Imp)
{
	if (Imp.BufferID != -1)
	{
		Buffers[Imp.BufferID].Slots[Imp.SlotID] = 0;
		Buffers[Imp.BufferID].FreeSlots++;
	}
	Imp.BufferID = -1;
	Imp.SlotID = -1;
}

void CImpostorSceneNode::getSlot(SNodeLink &Imp, s32 size)
{
	releaseSlot(Imp);

	bool found=false;
	for (u32 i=0; i < Buffers.size(); ++i)
	{
		if (Buffers[i].SlotSize == size && Buffers[i].FreeSlots > 0)
		{
			found = true;
			// find the first empty one
			s32 search = Buffers[i].Slots.linear_search(0);

			if (search == -1)
				printf("HALP!!! we're all messed up\n");

			Buffers[i].Slots[search] = Imp.Node;
			Buffers[i].FreeSlots--;
			Imp.BufferID = i;
			Imp.SlotID = search;

			break;
		}
	}

	if (!found)
	{
		// need a new texture
		SBufferLink t;
		t.SlotSize = size;
		t.FreeSlots = (TextureWidth / size)*(TextureWidth / size) -1;
		t.Slots.set_used(t.FreeSlots+1);
		t.Slots[0] = Imp.Node;
		t.MeshBuffer = new SMeshBuffer();
		for (u32 j=1; j<t.Slots.size(); ++j)
			t.Slots[j] = 0;

		t.Texture =
			LocalManager->getVideoDriver()->addRenderTargetTexture(
				core::dimension2du(TextureWidth, TextureWidth));
		LocalManager->getVideoDriver()->setRenderTarget(t.Texture); // clear it
		SceneManager->getVideoDriver()->setRenderTarget(0, false, false);
		Imp.BufferID = Buffers.size();
		Imp.SlotID = 0;
		Buffers.push_back(t);
	}
}

void CImpostorSceneNode::addChild(ISceneNode* child)
{
	ISceneNode::addChild(child);

	SNodeLink n;
	n.Node = child;
	n.SlotID = -1;
	n.BufferID = -1;
	n.Color = video::SColor(255,255,255,255);
	n.IsActive = false;

	bool enable = true;
	bool special = false;

	// is this a kind of node we like?
	switch(child->getType())
	{
	case ESNT_WATER_SURFACE:
	case ESNT_SKY_BOX:
	case ESNT_SHADOW_VOLUME:
	case ESNT_EMPTY:
	case ESNT_DUMMY_TRANSFORMATION:
	case ESNT_PARTICLE_SYSTEM:
	case ESNT_CAMERA:
	case ESNT_CAMERA_MAYA:
	case ESNT_CAMERA_FPS:
		// reject these: add as disabled node
		return;

	case ESNT_LIGHT:
	case ESNT_BILLBOARD:
	case ESNT_ANIMATED_MESH:
		// todo: special cases
		special = true;
		break;

	case ESNT_CUBE:
	case ESNT_SPHERE:
	case ESNT_TEXT:
	case ESNT_OCTREE:
	case ESNT_MESH:
	case ESNT_TERRAIN:
	case ESNT_MD3_SCENE_NODE:
	case ESNT_UNKNOWN:
	default:
		// accept
		break;
	}

	// add a link to this node

	if (special)
	{
		printf("Special nodes not supported yet, ignored\n");
	}
	else
	if (enable)
	{
		// add to normal nodes
		printf("Node added\n");
		Impostors.push_back(n);
	}
	else
	{
		// add to disabled nodes
		printf("Ignoring disabled nodes for now\n");
	}
}

bool CImpostorSceneNode::removeChild(ISceneNode* child)
{
	// remove this child
	SNodeLink n;
	n.Node = child;
	s32 p = Impostors.linear_search(n);
	if (p!=-1)
	{
		// remove from texture slot
		if (Impostors[p].BufferID != -1 && Impostors[p].SlotID != -1)
			Buffers[ Impostors[p].BufferID ].Slots[ Impostors[p].SlotID ] = 0;
		// remove it
		Impostors.erase(p);
	}

	return ISceneNode::removeChild(child);
}

void CImpostorSceneNode::render()
{
	// get camera pos
	ICameraSceneNode* cam=SceneManager->getActiveCamera();
	core::vector3df camPos = cam->getAbsolutePosition();
	// driver
	video::IVideoDriver* Driver = SceneManager->getVideoDriver();

	// all aboard the node sorter!
	NodeSorter.set_used(0);
	u32 i=0;
	for (; i < Impostors.size(); ++i)
	{
		core::aabbox3df bbox = Impostors[i].Node->getTransformedBoundingBox();
		if (i==0)
			Box.reset(bbox.getCenter());

		Box.addInternalBox(bbox);

		if (Impostors[i].IsActive && Impostors[i].BufferID != -1)
		{
			// debug data
			if (DebugDataVisible & EDS_BBOX_BUFFERS)
			{
				video::SMaterial material;
				material.Lighting=false;
				material.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
				s32 col = Impostors[i].Color.getAlpha();
				material.DiffuseColor = video::SColor(255, col, col, col);

				Driver->setMaterial(material);
				Driver->setTransform(video::ETS_WORLD, Impostors[i].Node->getAbsoluteTransformation());
				Driver->draw3DBox(Impostors[i].Node->getBoundingBox(), Impostors[i].IsQueued ? video::SColor(255,255,0,0) : video::SColor(255,0,255,0) );
			}
			// todo: use real centre
			SNodeSorter n;
			n.NodeID = i;
			n.Position = bbox.getCenter();
			n.Distance = camPos.getDistanceFrom(n.Position);

			NodeSorter.push_back(n);
		}
	}
	// sort them
	NodeSorter.sort();

	// clear mesh buffers
	reallocateBuffers();

	// add to mesh buffers

	// fill buffers

	Positions.set_used(Buffers.size());
	for (i=0; i < Positions.size(); ++i)
		Positions[i] = 0;

	for (i=0; i < NodeSorter.size(); ++i)
	{
		SNodeLink &Imp = Impostors[NodeSorter[i].NodeID];
		u32 no = Imp.BufferID;
		SMeshBuffer& Buffer = *(Buffers[no].MeshBuffer);
		u32 v = Positions[no];

		// tcoords
		f32 slot = f32(Buffers[no].SlotSize) / f32(TextureWidth);
		s32 d = TextureWidth / Buffers[no].SlotSize;
		f32 x = f32(Imp.SlotID % d) * slot;
		f32 y = f32(Imp.SlotID / d) * slot;


		// normal
		core::vector3df view(NodeSorter[i].Position - camPos);
		view.normalize();
		view *= -1.0f;

		// position
		Buffer.Vertices[0+v].Pos = Imp.BilPos2;
		Buffer.Vertices[1+v].Pos = Imp.BilPos3;
		Buffer.Vertices[2+v].Pos = Imp.BilPos1;
		Buffer.Vertices[3+v].Pos = Imp.BilPos4;

		core::matrix4 matrix = Imp.Node->getAbsoluteTransformation();
		matrix.transformVect(Buffer.Vertices[0+v].Pos);
		matrix.transformVect(Buffer.Vertices[1+v].Pos);
		matrix.transformVect(Buffer.Vertices[2+v].Pos);
		matrix.transformVect(Buffer.Vertices[3+v].Pos);

		// colour
		video::SColor bilCol = Imp.Color;

		if (DebugDataVisible & EDS_IMPOSTOR_STRESS)
		{
			bilCol = Imp.IsQueued ? video::SColor(255,255,0,0) : video::SColor(255,0,255,0);
		}


		// everything else

		Buffer.Vertices[0+v].Normal = view;
		Buffer.Vertices[0+v].TCoords.set(x+slot, y+slot);
		Buffer.Vertices[0+v].Color = bilCol;

		Buffer.Vertices[1+v].Normal = view;
		Buffer.Vertices[1+v].TCoords.set(x+slot, y);
		Buffer.Vertices[1+v].Color = bilCol;

		Buffer.Vertices[2+v].Normal = view;
		Buffer.Vertices[2+v].TCoords.set(x, y);
		Buffer.Vertices[2+v].Color = bilCol;

		Buffer.Vertices[3+v].Normal = view;
		Buffer.Vertices[3+v].TCoords.set(x, y+slot);
		Buffer.Vertices[3+v].Color = bilCol;

		Positions[no] += 4;
	}
	for (i=0; i<Buffers.size(); ++i)
	{
		u32 quadCount = Positions[i] / 4;
		if (quadCount < Buffers[i].MeshBuffer->Vertices.size() / 4)
			Buffers[i].MeshBuffer->Vertices.set_used(Positions[i]);
		if (quadCount < Buffers[i].MeshBuffer->Indices.size() / 6)
			Buffers[i].MeshBuffer->Indices.set_used(quadCount*6);
	}

	// everyone get onboard the buffer sorter..
	BufferSorter.set_used(0);
	for (i=0; i < Buffers.size(); ++i)
	{
		if (Buffers[i].FreeSlots == Buffers[i].Slots.size())
			continue;
		SBufferSorter bs;
		bs.BufferID = i;
		bs.ItemCount = Buffers[i].Slots.size() - Buffers[i].FreeSlots;
		bs.SlotSize = Buffers[i].SlotSize;
		BufferSorter.push_back(bs);
	}
	BufferSorter.sort();

	// set up the driver for drawing
	core::matrix4 m; // identity
	Driver->setTransform(video::ETS_WORLD, m);


	//printf("Drawing %d buffers\n", BufferSorter.size());
	// and draw the buffers in order
	for (i=0; i<BufferSorter.size(); ++i)
	{
		Material.setTexture(0, Buffers[BufferSorter[i].BufferID].Texture);
		//Material.setTexture(0, WorkTexture);
		Driver->setMaterial(Material);
		Driver->drawMeshBuffer(Buffers[BufferSorter[i].BufferID].MeshBuffer);

		if (DebugDataVisible & EDS_MESH_WIRE_OVERLAY)
		{
			Material.Wireframe = true;
			Material.setTexture(0, 0);
			Driver->setMaterial(Material);
			Driver->drawMeshBuffer(Buffers[BufferSorter[i].BufferID].MeshBuffer);

			Material.Wireframe = false;
		}
	}

	if (DebugDataVisible & EDS_BBOX)
	{
		video::SMaterial material;
		material.Lighting=false;
		material.DiffuseColor = video::SColor(255,0,0,255);

		Driver->setMaterial(material);
		Driver->draw3DBox(Box);
	}

}

void CImpostorSceneNode::reallocateBuffers()
{
	for (u32 i=0; i < Buffers.size(); ++i)
	{
		SMeshBuffer& Buffer = *(Buffers[i].MeshBuffer);
		u32 Size = Buffers[i].Slots.size() - Buffers[i].FreeSlots;

		if (Size * 4 != Buffer.getVertexCount() ||
			Size * 6 != Buffer.getIndexCount())
		{
			u32 oldSize = Buffer.getVertexCount();
			Buffer.Vertices.set_used(Size * 4);

			u32 j;

			// fill remaining indices
			u32 oldIdxSize = Buffer.getIndexCount();
			u32 oldvertices = oldSize;
			Buffer.Indices.set_used(Size * 6);

			for (j=oldIdxSize; j<Buffer.Indices.size(); j+=6)
			{
				Buffer.Indices[0+j] = 0+oldvertices;
				Buffer.Indices[1+j] = 2+oldvertices;
				Buffer.Indices[2+j] = 1+oldvertices;
				Buffer.Indices[3+j] = 0+oldvertices;
				Buffer.Indices[4+j] = 3+oldvertices;
				Buffer.Indices[5+j] = 2+oldvertices;
				oldvertices += 4;
			}
		}
	}
}

void CImpostorSceneNode::flattenBox(const core::aabbox3df& box)
{

	core::matrix4 m;
	core::vector3df cpos   = SceneManager->getActiveCamera()->getAbsolutePosition();
	core::vector3df target = box.getCenter();
	core::vector3df vector = (cpos-target).normalize();

	core::plane3df plane(target+(box.getExtent()*vector), vector);
	m.buildShadowMatrix(cpos, plane);

}

const irr::core::aabbox3df& CImpostorSceneNode::getBoundingBox() const
{
	return Box;
}

void CImpostorSceneNode::setDebugFont(gui::IGUIFont* newFont)
{
	if (DebugFont)
		DebugFont->drop();
	DebugFont = newFont;
	if (DebugFont)
		DebugFont->grab();
}

// fills attributes with statistics information
void CImpostorSceneNode::getStats( io::IAttributes* statistics)
{
	statistics->setAttribute("Nodes", s32(Impostors.size()));
	//statistics->setAttribute("Lights", 0 ); // todo
	statistics->setAttribute("Buffers", s32(Buffers.size()));
	s32 TotalSlots=0, UsedSlots=0;
	for (u32 i=0; i<Buffers.size(); ++i)
	{
		TotalSlots+= Buffers[i].Slots.size();
		UsedSlots += Buffers[i].Slots.size() - Buffers[i].FreeSlots;
	}
	statistics->setAttribute("TotalSlots", TotalSlots);
	statistics->setAttribute("UsedSlots", UsedSlots);
	statistics->setAttribute("SlotUsage%", f32(UsedSlots)/ f32(TotalSlots) * 100.0f);

	statistics->setAttribute("TotalTextureMB", f32(Buffers.size()*TextureWidth*TextureWidth*4)/1048576.0f );


	statistics->setAttribute("QueueLength", s32(RenderQueue.size()));
	//statistics->setAttribute("AverageQueueLength", 0); // todo

	// todo
	statistics->setAttribute("RenderCount", RenderCount);
	statistics->setAttribute("CacheHit%", (f32(CacheHits) / f32(CacheHits + CacheMisses)) * 100.0f );
	statistics->setAttribute("CacheHits", CacheHits);
	statistics->setAttribute("CacheMisses", CacheMisses);

	//statistics->setAttribute("LastRegisterTime", 0);
	//statistics->setAttribute("TotalRegisterTime", 0);
	//statistics->setAttribute("AvgRegisterTime", 0);

	//statistics->setAttribute("LastRenderTime", 0);
	//statistics->setAttribute("TotalRenderTime", 0);
	//statistics->setAttribute("AvgRenderTime", 0);
}

} // scene
} // irr
