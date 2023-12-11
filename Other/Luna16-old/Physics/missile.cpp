//#include "stdafx.h"

#include <iostream>
#include <map>
#include <vector>
#include <list>

#include <btBulletDynamicsCommon.h>
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "BulletSoftBody/btSoftBody.h"

#include "BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkEpa2.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btIDebugDraw.h"
#include "LinearMath/btConvexHull.h"

#include <irrlicht.h>

#pragma comment (lib, "libbulletdynamics.lib")
#pragma comment (lib, "libbulletcollision.lib")
#pragma comment (lib, "libbulletmath.lib")
#pragma comment (lib, "libbulletsoftbody.lib")
#pragma comment (lib, "Irrlicht.lib")

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#define SCALE (10.0)

#define CUBE_HALF_EXTENTS (15.0)

#define PI          (3.14159265358979323846264338327950288)
// �x�̃��W�A���̕ϊ�
#define RAD(c) ((PI * (c)) / 180.0)
#define DEG(c) (((c) * 180.0) / PI)

// bounce�f����������Q�l
void QuaternionToEuler(const btQuaternion quaternion, float euler[3])
{
	btScalar w,x,y,z;
	w=quaternion.getW();
	x=quaternion.getX();
	y=quaternion.getY();
	z=quaternion.getZ();
	double sqw = w*w;
	double sqx = x*x;
	double sqy = y*y;
	double sqz = z*z;
	// yaw = attitude
//	euler[0] = (btScalar) (asin(-2.0 * (x*z - w*y)));
	// pitch = bank
//	euler[1] = (btScalar) (atan2(2.0 * (y*z + w*x), (sqw - sqx - sqy + sqz)));
	// roll = heading
//	euler[2] = (btScalar) (atan2(2.0 * (x*y + w*z), (sqw + sqx - sqy - sqz)));
	// heading
    euler[2] = (irr::f32) (atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw)));
    // bank
    euler[0] = (irr::f32) (atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw)));
    // attitude
    euler[1] = (irr::f32) (asin(-2.0 * (x*z - y*w)));
}

// ����Ŋw�ԃQ�[��3D���w��������Q�l
// Bullet��getEuler�֐��̎g�������悭�킩��Ȃ������̂ŁB�B�B
void MatrixToEuler(const btScalar mat[16], btScalar euler[3])
{
    btScalar m11 = mat[0];
    btScalar m12 = mat[1];
    btScalar m13 = mat[2];
    btScalar m21 = mat[4];
    btScalar m22 = mat[5];
    btScalar m23 = mat[6];
    btScalar m31 = mat[8];
    btScalar m32 = mat[9];
    btScalar m33 = mat[10];

    btScalar h, p, b;
    btScalar sp = -m23;
    printf("sp=%f\n", sp);
    if (sp <= -1.0f) {
        p = -1.570796f; // -pi/2
    } else if (sp >= 1.0) {
        p = 1.570796f; // pi/2
    } else {
        p = asin(sp);
    }

    if (sp > 0.9999f)
    {
        b = 0.0f;
        h = atan2(m13, m33);
    } else {
        h = atan2(m13, m33);
        b = atan2(m21, m22);
    }
    euler[0] = h;
    euler[1] = p;
    euler[2] = b;
}

int mainaaa (void)
{
    // Irrlicht�̏���
    IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2du(640, 480), 16,
        false, true, false, 0);
    device->setWindowCaption(L"Irrlicht + Bullet : RigidBody Demo");
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager *smgr = device->getSceneManager();

    // �J�����̐ݒ�
#if 1
    ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0, 150, 500, -1, 0, 0, false);
    camera->setPosition(core::vector3df(0,100,250));
    camera->setFarValue(10000);
    camera->setTarget(core::vector3df(0, 50, 0));
#else
    ICameraSceneNode *camera = smgr->addCameraSceneNode(0, vector3df(0,200,500),vector3df(0,100,0), -1);
