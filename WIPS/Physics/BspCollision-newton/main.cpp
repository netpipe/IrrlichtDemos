//
// Irrlicht 3D Engine / Newton Physics SDK Demo
//

#include <irrlicht.h>
#include <memory.h>
#include <iostream>

// Requires Newton SDK 1.53 or later
#include <newton/Newton.h>

#pragma comment (lib, "irrlicht.lib")

// Irrlicht namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;

// Newton
#define NEWTON_GRAVITY -800.0f
static NewtonWorld* nWorld;

// Irrlicht
bool				g_bQuit = false;
IrrlichtDevice		*device = NULL;
IVideoDriver		*driver = NULL;
ISceneManager		*smgr = NULL;
IGUIStaticText		*pText = NULL;
bool				debugOn = true;
ICameraSceneNode	*camera = NULL;
core::aabbox3df		box;

// Struct to tie a Newton body to an Irrlicht mesh/node
struct NewtonCube {
	IAnimatedMesh *mesh;
	ISceneNode *node;
	NewtonBody *body;
	const NewtonBody *waterBody;
};

// allocation of memory from Newton
void* _cdecl
NewtonAlloc (int sizeInBytes)
{
	return new char[sizeInBytes];
}

// destruction of memory from Newton
void _cdecl
NewtonFree (void *ptr, int sizeInBytes)
{
	char *tmp;
	tmp = (char*) ptr;
	delete[] tmp;
}

// Newton callback
void setMeshTransformEvent(const NewtonBody* body, const float* matrix)
{
	// copy the matrix into an irrlicht matrix4
	matrix4 mat;
	memcpy(mat.M, matrix, sizeof(float)*16);

	// Retrieve the user data attached to the newton body
	ISceneNode *tmp = ((NewtonCube *)NewtonBodyGetUserData(body))->node;
	if (tmp)
	{
		// Position the node
		tmp->setPosition(mat.getTranslation());
		tmp->setRotation(mat.getRotationDegrees());
	}
}

// Newton callback
void applyForceAndTorqueEvent(const NewtonBody* body)
{
	float mass;
	float Ixx;
	float Iyy;
	float Izz;

	NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);

	vector3df force (0.0f , NEWTON_GRAVITY * mass , 0.0f);
	vector3df torque (0.0f, 0.0f, 0.0f);

	NewtonBodySetForce(body, &force.X);
}

// Irrlicht event receiver
class MyEventReceiver : public  IEventReceiver
{
public:
	virtual bool OnEvent(SEvent event) {
		// Test for keypresses
		if (event.EventType == irr::EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown) {
			switch(event.KeyInput.Key) {
			case KEY_ESCAPE:
				g_bQuit = true;
				break;
			case KEY_KEY_Q:
				debugOn = !debugOn;
				break;
			}
		}

		// Test for mouse events
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) {
			// make a cube where the camera is and set its velocity to follow the target

			vector3df camvec = (camera->getTarget() - camera->getPosition()).normalize() * 500;

			NewtonCube *tmp = new NewtonCube;
			tmp->mesh = smgr->getMesh("data/smallcube.3ds");
			tmp->node = smgr->addAnimatedMeshSceneNode(tmp->mesh);
			tmp->node->setMaterialTexture(0, driver->getTexture("data/crate.jpg"));
			tmp->node->setMaterialFlag(EMF_LIGHTING, false);
			tmp->waterBody = NULL;

			// Create a box primitive. 40 is just an estimated value of the size of the model
			float x = 40 ;
			float y = 40 ;
			float z = 40 ;
			NewtonCollision* collision;
			collision = NewtonCreateBox(nWorld, x, y, z, NULL);

			// create the body
			NewtonBody* body;
			body = NewtonCreateBody(nWorld, collision);
			tmp->body = body;

			// release the collsion tree (this way the application does not have to do book keeping of Newton objects
			NewtonReleaseCollision (nWorld, collision);

			// Set user data pointer to the scene node
			NewtonBodySetUserData(body, tmp);

			// Set body mass & inertia matrix
			float mass = 10.0f;
			float Ixx = 0.7f * mass * (y * y + z * z) / 12.0f;
			float Iyy = 0.7f * mass * (x * x + z * z) / 12.0f;
			float Izz = 0.7f * mass * (x * x + y * y) / 12.0f;
			NewtonBodySetMassMatrix (body, mass, Ixx, Iyy, Izz);

			// leave the sleep tolerance alone
			NewtonBodySetFreezeTreshold(body, 1.0, 1.0, 1);

			// Set callback functions for the body
			NewtonBodySetTransformCallback(body, setMeshTransformEvent);
			NewtonBodySetForceAndTorqueCallback(body, applyForceAndTorqueEvent);

			// Set the position of the body
			matrix4 mat;
			mat.setTranslation(camera->getPosition());
			NewtonBodySetMatrix(body, &mat.M[0]);

			tmp->node->setPosition(camera->getPosition());
			setMeshTransformEvent(body, &mat.M[0]);

			vector3df nVeloc (camvec);
			NewtonBodySetVelocity(body, &nVeloc.X);
		}

