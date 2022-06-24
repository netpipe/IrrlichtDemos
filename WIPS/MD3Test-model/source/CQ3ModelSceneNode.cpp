#include <ISceneManager.h>
#include <IMaterialRenderer.h>
#include <CShadowVolumeSceneNode.h>
#include <irrList.h>
#include <os.h>
#include <IGUIEnvironment.h>
#include "CQ3ModelSceneNode.h"
#include "StageShader.h"
#include "CQ3Surface.h"

namespace irr
{
namespace scene
{

CQ3ModelSceneNode::CQ3ModelSceneNode(IQ3ModelMesh* mesh,
                                     ISceneNode* parent,
                                     ISceneManager* mgr,
                                     s32 id,
                                     const core::vector3df& position,
                                     const core::vector3df& rotation,
                                     const core::vector3df& scale )
    : IQ3ModelSceneNode(parent, mgr, id, position, rotation, scale),
	  Mesh(NULL), BeginFrameTime(0), StartFrame(0), EndFrame(0), CurrentFrameNr(0),
	  FramesPerSecond(25.f / 1000.f), Looping(true), FirstLoop(false), LoopFrames(0), LoopCallBack(0),
	  RenderFromIdentity(false), Shadow(NULL), Bone(NULL), AnimationType(quake3::BOTH_DEATH1),
	  Animations(NULL), Skined(NULL), TimeAbs(0.f)
{
    #ifdef _DEBUG
    setDebugName("CQ3ModelSceneNode");
    #endif

    BeginFrameTime = os::Timer::getTime();

    setMesh(mesh);
}

CQ3ModelSceneNode::~CQ3ModelSceneNode()
{
	if (Mesh)
		Mesh->drop();

	if (LoopCallBack)
		LoopCallBack->drop();

    if (Shadow)
        Shadow->drop();

    if (Animations)
        Animations->drop();

    if (Skined)
        Skined->drop();

    dropMaterials();

}

void CQ3ModelSceneNode::setMesh(IQ3ModelMesh* mesh)
{
 // Check out.
	if (!mesh) {
	    os::Printer::log("Tried to set a null reference to a mesh \"IQ3ModelMesh\" in a node \"IQ3ModelSceneNode\"", ELL_WARNING);
		return; // won't set null mesh
	}

 // Update mesh reference.
	if (Mesh)
		Mesh->drop();
	Mesh = mesh;

 // Get bounding box.
	Box = Mesh->getBoundingBox();

 // Get materials.
	updateMaterials();

 // Get start and begin time.
	setFrameLoop(0, Mesh->getFrameCount());

 // Grab the mesh.
	if (Mesh)
		Mesh->grab();

 // Naming model.
    switch (Mesh->getModelType())
    {
        case MD3_HEAD  : setName("head"  ); break;
        case MD3_UPPER : setName("upper" ); break;
        case MD3_LOWER : setName("lower" ); break;
        case MD3_WEAPON: setName("weapon"); break;
        default: break;
    }
}

void CQ3ModelSceneNode::dropMaterials()
{
    u32 i;

    for (i=0; i<Materials.size(); i++)
    {
        delete Materials[i].shader;
//        if (Materials[i].surface)
//            delete Materials[i].surface;
    }

    Materials.clear();
}

void CQ3ModelSceneNode::updateMaterials()
{
	if (Mesh)
	{
		if (Materials.size() > 0)
		    dropMaterials();

		u32 i;
		for (i=0; i<Mesh->getSurfaceCount(); ++i)
		{
			quake3::Shader * shader = new quake3::Shader( SceneManager, Mesh->getSurface(i)->getMaterial() );
			Materials.push_back( SQ3ModelTextures(shader) );
		}
	}
}


void CQ3ModelSceneNode::setCurrentFrame(u32 frame)
{
	// if you pass an out of range value, we just clamp it
	CurrentFrameNr = core::clamp ( (s32)frame, StartFrame, EndFrame );

	BeginFrameTime = os::Timer::getTime() - (s32)((CurrentFrameNr - StartFrame) / FramesPerSecond);
}


s32 CQ3ModelSceneNode::buildFrameNr(u32 timeMs)
{
	if (StartFrame==EndFrame)
		return StartFrame; //Support for non animated meshes

	if (FramesPerSecond==0.f)
		return StartFrame;

	f32 frame;

	if (Looping)
	{
		// play animation looped
		s32 Start, End;
		Start = StartFrame;
		if (FirstLoop)
		{
		    End = EndFrame;
		    FirstLoop = false;
		}
		else
		    End = Start + LoopFrames - 1;

        if (End - Start == 0)
            return Start;

		s32 lenInMs = core::abs_(s32( (End - Start) / FramesPerSecond ));
		if (FramesPerSecond > 0.f) //forwards...
		{
            frame = Start + ( (timeMs - BeginFrameTime) % lenInMs) * FramesPerSecond;
		}
		else //backwards...
		{
			frame = End - ( (timeMs - BeginFrameTime) % lenInMs)* -FramesPerSecond;
		}
	}
	else
	{
		// play animation non looped

		if (FramesPerSecond > 0.f) //forwards...
		{
			const f32 deltaFrame = ( timeMs - BeginFrameTime ) * FramesPerSecond;

			frame = StartFrame + deltaFrame;

			if (frame > (f32)EndFrame)
			{
				frame = (f32)EndFrame;
				if (LoopCallBack)
					LoopCallBack->OnAnimationEnd((IAnimatedMeshSceneNode *)this);
			}
		}
		else //backwards... (untested)
		{
			const f32 deltaFrame = ( timeMs - BeginFrameTime ) * -FramesPerSecond;

			frame = EndFrame - deltaFrame;

			if (frame < (f32)StartFrame)
			{
				frame = (f32)StartFrame;
				if (LoopCallBack)
					LoopCallBack->OnAnimationEnd((IAnimatedMeshSceneNode *)this);
			}

		}
	}

	s32 iframe = core::floor32(frame);

	if ((u32)iframe >= Mesh->getFrameCount())
	    iframe = Mesh->getFrameCount() - 1;

	return iframe;

}

void CQ3ModelSceneNode::OnAnimate(u32 timeMs)
{
	TimeAbs = f32( timeMs ) * (1.f/1000.f);

	CurrentFrameNr = buildFrameNr ( timeMs );

	if (Mesh)
	{
		scene::IMesh *m = Mesh->getMesh(CurrentFrameNr, 255, StartFrame, EndFrame);
		if ( m )
			Box = m->getBoundingBox();
	}

	IQ3ModelSceneNode::OnAnimate(timeMs);
}


bool CQ3ModelSceneNode::setFrameLoop(s32 begin, s32 end)
{
	const s32 maxFrameCount = Mesh->getFrameCount() - 1;

	if ( end < begin )
	{
	    s32 tmp;
	    tmp = begin;
	    begin = end;
	    end = tmp;
	}

	StartFrame = core::s32_clamp(begin, 0, maxFrameCount);
	EndFrame = core::s32_clamp(end, StartFrame, maxFrameCount);

	setCurrentFrame ( StartFrame );

	LoopFrames = EndFrame - StartFrame + 1;

	FirstLoop = false;

	return true;
}


void CQ3ModelSceneNode::setAnimationSpeed(f32 framesPerSecond)
{
	FramesPerSecond = framesPerSecond * 0.001f;
}


void CQ3ModelSceneNode::setLoopMode(bool playAnimationLooped)
{
	Looping = playAnimationLooped;
	if (Looping)
	    FirstLoop = false;
}


IShadowVolumeSceneNode* CQ3ModelSceneNode::addShadowVolumeSceneNode(s32 id, bool zfailmethod, f32 infinity)
{
	if (!SceneManager->getVideoDriver()->queryFeature(video::EVDF_STENCIL_BUFFER))
		return 0;

	if (Shadow)
	{
		os::Printer::log("This node already has a shadow.", ELL_WARNING);
		return 0;
	}

	Shadow = new CShadowVolumeSceneNode(this, SceneManager, id,  zfailmethod, infinity);
	return Shadow;
}

ISceneNode* CQ3ModelSceneNode::getJointNode(u32 index)
{
    IDummyTransformationSceneNode* dummy;
    dummy = NULL;

    if (index < Mesh->getTagCount())
    {
        dummy = SceneManager->addDummyTransformationSceneNode(this);
        if (dummy)
        {
           SJoinChildSceneNode join;
           join.Index = index;
           join.Node = dummy;
           JointChildSceneNodes.push_back(join);
        }
    }

    return dummy;
}

ISceneNode* CQ3ModelSceneNode::getJointNode(const c8* tagName)
{
    s32 index;

    if ((index = Mesh->getTagIndex(tagName)) < 0)
        return getJointNode((u32)index);
    else
        return NULL;
}

ISceneNode* CQ3ModelSceneNode::getJointNode(EMD3_TAG_TYPE type)
{
    s32 index;

    if ((index = Mesh->getTagIndex(type)) >= 0)
        return getJointNode((u32)index);
    else
        return NULL;
}

ISceneNode* CQ3ModelSceneNode::addJointToNode(IQ3ModelSceneNode* jointNode)
{
    if (jointNode == NULL)
        return NULL;

    IQ3ModelMesh* MeshJoin;
    MeshJoin = (IQ3ModelMesh *) jointNode->getMesh();

    EMD3_MODEL_TYPE joinType;
    joinType = MeshJoin->getModelType();

    EMD3_TAG_TYPE tagType;
    tagType = TAG_UNKNOWN;

    IQ3ModelSceneNode* n1, *n2;
    n1 = NULL; n2 = NULL;

    switch ( Mesh->getModelType() )
    {
        case MD3_HEAD:
        {
            if (joinType == MD3_UPPER)
            {
                tagType = TAG_HEAD;
                n1 = jointNode;
                n2 = this;
            }
            break;
        }
        case MD3_UPPER:
        {
            switch (joinType)
            {
                case MD3_HEAD:
                {
                    tagType = TAG_HEAD;
                    n1 = this;
                    n2 = jointNode;
                    break;
                }
                case MD3_LOWER:
                {
                    tagType = TAG_TORSO;
                    n1 = jointNode;
                    n2 = this;
                    jointNode->setRotation( core::vector3df(-90, 0, 0) );
                    break;
                }
                case MD3_WEAPON:
                {
                    tagType = TAG_WEAPON;
                    n1 = this;
                    n2 = jointNode;
                    break;
                }
                default: break;
            }
            break;
        }
        case MD3_LOWER:
        {
            if (joinType == MD3_UPPER)
            {
                tagType = TAG_TORSO;
                n1 = this;
                n2 = jointNode;
            }
            setRotation( core::vector3df(-90, 0, 0) );
            break;
        }
        case MD3_WEAPON:
        {
            if (joinType == MD3_UPPER)
            {
                tagType = TAG_WEAPON;
                n1 = jointNode;
                n2 = this;
            }
            break;
        }
        default: break;
    }

    ISceneNode* tag;
    tag = NULL;
    if (n1 != NULL && n2 != NULL && (tag = n1->getJointNode(tagType)) != NULL)
    {
        tag->addChild(n2);
        Bone = (n1 == this ? n2 : n1);
    }

    return tag;
}

void CQ3ModelSceneNode::setAnimationConfig(quake3::Animation * animation)
{
    if (animation && animation->isValid())
    {
        if (Animations)
            Animations->drop();
        Animations = animation;
        Animations->grab();
        if (Mesh->getModelType() == MD3_UPPER)
            setAnimation(quake3::TORSO_STAND);
        else if (Mesh->getModelType() == MD3_LOWER)
            setAnimation(quake3::LEGS_WALKCR);
    }
}

void CQ3ModelSceneNode::setAnimation(quake3::EMD3_MODEL_ANIMATION_TYPE animationType)
{
    EMD3_MODEL_TYPE ModelType;
    ModelType = Mesh->getModelType();

 // Check out.
    if (!Animations)
        return;

 // Determinando si la animación es válida.
    const quake3::SMD3_Animation * animation;
    animation = NULL;

    switch (ModelType)
    {
        case MD3_UPPER:
        {
            if ( (animationType >= quake3::TORSO_GESTURE && animationType <= quake3::TORSO_STAND2) ||
                 (animationType >= quake3::TORSO_GETFLAG && animationType <= quake3::TORSO_NEGATIVE) )
                animation = Animations->get(animationType);
            break;
        }

        case MD3_LOWER:
        {
            if ( (animationType >= quake3::LEGS_WALKCR && animationType <= quake3::LEGS_TURN) ||
                 (animationType >= quake3::LEGS_BACKCR && animationType <= quake3::LEGS_BACKWALK) )
                animation = Animations->get(animationType);
            break;
        }

     // MD3_UNKNOWN debe utilizar el método setFrameLoop().
        case MD3_HEAD:     // Lacks animations.
        case MD3_WEAPON:   // You must use the method setFrameLoop().
        case MD3_UNKNOWN:  // You must use the method setFrameLoop().
        default: return;
    }

 // For the animations such as "BOTH ...".
    if (animation == NULL)
    {
        if ( animationType >= quake3::BOTH_DEATH1 && animationType <= quake3::BOTH_DEAD3 )
            animation = Animations->get(animationType);
        else
            return; // Missed when choosing the animation.
    }

 // Estableciendo la animación.
    AnimationType   = animationType;
    StartFrame      = animation->firstFrame;
    EndFrame        = StartFrame + animation->numFrames;
    FramesPerSecond = ( (animation->reversed ? -1 : 1) * animation->fps ) * 0.001f;
    LoopFrames      = animation->loopFrames;
    FirstLoop       = false;
}

void CQ3ModelSceneNode::setSkin(quake3::Skin * skin)
{
    if (skin && skin->isValid() && Mesh)
    {
        Skined = skin;
        Skined->grab();
        Skined->applyTo(Mesh);
        updateMaterials();
    }
}

void CQ3ModelSceneNode::setAnimation(quake3::Animation * animations)
{
    if (animations && animations->isValid())
    {
        Animations = animations;
    }
}

video::SMaterial& CQ3ModelSceneNode::getMaterial(u32 i)
{

	if ( i < Materials.size() )
	    return Materials[i].shader->getMaterial();
    else
        return ISceneNode::getMaterial(i);
}

u32 CQ3ModelSceneNode::getMaterialCount() const
{
	return Materials.size();
}

void CQ3ModelSceneNode::setAnimationEndCallback(IAnimationEndCallBack* callback)
{
	if (LoopCallBack)
		LoopCallBack->drop();

	LoopCallBack = callback;

	if (LoopCallBack)
		LoopCallBack->grab();
}

void CQ3ModelSceneNode::OnRegisterSceneNode()
{
	if ( isVisible() )
	{

	/*
	 * Because this node supports rendering of mixed mode meshes, consisting of
	 * transparent and solid material at the same time, we need to go through all
	 * materials, check of what type they are and register this node for the right
	 * render pass according to that.
	 */

		int transparentCount = 0;
		int solidCount = 0;

	 // Count transparent and solid materials in this scene node.
		u32 i;
		for (i=0; i<Materials.size(); ++i)
		{
			if (Materials[i].shader->isTransparent())
				++transparentCount;
			else
				++solidCount;

			if (solidCount && transparentCount)
				break;
		}

     // Register according to material types counted.
		if (solidCount)
			SceneManager->registerNodeForRendering(this, scene::ESNRP_SOLID);

		if (transparentCount)
			SceneManager->registerNodeForRendering(this, scene::ESNRP_TRANSPARENT);

        ISceneNode::OnRegisterSceneNode();

	}
}

void CQ3ModelSceneNode::normalRender(video::IVideoDriver* driver)
{
	bool isTransparentPass = SceneManager->getSceneNodeRenderPass() == scene::ESNRP_TRANSPARENT;
	bool ZWriteEnable = !isTransparentPass;

	IQ3ModelMesh* mesh = (IQ3ModelMesh*)Mesh->getMesh(CurrentFrameNr, 255, StartFrame, EndFrame);

 	for (u32 i=0; i<mesh->getSurfaceCount(); ++i)
	{
		quake3::Shader *shader = Materials[i].shader;

		if (shader->isTransparent() != isTransparentPass)
		    continue;

        scene::CQ3Surface *surface;
        surface = (scene::CQ3Surface *)mesh->getSurface(i);

        surface->original();
        video::S3DVertex2TCoords *vsrc = (video::S3DVertex2TCoords *) surface->getVertices();

        surface->clone();
        video::S3DVertex2TCoords *vdst = (video::S3DVertex2TCoords *) surface->getVertices();

        core::list<quake3::IStageShader*>& stages = shader->getStages(vsrc, vdst, surface->getVertexCount());

        driver->setTransform(video::ETS_WORLD, AbsoluteTransformation );

        quake3::IStageShader *stage;
        core::list<quake3::IStageShader*>::Iterator it = stages.begin();
        for (; it != stages.end(); ++it)
        {
            stage = *it;
            if ( stage->animate(TimeAbs) )
            {
                if (RenderFromIdentity)
                    driver->setTransform(video::ETS_WORLD, core::matrix4() );
                shader->getStageMaterial().ZWriteEnable= shader->getStageMaterial().ZWriteEnable || ZWriteEnable;
                driver->setMaterial( shader->getStageMaterial() );
                driver->drawMeshBuffer( surface );
            }
            ZWriteEnable = false;
        } // 2st for

	} // 1st for (u32 i=0; i<m->getMeshBufferCount(); ++i)

}

void CQ3ModelSceneNode::debugRender(video::IVideoDriver* driver)
{
 	scene::IMesh* m = (scene::IMesh*) Mesh;
 	video::SMaterial mat;

 // overwrite half transparency
	if ( DebugDataVisible & scene::EDS_HALF_TRANSPARENCY )
	{
	    for (u32 i=0; i<m->getMeshBufferCount(); ++i)
		{
            scene::IMeshBuffer* mb = m->getMeshBuffer(i);
			mat = Materials[i].shader->getMaterial();
			mat.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
			if (RenderFromIdentity)
				driver->setTransform(video::ETS_WORLD, core::matrix4() );
			driver->setMaterial(mat);
			driver->drawMeshBuffer(mb);
		}
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
    }
	mat.Lighting = false;
	driver->setMaterial(mat);

 // show normals
	if ( DebugDataVisible & scene::EDS_NORMALS )
	{
		IAnimatedMesh * arrow = SceneManager->addArrowMesh (
				"__debugnormal", 0xFFECEC00,
				0xFF999900, 4, 8, 1.f, 0.6f, 0.05f,
				0.3f);
		if ( 0 == arrow )
		{
			arrow = SceneManager->getMesh ( "__debugnormal" );
		}
		const IMesh *mesh = arrow->getMesh ( 0 );

		// find a good scaling factor
		core::matrix4 m2;

		// draw normals
		for (u32 g=0; g<m->getMeshBufferCount(); ++g)
		{
			const scene::IMeshBuffer* mb = m->getMeshBuffer(g);
			const u32 vSize = video::getVertexPitchFromType(mb->getVertexType());
			const video::S3DVertex2TCoords* v = ( const video::S3DVertex2TCoords*)mb->getVertices();
			for ( u32 i=0; i != mb->getVertexCount(); ++i )
			{
				// Align to v->normal
				core::quaternion quatRot( v->Normal.Z, 0.f, -v->Normal.X, 1 + v->Normal.Y );
				quatRot.normalize();
				quatRot.getMatrix ( m2 );

				m2.setTranslation(v->Pos);
				m2*=AbsoluteTransformation;

				driver->setTransform(video::ETS_WORLD, m2 );
				for ( u32 a = 0; a != mesh->getMeshBufferCount(); ++a )
					driver->drawMeshBuffer ( mesh->getMeshBuffer ( a ) );

				v = (const video::S3DVertex2TCoords*) ( (u8*) v + vSize );
			}
		}
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	}
	mat.MaterialType = video::EMT_SOLID;
	mat.ZBuffer = false;
	driver->setMaterial(mat);

 // show bounding box
	if ( DebugDataVisible & scene::EDS_BBOX_BUFFERS )
	{
		for (u32 g=0; g< m->getMeshBufferCount(); ++g)
		{
			const IMeshBuffer* mb = m->getMeshBuffer(g);

			driver->draw3DBox( mb->getBoundingBox(),
					video::SColor(0,190,128,128) );
		}
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	}

	if ( DebugDataVisible & scene::EDS_BBOX )
		driver->draw3DBox(Box, video::SColor(0,255,255,255));

 // show mesh
	if ( DebugDataVisible & scene::EDS_MESH_WIRE_OVERLAY )
	{
		mat.Lighting = false;
		mat.Wireframe = true;
		mat.ZBuffer = true;
		driver->setMaterial(mat);

		for (u32 g=0; g<m->getMeshBufferCount(); ++g)
		{
			const IMeshBuffer* mb = m->getMeshBuffer(g);
			if (RenderFromIdentity)
				driver->setTransform(video::ETS_WORLD, core::matrix4() );
			driver->drawMeshBuffer(mb);
		}
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	}
}

void CQ3ModelSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	_IRR_DEBUG_BREAK_IF(!Mesh || !driver) // access violation