#endif

	// create skybox
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	smgr->addSkyBoxSceneNode(
		driver->getTexture("../media/irrlicht2_up.jpg"),
		driver->getTexture("../media/irrlicht2_dn.jpg"),
		driver->getTexture("../media/irrlicht2_lf.jpg"),
		driver->getTexture("../media/irrlicht2_rt.jpg"),
		driver->getTexture("../media/irrlicht2_ft.jpg"),
		driver->getTexture("../media/irrlicht2_bk.jpg"));
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

    // create light
	scene::ISceneNode* light1 =
		smgr->addLightSceneNode(camera, core::vector3df(100, 100, -100), SColorf(100.0, 100.0, 100.0, 0.0), 1500.0f);

    // RigidBody�̃��b�V���f�[�^���t�@�C������ǂݍ��݁iIrrlicht�p�j
    IAnimatedMesh *cubeMesh = smgr->getMesh("../media/hatyune_one_bone_walk.3ds");
    IAnimatedMeshSceneNode *cubeNode = smgr->addAnimatedMeshSceneNode(cubeMesh, 0, -1, vector3df(0, 250, 0), vector3df(0,0,0), vector3df(1,1,1), false);
    cubeNode->addShadowVolumeSceneNode();
    cubeNode->setScale(core::vector3df(SCALE,SCALE,SCALE));
    cubeNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
