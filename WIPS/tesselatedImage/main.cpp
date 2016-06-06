/* ************************************************************** *
 * Bored programmer - part one                                    *
 * by Adam Kazimierczak (adam.kazimierczak@gmail.com)             *
 *                                                                *
 * License: zlib (http://www.gzip.org/zlib/zlib_license.html)     *
 *                                                                *
 * Specially for Irrlicht community                               *
 * http://irrlicht.sourceforge.net/                               *
 *                                                                *
 * May 2010                                                       *
 * ************************************************************** *
 */

// Includes
#include <irrlicht.h>
#include <time.h>
#include <iostream>

using namespace std;

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#define DRIVER EDT_OPENGL // Rendering driver
#define VIDX 800 // render window X
#define VIDY 600  // render window Y
#define FULLSCREEN false // fullscreen mode, bool type

#define DIMENSION 100 // Image size (image MUST be a rectangle!)
#define MATRIX_SIZE (DIMENSION*DIMENSION) // Matrix size (for ball creation)
#define OFFSET 5 // Offset between images at plane (seems 5 is the best)
#define SPEED 5 // Speed of balls moving

#define BALL_TEXTURE "ballf.png" // Balls billboards texture
#define IMG_TEXTURE "test.bmp"  // Image to build


int main() {

	/* INIT STUFF */
	IrrlichtDevice* device = createDevice(DRIVER,
		core::dimension2d<u32>(VIDX, VIDY), 32, FULLSCREEN);

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	srand( unsigned( time(NULL) ) );

	/* CREATE BALLS NODES */
	IBillboardSceneNode *ballNode[MATRIX_SIZE];
	ITexture *ballTexture = driver->getTexture(BALL_TEXTURE);

	for(unsigned int i=0; i<MATRIX_SIZE; i++) {
		ballNode[i] = smgr->addBillboardSceneNode(0, dimension2d<f32>(5, 5));
		ballNode[i]->setMaterialFlag(EMF_LIGHTING, false);
		ballNode[i]->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
		ballNode[i]->setMaterialTexture(0, ballTexture);
		ballNode[i]->setColor(SColor(0,rand()%220,rand()%220,rand()%220)); // Not too bright
		ballNode[i]->setPosition(vector3df(rand()%600,rand()%600,rand()%600)); // Random starting positions
	}


	/* CREATE BALL POSITIONS MATRIX */
	vector3df planeMatrix[MATRIX_SIZE];

	unsigned int x=0, y=0;

	for(unsigned int i=0; i<MATRIX_SIZE; i++) {
		planeMatrix[i] = vector3df(x*OFFSET,y*OFFSET,0);

		x++;

		if(x==DIMENSION) {
			x=0;
			y++;
		}

	}

	///* SET ALL BALLS TO FINAL POSITION (DEBUG ONLY) */
	//for(unsigned int i=0; i<MATRIX_SIZE; i++) {
	//	ballNode[i]->setPosition(vector3df(planeMatrix[i].X, planeMatrix[i].Y, 0));
	//}

	/* CREATE IMAGE MATRIX */
	SColor imageMatrix[MATRIX_SIZE];
	unsigned char tabB [MATRIX_SIZE], tabG [MATRIX_SIZE], tabR [MATRIX_SIZE];
	FILE * file = fopen ( IMG_TEXTURE , "rb" );

	// Load data from BMP
	fseek (file, 54, SEEK_SET); // Seek to where data is (without headers)
	for (int i=0; i < MATRIX_SIZE; i++) {
		fread (&tabB[i],1,1,file);
		fread (&tabG[i],1,1,file);
		fread (&tabR[i],1,1,file);
	}

	fclose(file);

	// Set data to imageMatrix
	for(unsigned int i=0; i<MATRIX_SIZE; i++) {
		imageMatrix[i] = SColor(0,tabR[i],tabG[i],tabB[i]);
	}

	///* SET ALL BALLS TO ITS DESTINATION COLOR (DEBUG ONLY) */
	//for(unsigned int i=0; i<MATRIX_SIZE; i++) {
	//	ballNode[i]->setColor(imageMatrix[i]);
	//}

	/* CAMERA */
	ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0, 100, 0.1);
	device->getCursorControl()->setVisible(false);
	camera->setFarValue(camera->getFarValue()*3); // To see full image
	camera->setPosition(vector3df(168,238,475)); // Little far from image
	camera->setTarget(vector3df(174,247,-82)); // Center of image

	/* VARIABLES */
	register unsigned int i; // Used in main algorithm loop
	unsigned int cur_ball_x, cur_ball_y, cur_ball_z, new_ball_x, new_ball_y, new_ball_z; // ball positions
	unsigned char cur_ball_r, cur_ball_g, cur_ball_b, new_ball_r, new_ball_g, new_ball_b;
	SColor cur_ball_color, new_ball_color;
	int fps, lastFPS = -1;

	IBillboardSceneNode **ballNodePointer = ballNode;
	vector3df *planeMatrixPointer = planeMatrix;
	SColor *imageMatrixPointer = imageMatrix;


	/* MAIN LOOP */
	while(device->run())
	{

		/* GET FPS */
		fps = driver->getFPS();

		if (lastFPS != fps) {
			stringw title(L"Bored programmer - part one test [");
			title += driver->getName();
			title += L"] FPS: ";
			title += fps;

			device->setWindowCaption(title.c_str());
			lastFPS = fps;
		}

		// Reset pointers
		ballNodePointer = ballNode;
		planeMatrixPointer = planeMatrix;
		imageMatrixPointer = imageMatrix;

		/* MAIN ALGHORITMS */
		for(i=0; i<MATRIX_SIZE; i++) {

			// Get current balls positions
			cur_ball_x = (int)(*ballNodePointer)->getPosition().X;
			cur_ball_y = (int)(*ballNodePointer)->getPosition().Y;
			cur_ball_z = (int)(*ballNodePointer)->getPosition().Z;

			// Get current balls colors
			(*ballNodePointer)->getColor(cur_ball_color, cur_ball_color);

			cur_ball_r = cur_ball_color.getRed();
			cur_ball_g = cur_ball_color.getGreen();
			cur_ball_b = cur_ball_color.getBlue();

			/* position */
			// X
			if(cur_ball_x > (*planeMatrixPointer).X )
				new_ball_x = cur_ball_x - SPEED;

			if(cur_ball_x < (*planeMatrixPointer).X)
				new_ball_x = cur_ball_x + SPEED;

			if(cur_ball_x == (*planeMatrixPointer).X) {
				new_ball_x = cur_ball_x;
			}

			// Y
			if(cur_ball_y > (*planeMatrixPointer).Y)
				new_ball_y = cur_ball_y - SPEED;

			if(cur_ball_y < (*planeMatrixPointer).Y)
				new_ball_y = cur_ball_y + SPEED;

			if(cur_ball_y == (*planeMatrixPointer).Y) {
				new_ball_y = cur_ball_y;
			}

			// Z
			if(cur_ball_z > (*planeMatrixPointer).Z)
				new_ball_z = cur_ball_z - SPEED;

			if(cur_ball_z < (*planeMatrixPointer).Z)
				new_ball_z = cur_ball_z + SPEED;

			if(cur_ball_z == (*planeMatrixPointer).Z) {
				new_ball_z = cur_ball_z;
			}

			/* color */
			// Red
			if(cur_ball_r > (*imageMatrixPointer).getRed() )
				new_ball_r = cur_ball_r - 1;

			if(cur_ball_r < (*imageMatrixPointer).getRed())
				new_ball_r = cur_ball_r + 1;

			if(cur_ball_r == (*imageMatrixPointer).getRed())
				new_ball_r = cur_ball_r;


			// Green
			if(cur_ball_g > (*imageMatrixPointer).getGreen())
				new_ball_g = cur_ball_g - 1;

			if(cur_ball_g < (*imageMatrixPointer).getGreen())
				new_ball_g = cur_ball_g + 1;

			if(cur_ball_g == (*imageMatrixPointer).getGreen())
				new_ball_g = cur_ball_g;

			if(cur_ball_b > (*imageMatrixPointer).getBlue())
				new_ball_b = cur_ball_b - 1;

			if(cur_ball_b < (*imageMatrixPointer).getBlue())
				new_ball_b = cur_ball_b + 1;

			if(cur_ball_b == (*imageMatrixPointer).getBlue())
				new_ball_b = cur_ball_b;

			// Update data (position and color)
			(*ballNodePointer)->setPosition(vector3df(new_ball_x, new_ball_y, new_ball_z));
			(*ballNodePointer)->setColor(SColor(0, new_ball_r, new_ball_g, new_ball_b));

			// Update pointers
			ballNodePointer++;
			planeMatrixPointer++;
			imageMatrixPointer++;
		}

		/* RENDER */
		driver->beginScene(true, true, video::SColor(255,245,245,245));
		smgr->drawAll();
		driver->endScene();
	}

	device->drop();
	return 0;

}

// i hope this will work :p
