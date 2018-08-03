/*
 * irrik.cpp
 *
 *  Created on: 13-12-2011
 *      Author: Marcin Załuski
 */

#include <irrlicht.h>
#include "Target.h"
#include "Input.h"
#include <iostream>

#define RED video::SColor(255, 255, 50, 50)
#define GREEN video::SColor(255, 50, 255, 50)
#define BLUE video::SColor(255, 50, 50, 255)
#define VEC0 core::vector3df(0, 0, 0)
#define BONELABEL 1234
void printvec(core::vector3df const &vec, std::string str = "Vec") {
	std::cout << str.c_str() << " X: " << vec.X << " Y: " << vec.Y << " Z: "
			<< vec.Z << std::endl;
}
void limitDOF(core::vector3df &eulers, scene::IBoneSceneNode &bone) {
	if ((bone.getBoneName()[5]) == '2' && (bone.getBoneName()[6] == '2')) {
		eulers.Z = 0;
		eulers.Y = 0;

		if (eulers.X > 15)
			eulers.X = 15;
		if (eulers.X < -160)
			eulers.X = -160;
	}
}

float ccd_angle(core::vector3df vecE, core::vector3df vecT) {
	float angle = 0;
	core::vector3df tmp;

	vecE.X = 0;
	vecT.X = 0;

	angle = vecE.dotProduct(vecT); //normalized, no need for dividing

	if (angle < 1) {
		tmp = vecT.crossProduct(vecE);
		angle = core::RADTODEG * acos(angle);
		if (angle > 50)
			angle = 30; //one iteration limit
		if (tmp.X > 0) {

			angle = -1 * angle;
		}
	}
	return angle;

}

void anim_ccd3(scene::IBoneSceneNode &effector, // k-chain end-effector node
		scene::IBoneSceneNode &bone, // bone to adjust
		core::vector3df target, // target to reach
		int steps, // how many parent bones adjust
		video::IVideoDriver *drv, // drv for debug lines
		bool inner = false) {

	core::vector3df vecE, vecT, tmp, eulers;
//	float angle;
	scene::IBoneSceneNode *parent;
	drv->setTransform(video::ETS_WORLD, core::IdentityMatrix);

	//get proper vectors
	tmp = bone.getAbsolutePosition(); //get abs pos to calculate vector to target
	vecE = effector.getAbsolutePosition() - tmp; //get effector pos relative to the bone
	vecT = target - tmp; //get target pos relative to the bone

	vecE.X = 0;
	vecT.X = 0;

	printvec(tmp, "bone");
	printvec(vecE, "vecE");
	printvec(vecE + tmp, "Effector");
	printvec(vecT, "vecT");
	printvec(vecT + tmp, "Target");
	vecE.normalize();
	vecT.normalize();

	eulers.X = ccd_angle(vecE, vecT);

	if ((bone.getBoneName()[5]) == '2' && (bone.getBoneName()[6] == '2')
			&& (eulers.X > 15)) {
		eulers.X = 0;
	}

	eulers.Y = 0;
	eulers.Z = 0;
	std::cout << eulers.X << " " << tmp.X << std::endl;

	bone.setRotation(eulers);
	parent = (scene::IBoneSceneNode*) bone.getParent();
	parent->updateAbsolutePositionOfAllChildren();
	if (steps > 0 && !inner) {
		anim_ccd3(effector, *parent, target, steps - 1, drv, true);
	} else
		return;

}

void anim_ccd1(scene::IBoneSceneNode &effector, // k-chain end-effector node
		scene::IBoneSceneNode &bone, // bone to adjust
		core::vector3df target, // target to reach
		int steps, // how many parent bones adjust
		video::IVideoDriver *drv, // drv for debug lines
		bool inner = false) {

	core::vector3df vecE, vecT, tmp, eulers;
	float angle;
	scene::IBoneSceneNode *parent;
	drv->setTransform(video::ETS_WORLD, core::IdentityMatrix);

//get proper vectors
	tmp = bone.getAbsolutePosition(); //get abs pos to calculate vector to target
	vecE = effector.getAbsolutePosition() - tmp; //get effector pos relative to the bone
	vecT = target - tmp; //get target pos relative to the bone

	vecE.X = 0;
	vecT.X = 0;

//	printvec(tmp, "bone");
//	printvec(vecE, "vecE");
//	printvec(vecE + tmp, "Effector");
//	printvec(vecT, "vecT");
//	printvec(vecT + tmp, "Target");

	vecE.normalize();
	vecT.normalize();

	angle = vecE.dotProduct(vecT); //normalized, no need for dividing

	eulers.X = 0;

	if (angle < 1) {
		tmp = vecT.crossProduct(vecE);
		angle = core::RADTODEG * acos(angle);
		if (tmp.X > 0) {

			eulers.X = -1 * angle;
		} else {
			if ((bone.getBoneName()[5]) == '2' && (bone.getBoneName()[6] == '2')
					&& (angle > 15)) {
				eulers.X = 0;
			} else
				eulers.X = angle;
		}

	}

	eulers.Y = 0;
	eulers.Z = 0;
//	std::cout << eulers.X << " " << tmp.X << std::endl;

	bone.setRotation(bone.getRotation() + eulers);
	parent = (scene::IBoneSceneNode*) bone.getParent();
	parent->updateAbsolutePositionOfAllChildren();
	if (steps > 0 && !inner) {
		anim_ccd1(effector, *parent, target, steps - 1, drv, true);
	} else
		return;

}