 // Drawing shadow.
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	if (Shadow)
		Shadow->setMeshToRenderFrom(Mesh);

 // Debug or not debug, that is the question.
    normalRender(driver);
    if ( DebugDataVisible )
        debugRender(driver);

 // update all dummy transformation nodes
    if (!JointChildSceneNodes.empty())
    {
        core::matrix4* mat;
        for (u32 i=0; i < JointChildSceneNodes.size(); i++)
        {
            mat = Mesh->getMatrixOfJoint(JointChildSceneNodes[i].Index, CurrentFrameNr);
            if (mat)
                JointChildSceneNodes[i].Node->getRelativeTransformationMatrix() = *mat;
        }
    }
}

void showShaderInformation(const quake3::SShader * sshader, const quake3::Shader * shader, const core::stringc surfaceName)
{
    core::stringc s;
    s = "Apply shader ";
    s+= sshader->name;
    s+= " to texture ";
    s+= shader->getMaterial().getTexture(0)->getName();
    s+= " and surface ";
    s+= surfaceName;
    os::Printer::log( s.c_str() );

    os::Printer::log( " " );
    os::Printer::log( "Script:" );
    s = dumpShader(s, sshader );
    os::Printer::log( s.c_str() );

    const core::list<quake3::IStageShader*>& stages = shader->getStages();
    os::Printer::log( " " );
    s = "Total stages: ";
    s+= stages.getSize();
    os::Printer::log( s.c_str() );

    quake3::IStageShader *stage;
    core::list<quake3::IStageShader*>::ConstIterator it = stages.begin();
    for (u32 i=0; it != stages.end(); ++it, ++i)
    {
        stage = *it;

        s= "- Stage ";
        s+= stage->isDrawable() ? "draw " : "no draw ";
        s+= i;
        s+= ": ";
        s+= stage->commands().size();
        s+= " commands, ";
        s+= stage->textures().size();
        s+= " textures.";
        os::Printer::log( s.c_str() );
        for (u32 i=0; i < stage->textures().size(); ++i)
        {
           s = i;
           s+= ": ";
           s+= stage->textures()[i]->getName();
           os::Printer::log( s.c_str() );
        }

    }
}

void CQ3ModelSceneNode::setShader(u32 materialIndex, const quake3::SShader * shader, io::IFileSystem * fileSystem)
{
    if (Mesh && shader && materialIndex < Materials.size())
    {
        Materials[materialIndex].shader->clear();
        Materials[materialIndex].shader->parse(shader, fileSystem);
    #if defined(_DEBUG)
        showShaderInformation(shader, Materials[materialIndex].shader, Mesh->getSurface(materialIndex)->getName());
    #endif
    }
}

void CQ3ModelSceneNode::setShaderFromList(quake3::ShadersParser * shaders, io::IFileSystem * fileSystem)
{
    if (Mesh && shaders && shaders->isValid())
    {
        u32 i;
        for (i=0; i < Materials.size(); i++)
        {
            s32 shaderIndex;
            shaderIndex = shaders->findShaderByTexture( Materials[i].shader->getMaterial().getTexture(0) );
            if (shaderIndex >= 0)
                setShader(i, shaders->get((u32)shaderIndex), fileSystem);
            //if (shaderIndex >= 0 && (u32)shaderIndex < Materials.size() && Materials[shaderIndex].shader->isNull())
            //    setShader(i, shaders->get((u32)shaderIndex), fileSystem);
        }
    }
}

} // namespace irr
} // namespace scene