		return 0;
	}
};

// Instantiate an Irrlicht event receiver
MyEventReceiver receiver;

// Entry point
int main() {
	// Print banner and ask user if they want to run fullscreen
	printf("Irrlicht 3D Engine / Newton Physics SDK Demo\n");
	printf("WASD keys to move camera\nLeft Mouse to shoot a box\nEscape key to exit\n\n");
	printf("Do you want to run fullscreen (y/n)?\n");
	char c;
	std::cin >> c;
	bool bFullscreen = ((c == 'y') || (c == 'Y'));

	core::dimension2d<s32> windowSize(800,600);
	u32 windowDepth = 32;

	if (bFullscreen) {
		// Create a NULL device so we can query it for the desktop resolution
		device = createDevice(EDT_NULL);
		if (device) {
			video::IVideoModeList *videoList = device->getVideoModeList();
			if (videoList) {
				windowSize = videoList->getDesktopResolution();
				windowDepth = videoList->getDesktopDepth();
				printf("Desktop is %dx%dx%d\n", windowSize.Width, windowSize.Height, windowDepth);
			} else {
				printf("Could not retrieve videomode list\n");
				bFullscreen = false;
			}
			// Finished with this device.
			device->drop();
		} else {
			printf("Creation of NULL device failed.\nNot running fullscreen.\n");
			bFullscreen = false;
		}
	}
	// Either fullscreen was not chosen or error occured getting desktop
	if (!bFullscreen) {
		windowSize = core::dimension2di(800,600);
		windowDepth = 32;
	}

	// NOTE: Enabling vsync (param 6) seems to help physics stability in this demo.
	device = createDevice(EDT_DIRECT3D9, windowSize, windowDepth, bFullscreen, false, true, &receiver);
	if (!device) {
		// First try failed. Try again with safer params
		device = createDevice(EDT_DIRECT3D9, dimension2d<s32>(800,600), 16, false, false, true, &receiver);
		if (!device) {
			// Exit app
			printf("Unable to create Irrlicht device\n");
			return 0;
		}
	}

	device->setWindowCaption(L"Irrlicht 3D Engine / Newton Physics SDK Demo");

	// Get useful pointers
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();

	// Use 32bit textures
	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	// Disable cursor
	device->getCursorControl()->setVisible(false);

	//1. Create Newtonworld
	nWorld = NewtonCreate(NewtonAlloc, NewtonFree);

	// Set up default material properties for newton
	int i = NewtonMaterialGetDefaultGroupID(nWorld);
	NewtonMaterialSetDefaultFriction   (nWorld, i, i, 0.8f, 0.4f);
	NewtonMaterialSetDefaultElasticity (nWorld, i, i, 0.3f);
	NewtonMaterialSetDefaultSoftness   (nWorld, i, i, 0.05f);
	NewtonMaterialSetCollisionCallback (nWorld, i, i, NULL, NULL, NULL, NULL);

	// Camera wasd navigation
	SKeyMap keyMap[4] = {
		{EKA_MOVE_FORWARD,	KEY_KEY_W},
		{EKA_MOVE_BACKWARD,	KEY_KEY_S},
		{EKA_STRAFE_LEFT,	KEY_KEY_A},
		{EKA_STRAFE_RIGHT,	KEY_KEY_D},
	};
	// Create the viewing camera and position it
	camera = smgr->addCameraSceneNodeFPS(0, 200.0f, 250.0f, -1, keyMap, 4);
	camera->setPosition(vector3df(-26,-221,-155));
	camera->setRotation(vector3df(0,90,0));

	// Load level in Quake3 format
	device->getFileSystem()->addZipFileArchive("./data/chiropteraDM.pk3");
	scene::IAnimatedMesh* q3levelmesh = smgr->getMesh("maps/chiropteradm.bsp");
	scene::ISceneNode* q3node = smgr->addOctTreeSceneNode(q3levelmesh->getMesh(0));

	//LevelNewtonCollision
	NewtonCollision* nmapcollision = NewtonCreateTreeCollision(nWorld, NULL);
	NewtonTreeCollisionBeginBuild(nmapcollision);
	u32 cMeshBuffer, j;
	int v1i, v2i, v3i;
	IMeshBuffer *mb;
	float vArray[9]; // vertex array (3*3 floats)
	int tmpCount = 0;

	scene::IMesh *mesh = q3levelmesh->getMesh(0);
	for (cMeshBuffer=0; cMeshBuffer<mesh->getMeshBufferCount(); cMeshBuffer++)
	{
		mb = q3levelmesh->getMesh(0)->getMeshBuffer(cMeshBuffer);
		S3DVertex2TCoords* mb_vertices = (S3DVertex2TCoords*)mb->getVertices();
		u16* mb_indices  = mb->getIndices();
		// add each triangle from the mesh
		for (j=0; j<mb->getIndexCount(); j+=3)
		{
			v1i = mb_indices[j];
			v2i = mb_indices[j+1];
			v3i = mb_indices[j+2];
			vArray[0] = mb_vertices[v1i].Pos.X;
			vArray[1] = mb_vertices[v1i].Pos.Y;
			vArray[2] = mb_vertices[v1i].Pos.Z;
			vArray[3] = mb_vertices[v2i].Pos.X;
			vArray[4] = mb_vertices[v2i].Pos.Y;
			vArray[5] = mb_vertices[v2i].Pos.Z;
			vArray[6] = mb_vertices[v3i].Pos.X;
			vArray[7] = mb_vertices[v3i].Pos.Y;
			vArray[8] = mb_vertices[v3i].Pos.Z;
			box.addInternalPoint(mb_vertices[v1i].Pos.X, mb_vertices[v1i].Pos.Y, mb_vertices[v1i].Pos.Z);
			box.addInternalPoint(mb_vertices[v2i].Pos.X, mb_vertices[v2i].Pos.Y, mb_vertices[v2i].Pos.Z);
			box.addInternalPoint(mb_vertices[v3i].Pos.X, mb_vertices[v3i].Pos.Y, mb_vertices[v3i].Pos.Z);
			NewtonTreeCollisionAddFace(nmapcollision, 3, &vArray[0], 12, 1);
		}
	}

	NewtonTreeCollisionEndBuild(nmapcollision, 0);
	NewtonBody* nmapbody = NewtonCreateBody(nWorld, nmapcollision);
	box = q3node->getTransformedBoundingBox();
	NewtonSetWorldSize (nWorld, (float*)&(box.MinEdge.X), (float*)&(box.MaxEdge.X));
	NewtonReleaseCollision(nWorld, nmapcollision);

	// Main loop: update physics and 3D engine.
	while(!g_bQuit && device->run())
	{
		if (device->isWindowActive()) {
			// This demo uses a fixed time increment.
			// You may wish to use elapsed time.
			NewtonUpdate(nWorld, 0.01f);

			driver->beginScene(true, true, video::SColor(0,0,0,0));
			smgr->drawAll();

			// Draw a nice bounding box around level mesh.
			// This is the limitations of out physics world.
			video::SMaterial matl;
			matl.Lighting = false;
			driver->setMaterial(matl);
			core::matrix4 matx;
			driver->setTransform(video::ETS_WORLD, matx);
			driver->draw3DBox(q3node->getTransformedBoundingBox());

			driver->endScene();
		}
	}

	// Release Irrlicht device
	device->drop();

	// 4. Destroy Physics World
	NewtonDestroy(nWorld);

	return 0;
}