void anim_ccd(scene::IBoneSceneNode &effector, // k-chain end-effector node
		scene::IBoneSceneNode &bone, // bone to adjust
		core::vector3df target, // target to reach
		int steps, // how many bones to involve
		video::IVideoDriver *drv, // drv for debug lines
		bool inner = false) {
	drv->setTransform(video::ETS_WORLD, core::IdentityMatrix);

	core::vector3df vecE, vecT, vec3, test, eulers;
	scene::IBoneSceneNode *parent;
//	float angle;

	vec3 = bone.getAbsolutePosition(); //get abs pos to calculate vector to target
	vecT = target - vec3;
	vecE = effector.getAbsolutePosition() - vec3;

//	vec3.normalize();
	vecT.normalize();
	vecE.normalize();

	if ((bone.getBoneName()[5]) == '2' && (bone.getBoneName()[6] == '2')) {
		// 1DOF for a knee
		vecE.X = 0;
		vecT.X = 0;

		eulers.X = core::RADTODEG * acos(vecE.dotProduct(vecT));

		if (vecE.crossProduct(vecT).X > 0 && eulers.X > 15)
			eulers.X = 15;
		else
			eulers.X *= -1;

		drv->draw3DLine(VEC0, vecT.crossProduct(vecE) * 50,
				video::SColor(205, 250, 250, 255));
		eulers.Y = 0;
		eulers.Z = 0;

	} else {

		core::quaternion quat;
		quat.rotationFromTo(vecE, vecT);
		quat.toEuler(eulers);

		eulers *= core::RADTODEG;

	}
//		std::cout << i << " step " << steps << " " << bone.getBoneName()
//				<< " euler X: " << eulers.X << std::endl;

//	bone.setRotation(eulers);
	bone.setRotation(bone.getRotation() + eulers);

	parent = (scene::IBoneSceneNode*) bone.getParent();
	parent->updateAbsolutePositionOfAllChildren();
	video::SMaterial material;
	material.setFlag(video::EMF_LIGHTING, false);

	drv->setMaterial(material);
	drv->setTransform(video::ETS_WORLD, core::IdentityMatrix);

	drv->draw3DLine(vec3, vecE, RED);
	drv->draw3DLine(vec3, effector.getAbsolutePosition(), GREEN);
	drv->draw3DLine(vec3, target, GREEN);
	drv->draw3DLine(vec3, bone.getAbsolutePosition(),
			video::SColor(205, 150, 150, 255));

	drv->draw3DLine(VEC0, vecE, RED);
	drv->draw3DLine(VEC0, vecT, GREEN);
	drv->draw3DLine(VEC0, effector.getAbsolutePosition(), GREEN);

	drv->draw3DLine(VEC0, target, GREEN);
//	drv->draw3DLine(VEC0, vec3, video::SColor(255, 255, 255, 255));

	if (steps > 0) {
		anim_ccd(effector, *parent, target, steps - 1, drv, true);
	} else {
		return;
	}

}

void boneLabels(scene::ISceneManager *scene, gui::IGUIFont* font,
		scene::IBoneSceneNode *root) {

	scene::IBoneSceneNode *child;

	core::list<scene::ISceneNode*>::ConstIterator begin =
			root->getChildren().begin();
	core::list<scene::ISceneNode*>::ConstIterator end =
			root->getChildren().end();
	for (; begin != end; ++begin) {
		child = (scene::IBoneSceneNode *) (*begin);
		std::cout << "joint name: " << child->getName() << std::endl;
		boneLabels(scene, font, child);
	}
	core::stringw str(root->getName());
	scene->addBillboardTextSceneNode(font, str.c_str(), root,
			core::dimension2d<f32>(10.0f, 4.0f), VEC0, BONELABEL);
}