//	smgr->setShadowColor(video::SColor(100,0,0,0));
//    cubeNode->setMaterialFlag(video::EMF_LIGHTING, false);

    // Missile
    IAnimatedMesh *missileMesh = smgr->getMesh("../media/missile.obj");
    IAnimatedMeshSceneNode *missileNode = smgr->addAnimatedMeshSceneNode(missileMesh, 0, -1, vector3df(0, 100, 500), vector3df(0, 0, 0), vector3df(1, 1, 1), false);
	// add shadow
	missileNode->addShadowVolumeSceneNode();
	smgr->setShadowColor(video::SColor(100,0,0,0));
    missileNode->setScale(core::vector3df(1,1,1));
    missileNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    // ���̕ӂ����Irrlicht�̃`���[�g���A���̉��������̂܂܎g�p
    // add light 2 (gray)
	scene::ISceneNode* light2 =
		smgr->addLightSceneNode(missileNode, core::vector3df(0,0,15),
		video::SColorf(0.5f, 0.2f, 0.2f, 0.0f), 100.0f);
	// attach billboard to light
	scene::ISceneNode* bill = smgr->addBillboardSceneNode(light2, core::dimension2d<f32>(20, 20));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, driver->getTexture("../media/particlewhite.bmp"));
	// add particle system
	scene::IParticleSystemSceneNode* ps =
		smgr->addParticleSystemSceneNode(false, light2);
	ps->setParticleSize(core::dimension2d<f32>(10.0f, 20.0f));
	// create and set emitter
	scene::IParticleEmitter* em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-3, 0, -3, 3, 1, 3),
		core::vector3df(0.0f,0.0f,0.03f),
		80,100,
		video::SColor(0,255,255,255), video::SColor(0,255,255,255),
		400,1100);
	ps->setEmitter(em);
	em->drop();
	// create and set affector
	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();
	ps->addAffector(paf);
	paf->drop();
	// adjust some material settings
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("../media/fireball.bmp"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

    // �n�ʂ̍쐬�iIrrlicht�p�j
    IAnimatedMesh *planemesh = smgr->addHillPlaneMesh("myHill", dimension2d<f32>(24, 24), dimension2d<u32>(100, 100));
    ISceneNode *q3sn = smgr->addOctTreeSceneNode(planemesh);
//    q3sn->setMaterialFlag(video::EMF_LIGHTING, false);
    q3sn->setMaterialTexture(0, driver->getTexture("../media/wall.jpg"));

    // SoftBody�p���[���h���
    btSoftBodyWorldInfo	m_softBodyWorldInfo;

    // ���[���h�̍L��
    btVector3 worldAabbMin(-10000,-10000,-10000);
    btVector3 worldAabbMax(10000,10000,10000);
    // �v���L�V�̍ő吔�i�Փ˕��̂̂悤�Ȃ��́j
    int maxProxies = 1024;
    // broadphase�̍쐬�iSAP�@�j
    btAxisSweep3* broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);
    m_softBodyWorldInfo.m_broadphase = broadphase;

    // �f�t�H���g�̏Փːݒ�ƃf�B�X�p�b�`���̍쐬
    btDefaultCollisionConfiguration* collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    m_softBodyWorldInfo.m_dispatcher = dispatcher;

    // �Փˉ����\���o
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    // Soft-Rigit���I���E�̍쐬
    btSoftRigidDynamicsWorld* dynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

    // �n�ʂ̏Փˌ`��̍쐬
    btCollisionShape* groundShape = new btBoxShape (btVector3(2000,CUBE_HALF_EXTENTS,2000));
    // �n�ʂ�MotionState�̐ݒ�
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-CUBE_HALF_EXTENTS,0)));
    // �n�ʂ̏�������ݒ�
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    // �n�ʂ̍��̂̍쐬
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    groundRigidBody->setCollisionFlags( groundRigidBody->getCollisionFlags() |  btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );
//    groundRigidBody->setRestitution(1.0f);
    // ���[���h�ɒn�ʂ̍��̂�ǉ�
    dynamicsWorld->addRigidBody(groundRigidBody);

    // RigidBody
    // ���b�V���ł�炸�ɒP���Ƀo�E���f�B���O�{�b�N�X��Bullet�p�`����쐬
    irr::core::aabbox3d<irr::f32> cube = cubeNode->getBoundingBox();
    irr::core::vector3df cubeExtend = cube.getExtent()*SCALE/2.0;
    irr::core::vector3df cubePos = cubeNode->getAbsolutePosition();
    irr::core::vector3df cubeOri = cubeNode->getRotation();
    btCollisionShape *cubeShape = new btBoxShape(btVector3(cubeExtend.X, cubeExtend.Y, cubeExtend.Z));
    btDefaultMotionState* cubeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(cubePos.X, cubePos.Y, -cubePos.Z)));
    btVector3 cubeLocalInertia( 0.0, 0.0, 0.0 );
    cubeShape->calculateLocalInertia( 1.0, cubeLocalInertia );
    btRigidBody::btRigidBodyConstructionInfo cubeRigidBodyCI(1.0, cubeMotionState, cubeShape, cubeLocalInertia);
    btRigidBody* cubeRigidBody = new btRigidBody(cubeRigidBodyCI);
    cubeRigidBody->setCollisionFlags( cubeRigidBody->getCollisionFlags() |  btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );
//    missileRigidBody->setRestitution(1.0f);
    dynamicsWorld->addRigidBody(cubeRigidBody);

    // missile
    // ���b�V���ł�炸�ɒP���Ƀo�E���f�B���O�{�b�N�X��Bullet�p�`����쐬
    irr::core::aabbox3d<irr::f32> box = missileNode->getBoundingBox();
    irr::core::vector3df extend = box.getExtent();
    irr::core::vector3df pos = missileNode->getAbsolutePosition();
    irr::core::vector3df ori = missileNode->getRotation();
    btCollisionShape *missileShape = new btBoxShape(btVector3(extend.X, extend.Y, extend.Z));
    btDefaultMotionState* missileMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(pos.X, pos.Y, -pos.Z)));
    btVector3 localInertia( 0.0, 0.0, 0.0 );
    missileShape->calculateLocalInertia( 1.0, localInertia );
    btRigidBody::btRigidBodyConstructionInfo missileRigidBodyCI(1.0, missileMotionState, missileShape, localInertia);
    btRigidBody* missileRigidBody = new btRigidBody(missileRigidBodyCI);
    missileRigidBody->setCollisionFlags( missileRigidBody->getCollisionFlags() |  btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );
//    missileRigidBody->setRestitution(1.0f);
    dynamicsWorld->addRigidBody(missileRigidBody);

    // �d�͂̐ݒ�
    dynamicsWorld->setGravity(btVector3(0,-10.0,0));

    vector3df force(0,0,0);
    static bool keyDown = false, keyBack = false, pause = true;

    std::cout << "start simulation" << std::endl;
    // �V�~�����[�V�����X�e�b�v
    while(device->run())
    {
        if (!pause)
        {
            // �V�~�����[�V������i�߂�B�Ԋu��60Hz
            dynamicsWorld->stepSimulation(1/60.0f, 10);
        }

        // RigidBody
        if (cubeRigidBody != NULL)
        {
            if (cubeRigidBody->getMotionState())
            {
                // ���̂̏����擾
            	btDefaultMotionState* myMotionState = (btDefaultMotionState*)cubeRigidBody->getMotionState();
                btTransform cubeTransform;
                // ���[���h���W�̎擾�EIrrlicht�֔��f
                myMotionState->getWorldTransform(cubeTransform);
                vector3df m_pos, m_ori;
                m_pos = vector3df(cubeTransform.getOrigin().getX(), cubeTransform.getOrigin().getY(), -cubeTransform.getOrigin().getZ());
                cubeNode->setPosition(m_pos);
                // �p�������擾�EIrrlicht�֔��f
                btQuaternion qt = cubeTransform.getRotation();
                float euler[3];
                // �N�H�[�^�j�I������I�C���[�p�ɕϊ��B���̕ӂ�͉������ł��B
                // Bullet�̃I�C���[�p�̓��W�A���ł����AIrrlicht�̃I�C���[�p�͓x�̂悤�ł��B
                QuaternionToEuler(qt, euler);
                m_ori.set(DEG(-euler[0]), DEG(-euler[1]), DEG(euler[2]));
                cubeNode->setRotation(m_ori);
            }
        }
        // missile
        if (missileRigidBody != NULL)
        {
            if (missileRigidBody->getMotionState())
            {
                // ���̂̏����擾
            	btDefaultMotionState* myMotionState = (btDefaultMotionState*)missileRigidBody->getMotionState();
                btTransform missileTransform;
                // ���[���h���W�̎擾�EIrrlicht�֔��f
                myMotionState->getWorldTransform(missileTransform);
                vector3df m_pos, m_ori;
                m_pos = vector3df(missileTransform.getOrigin().getX(), missileTransform.getOrigin().getY(), -missileTransform.getOrigin().getZ());
                missileNode->setPosition(m_pos);

                btQuaternion qt = missileTransform.getRotation();
                float euler[3];

                QuaternionToEuler(qt, euler);
                m_ori.set(DEG(-euler[0]), DEG(-euler[1]), DEG(euler[2]));
                missileNode->setRotation(m_ori);

                vector3df em_pos = ps->getAbsolutePosition();
                em->setDirection((em_pos-m_pos).normalize()/20.0);

                btMatrix3x3 rot;
                rot.setRotation(qt);

                btVector3 force = rot.getColumn(2)*10.0f;
                missileRigidBody->applyForce(force, btVector3(0,0,0));
            }
        }

        if (keyBack)
        {

            btTransform missileTransform;


            vector3df pos = camera->getAbsolutePosition();

            vector3df target = camera->getTarget() - camera->getAbsolutePosition();
            if (target == vector3df(0,0,0))
            {
                target.set(0, 0, 1);
            }
            target.normalize();

            vector3df up = camera->getUpVector();//.normalize();

//            if (up == target)
       //     {
      //          printf("error\n");
      //      }

            btVector3 rayFrom(pos.X, pos.Y, -pos.Z);

            btVector3 rayForward(target.X, target.Y, -target.Z);
	        btVector3 vertical(up.X, up.Y, -up.Z);
            if (rayForward == vertical)
            {
                vertical = (rayForward+btVector3(0,1,0)).normalize();
            }
	        btVector3 hor;
	        hor = rayForward.cross(vertical);
	        hor.normalize();

	        vertical = hor.cross(rayForward);
	        vertical.normalize();

            btScalar m[16];
            m[0] = hor.getX();
            m[4] = hor.getY();
            m[8] = hor.getZ();
            m[3] = 0;

            m[1] = vertical.getX();
            m[5] = vertical.getY();
            m[9] = vertical.getZ();
            m[7] = 0;

            m[2] = rayForward.getX();
            m[6] = rayForward.getY();
            m[10] = rayForward.getZ();
            m[11] = 0;

            m[12] = rayFrom.getX();
            m[13] = rayFrom.getY();
            m[14] = rayFrom.getZ();
            m[15] = 0;

            // missileTransform.setFromOpenGLMatrix()�łȂ������܂������Ȃ������̂�
            // �I�C���[�p�ɕϊ����Ă���ݒ�i���[���h���W�n�ƃ��[�J�����W�n�̈Ⴂ�H�j
            btScalar euler[3];
            missileTransform.setIdentity();                 // �s��̏�����
            missileTransform.setOrigin(rayFrom);            // �ʒu
            MatrixToEuler(m, euler);                        // �s�񂩂�I�C���[�p���v�Z
            btQuaternion qt(euler[0], euler[1], euler[2]);  // �N�H�[�^�j�I���쐬
            qt.normalize();                                 // �O�̂��ߐ��K��
            missileTransform.setRotation(qt);               // �N�H�[�^�j�I���ɂ��ݒ�
            missileRigidBody->setWorldTransform(missileTransform);  // �~�T�C�����W�n�̐ݒ�
            missileRigidBody->setLinearVelocity(btVector3(0,0,0));  // ���x�̏�����
            missileRigidBody->setAngularVelocity(btVector3(0,0,0)); // �p���x�̏�����
            missileRigidBody->setActivationState(true);             // �X���[�v��ԂɂȂ��Ă�����N����
            missileRigidBody->applyImpulse(rayForward*50.0f, btVector3(0.0, 0.0, 0.0)); // �C���p���X�͂�������
            keyBack = false;
        }
/*
        if (GetAsyncKeyState(KEY_SPACE))
        {
            keyDown = true;
        }
        else if (GetAsyncKeyState(KEY_KEY_ESCAPE))
        {
            break;
        }
        else if (GetAsyncKeyState(KEY_KEY_RETURN))
        {
            pause = !pause;
        }
        else
        {
            if (keyDown)
            {
                keyDown = false;
                keyBack = true;
            }
        }
*/
        driver->beginScene(true, true, SColor(0,200,200,200));
        smgr->drawAll();
        driver->endScene();
    }
    device->drop();

    /* Clean up	*/
    for(int i=dynamicsWorld->getNumCollisionObjects()-1;i>0;i--)
    {
        btCollisionObject*	obj=dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody*		body=btRigidBody::upcast(obj);
        if(body&&body->getMotionState())
        {
            delete body->getMotionState();
        }
        while(dynamicsWorld->getNumConstraints())
        {
            btTypedConstraint*	pc=dynamicsWorld->getConstraint(0);
            dynamicsWorld->removeConstraint(pc);
            delete pc;
        }
        btSoftBody* softBody = btSoftBody::upcast(obj);
        if (softBody)
        {
            dynamicsWorld->removeSoftBody(softBody);
        } else
        {
            dynamicsWorld->removeCollisionObject(obj);
        }
        delete obj;
    }

    delete dynamicsWorld;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;

    return 0;
}