void stairsclimb(scene::IAnimatedMeshSceneNode* node, IrrlichtDevice *dev) {

	static int frame = -1;
	static Target target(dev, 0, 0, 0);
	int speed = 50;
	core::vector3df vec(0, 0, 0), vec1(0,0,0);
//	std::cout << "FRAME: " << frame << std::endl;

	video::IVideoDriver *drv = dev->getVideoDriver();

	scene::IBoneSceneNode *e1 = node->getJointNode("Joint23");
	scene::IBoneSceneNode *e2 = node->getJointNode("Joint19");
	static scene::IBoneSceneNode *e,*f;

	if(frame == -1){
		target.show();
		e = e1;
		f = e2;
	}
	if(frame < (1*speed)) {

		vec = e->getAbsolutePosition();
		vec.Y+=0.5;
		vec.Z+=2;
		vec1=f->getAbsolutePosition();
		vec1.Z+=1;
//		printvec(vec,"V1");
	}

	if (frame >= (1*speed) && frame < (2*speed)) {

		vec = e->getAbsolutePosition();
		vec.Y-=0.5;
		vec.Z-=2;
		vec1=f->getAbsolutePosition();
		vec1.Z+=1;
//		printvec(vec,"V2");
	}

	if (frame >= (2*speed) && frame < (3*speed)) {
		vec = e->getAbsolutePosition();
		vec = f->getAbsolutePosition();
//		vec = node->getAbsolutePosition();
//		printvec(vec,"V3");
	}

	if(frame >= (3*speed)){
//		printvec(vec,"V4");
		frame = 0;

		e = (e==e1) ? e2 : e1;
		f = (f==e1) ? e2 : e1;
		vec = e->getAbsolutePosition();
		vec = f->getAbsolutePosition();
	}


	anim_ccd1(*e, (scene::IBoneSceneNode &) *(e->getParent()), vec, 1,
			drv);
	anim_ccd1(*f, (scene::IBoneSceneNode &) *(f->getParent()), vec1, 1,
			drv);

	++frame;
	target.setPosition(vec);

}
int main() {

	Input input;
	//create irrlicht opengl device
	IrrlichtDevice *dev = createDevice(video::EDT_OPENGL,
			core::dimension2d<u32>(800, 450), 32, false, false, false, &input);
	if (!dev)
		return EXIT_FAILURE;

	scene::ISceneManager *scene = dev->getSceneManager();
	video::IVideoDriver *drv = dev->getVideoDriver();

	//get maya like camera
	scene::ICameraSceneNode* camera = scene->addCameraSceneNodeMaya(NULL, 150,
			500, 150, 1, 1);
	camera->setTarget(core::vector3df(50, 50, -30));
	//load an q3 map
	dev->getFileSystem()->addZipFileArchive("media/map-20kdm2.pk3");
	scene::IAnimatedMesh* q3map = scene->getMesh("20kdm2.bsp");
	scene::IMeshSceneNode* q3mapNode = scene->addOctreeSceneNode(
			q3map->getMesh(0));
	q3mapNode->setPosition(core::vector3df(-1400, -70, -1400));

	scene::IAnimatedMeshSceneNode* node = 0;

	scene::ITriangleSelector* selector = 0;
	selector = scene->createOctreeTriangleSelector(q3mapNode->getMesh(),
			q3mapNode, 128);
	q3mapNode->setTriangleSelector(selector);
	if (!selector)
		return EXIT_FAILURE;
	video::SMaterial material;

	scene::ISceneNodeAnimator* anim = NULL;

// Load a zobie model
	scene::IAnimatedMeshSceneNode* node1 = 0;

	node1 = scene->addAnimatedMeshSceneNode(
			scene->getMesh("media/zombie/zombie.b3d"));
	node1->setScale(core::vector3df(7, 7, 7));
	node1->setPosition(core::vector3df(0, 90, 150));
	node1->setRotation(core::vector3df(0, -0, 0));
	node1->getMaterial(0).NormalizeNormals = true;
	node1->setFrameLoop(2, 18);
//	node1->setJointMode(scene::EJUOR_CONTROL);
//	node1->setDebugDataVisible(
//			(scene::E_DEBUG_SCENE_TYPE) (node1->isDebugDataVisible()
//					^ scene::EDS_SKELETON));
	node1->setMaterialFlag(video::EMF_LIGHTING, false);
	node1->setAnimationSpeed(10);
	const core::aabbox3d<f32>& box = node1->getBoundingBox();
	core::vector3df radius = box.MaxEdge - box.getCenter();
	anim = scene->createCollisionResponseAnimator(selector, node1, radius,
			core::vector3df(0, -10, 0), core::vector3df(0, -9, 5));
	node1->addAnimator(anim);

	anim->drop();

//	anim = scene->createFlyStraightAnimator(core::vector3df(0, 0, -150),
//			core::vector3df(0, 0, -350), 4000, true);
//	if (anim) {
//		node1->addAnimator(anim);
//		anim->drop();
//	}

// Load a zobie model`
	node = scene->addAnimatedMeshSceneNode(
			//scene->getMesh("media/zombie/zombie.b3d"));
            scene->getMesh("media/zombie/zombie01.ms3d"));
	node->setScale(core::vector3df(7, 7, 7));
	node->setPosition(core::vector3df(0, 50, -300));
	node->getMaterial(0).NormalizeNormals = true;
	node->setJointMode(scene::EJUOR_CONTROL);
	node->setMaterialFlag(video::EMF_LIGHTING, false);

	const core::aabbox3d<f32>& box1 = node->getBoundingBox();
	radius = box1.MaxEdge - box1.getCenter();
	anim = scene->createCollisionResponseAnimator(selector, node, radius,
			core::vector3df(0, -10, 0), core::vector3df(0, -9, 5));
	node->addAnimator(anim);
	anim->drop();

//text
	gui::IGUIEnvironment* guienv = dev->getGUIEnvironment();
	gui::IGUIFont* font = guienv->getFont("media/bigfont.png");
	gui::IGUIStaticText* textinfo = guienv->addStaticText(L"",
			core::rect<s32>(10, 10, 200, 50), false);
	textinfo->setOverrideColor(video::SColor(120, 255, 255, 255));
	textinfo->setOverrideFont(font);
	textinfo->setWordWrap(true);

	int maxfps=0,minfps=9999,sumfps=0,fcount=0;
	int lastFPS = -1;
	float wheel = 0;

	scene::IBoneSceneNode *bone, *e1, *e2, *rootbone = node->getJointNode(
			(u32) 0);

	e1 = node->getJointNode("Joint23");
	e2 = node->getJointNode("Joint19");
	bone = node->getJointNode("Joint13");
	Target target(dev, 0, 0, 0);
	target.show();

	core::vector3df vec, normal, v1, v2, old = node1->getAbsolutePosition(), r;

	float delta = 2;
	r.X = 30;
//main loop

	while (dev->run()) {

		drv->beginScene(true, true, video::SColor(1));
//starting the scene =====================

		node->updateAbsolutePosition();
		rootbone->updateAbsolutePositionOfAllChildren();

		int fps = drv->getFPS();

		if (lastFPS != fps) {
			core::stringw str = L"Irrlicht, FPS: ";
			core::stringw istr = L"FPS: ";
			str += fps;
			istr += fps;
			textinfo->setText(istr.c_str());
			dev->setWindowCaption(str.c_str());
			lastFPS = fps;

			sumfps+=fps;
			++fcount;
			if(fps > maxfps)
				maxfps=fps;
			if((fps < minfps) && fcount > 2)
				minfps = fps;
			std::cout<<" min: "<<minfps<<" max: "<<maxfps<<" avg: "<<sumfps/fcount<<std::endl;
		}

// keys ===============
//q3mapNode->setVisible(false);
		if (input.isPressed(irr::KEY_TAB)) {

			scene::IBillboardTextSceneNode *tsn;
			if (scene->getSceneNodeFromId(BONELABEL) == NULL) {
				boneLabels(scene, font, rootbone);
				node->setDebugDataVisible(
						(scene::E_DEBUG_SCENE_TYPE) (scene::EDS_MESH_WIRE_OVERLAY
								| scene::EDS_HALF_TRANSPARENCY
								| scene::EDS_SKELETON));
			} else {
				while ((tsn =
						(scene::IBillboardTextSceneNode *) scene->getSceneNodeFromId(
								BONELABEL))) {

					tsn->remove();

				}

				node->setDebugDataVisible(
						(scene::E_DEBUG_SCENE_TYPE) (scene::EDS_OFF));

			}
			input.unpress(irr::KEY_TAB);
		}
		if (input.isPressed(irr::KEY_F1)) {
			bone = node->getJointNode("Joint13");
			target.setPosition(bone->getAbsolutePosition());
//			node->setDebugDataVisible(
//								(scene::E_DEBUG_SCENE_TYPE) (scene::EDS_MESH_WIRE_OVERLAY | scene::EDS_HALF_TRANSPARENCY | scene::EDS_SKELETON));

		}
		if (input.isPressed(irr::KEY_F2)) {
			bone = node->getJointNode("Joint16");
			target.setPosition(bone->getAbsolutePosition());
//			node->setDebugDataVisible(
//					(scene::E_DEBUG_SCENE_TYPE) ( scene::EDS_HALF_TRANSPARENCY | scene::EDS_SKELETON));

		}
		if (input.isPressed(irr::KEY_F3)) {
			bone = node->getJointNode("Joint23");
			target.setPosition(bone->getAbsolutePosition());
		}
		if (input.isPressed(irr::KEY_F4)) {
			bone = node->getJointNode("Joint19");
			target.setPosition(bone->getAbsolutePosition());
		}
		float scalefactor = 1;
		vec = target.getPosition();

		if (input.isPressed(irr::KEY_ESCAPE)) {
			dev->drop();
			return EXIT_SUCCESS;
		}
		if (input.isPressed(irr::KEY_KEY_W)) {
			vec.X -= 1 * scalefactor;

		}
		if (input.isPressed(irr::KEY_KEY_S)) {
			vec.X += 1 * scalefactor;
		}
		if (input.isPressed(irr::KEY_KEY_A)) {
			vec.Z -= 1 * scalefactor;
		}
		if (input.isPressed(irr::KEY_KEY_D)) {
			vec.Z += 1 * scalefactor;
		}
		if (input.isPressed(irr::KEY_KEY_Q)) {
			vec.Y -= 1 * scalefactor;
		}
		if (input.isPressed(irr::KEY_KEY_E)) {
			vec.Y += 1 * scalefactor;
		}
		if (vec != target.getPosition() || input.isPressed(irr::KEY_SPACE)) {

			target.setPosition(vec);
			for (int i = 0; i < 5; ++i) {
				anim_ccd1(*bone, (scene::IBoneSceneNode &) *(bone->getParent()),
						target.getAbsolutePosition(), 1, drv);
				rootbone->updateAbsolutePositionOfAllChildren();
				node->updateAbsolutePosition();
			}
		}

		wheel = input.getWheel();
		if (wheel != 0) {
			vec = camera->getTarget();
			normal = camera->getUpVector();
			normal.normalize();
			vec += normal * wheel * 10;
			camera->setTarget(vec);
		}
		if (input.isPressed(irr::KEY_LEFT)) {
			node->setPosition(
					node->getAbsolutePosition() + core::vector3df(0, 0, -1));
		}
		if (input.isPressed(irr::KEY_RIGHT)) {
			node->setPosition(
					node->getAbsolutePosition() + core::vector3df(0, 0, 1));
		}
		if (input.isPressed(irr::KEY_UP)) {
			node->setPosition(
					node->getAbsolutePosition() + core::vector3df(0, 10, 0));
		}
		if (input.isPressed(irr::KEY_DOWN)) {
			node->setPosition(
					node->getAbsolutePosition() + core::vector3df(0, -10, 0));
		}

//drawing scene =====================
		scene->drawAll();
		scene->drawAll();
		guienv->drawAll();

//ending scene =====================
		drv->endScene();

		//frame animation stairs climbing
//		vec = node1->getAbsolutePosition();
//		delta = old.Z - vec.Z;
//
//		old = vec;
//		if ((-540 < vec.Z) && (vec.Z < -265) && (delta == 0)) {
//			vec.Y += 5;
//		}
//		if (-600 > vec.Z)
//			vec.Z = -150;
//
//		vec.Z -= 0.25;
//		node1->setPosition(vec);
//		node1->updateAbsolutePosition();

		vec = node->getAbsolutePosition();
		delta = old.Z - vec.Z;

		old = vec;
		if ((-540 < vec.Z) && (vec.Z < -265) && (delta == 0)) {
			vec.Y += 5;
			target.setPosition(vec + core::vector3df(0, 10, -20));
		}
		if (-600 > vec.Z) {
			vec.Z = -300;
			target.setPosition(core::vector3df(0, 0, -300));
		}
		vec.Z -= 0.25;
		node->setPosition(vec);
		node->updateAbsolutePosition();

//		target.setPosition(e1->getAbsolutePosition()+core::vector3df(0, 1, -1));
//		anim_ccd1(*e1, (scene::IBoneSceneNode &) *(e1->getParent()),
//				target.getAbsolutePosition(), 1, drv);

		stairsclimb(node, dev);
		rootbone->updateAbsolutePositionOfAllChildren();

	} //mainloop end

	dev->drop();

	return EXIT_SUCCESS;
}
