#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "../header/meshLoader.h"
#include "../header/shader.h"
#include "../header/matrices.h"
#include <iostream>
#include "../header/mnist.h"
#include "../header/mlp.h"
#include <time.h>
#include <fstream>

using namespace std;

//screen width and height init
float SCREEN_WIDTH = 640.0;
float SCREEN_HEIGHT = 480.0;
//SDL init
SDL_Window* gWindow;
SDL_Surface* gScreenSurface;
//matrix operations
matrices pipeline;
//scene: can be used to display images or objects
meshLoader* scene;
//mesh to attach images to and render
mesh* quad;
mesh* quadInverted;

//MNIST Object
idxFileReader* digitSet;
unsigned char* digitImage;
unsigned int digitTexture;


//Shaders
shader* displayShades;						//to display Textures the way it is
shader* convolutionFirstShades;					//first convolution + ReLU operation

//frameBuffers and their render targets
unsigned int colorFBO, colorImage;
unsigned int convolutionFirstFBO, convolutionFirstImage;


//buffer pointer to save output of convolution layer to pass it to FCL 
float *convPixelBuffer;
float *labelBuffer;
//buffer pointer to save delta errors received from FCL
float *convGradientBuffer;
unsigned int gradientTexture;

//create openGL Texture or update its contents
unsigned int createTexture(int w,int h, bool isFloatTex = false);
unsigned int createTexture(int w, int h, unsigned char *pixels);
unsigned int createTexture(int w, int h, float *pixels);
void updateTextureContent(unsigned int textureId, int w, int h, unsigned char* pixels);
void updateTextureContent(unsigned int textureId, int w, int h, float* pixels);
unsigned int createRGBTexture(int w, int h);


//dummy variable to check frameBuffer Status
int fbStatus;

//variables to keep track of image
int imgCount;
int imageID = 0;
int label;
int prevImageID = -1;

//Instance of Mulitple Layer Perceptron : FCL
mlp* mlpObject;

//kernel Matrix for convolution Operation
float kernelFirstMatrix[9][3][3];

//initialization function
void init()
{
	//set up world co-ordinate
	pipeline.matrixMode(PROJECTION_MATRIX);
	pipeline.loadIdentity();
	pipeline.ortho(-1.0, 1.0, -1.0, 1.0, 1, 100);

	//load scene : scene used for checking purposes only
	scene = new meshLoader();

	//load MNIST file, read and store them
	digitSet = new idxFileReader("../models/train-images.idx3-ubyte", "../models/train-labels.idx1-ubyte");


	//load shader from file
	displayShades = new shader("../v_shader/displayShader.vs","../f_shader/displayShader.frag");
	convolutionFirstShades = new shader("../v_shader/convolutionFirstShader.vs","../f_shader/convolutionFirstShader.frag");
	
	//create Textures
	colorImage = createTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
	convolutionFirstImage = createTexture(digitSet->getImageWidth(), digitSet->getImageHeight(), true);


	//graph configuration for fully connected layer
	int inputLayerSize = 3 * (digitSet->getImageWidth()) * (digitSet->getImageHeight());
	int hiddenLayerCount = 1;
	int outputLayerCount = 10;
	int numCellsPerLayer = 25;
	mlpObject = new mlp(inputLayerSize, hiddenLayerCount, outputLayerCount, numCellsPerLayer);

	//receive pointer to mlpObject's input buffer, input's label buffer and input's delta buffer
	convPixelBuffer = mlpObject->getPtrToInput();	
	labelBuffer = mlpObject->getPtrToLabel();
	convGradientBuffer = mlpObject->getPtrToInputGradient();		


	//defining frameBuffers with attached textures as render targets

	//colorFBO to convert from char vals[0, 255] to floating point vals[0.0, 1.0] for pixels
	glGenFramebuffers(1, &colorFBO);
	glEnable(GL_TEXTURE_2D);
	glBindFramebuffer(GL_FRAMEBUFFER,colorFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,colorImage,0);

	fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(fbStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer is not OK, status=" << fbStatus << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	
	//convolutionFirstFBO : convolution and ReLU operation
	//kernel Matrix for operation, colorImage as input featureMap and colutionFirstImage[-inf, inf] as render target
	glGenFramebuffers(1, &convolutionFirstFBO);
	glEnable(GL_TEXTURE_2D);
	glBindFramebuffer(GL_FRAMEBUFFER,convolutionFirstFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,convolutionFirstImage,0);

	fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(fbStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer is not OK, status=" << fbStatus << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER,0);


	//quad as 2D surface to attach and render textures, quadInverted is quad inverted vertically
	{
		std::vector<unsigned int> indices;
		std::vector<vertexData> vertices;
		vertexData tmp;
		//1.
		tmp.position.change(-1.0,1.0,-1.0);
		tmp.U=0;
		tmp.V=0;
		vertices.push_back(tmp);
		//2.
		tmp.position.change(-1.0,-1.0,-1.0);
		tmp.U=0;
		tmp.V=1;
		vertices.push_back(tmp);
		//3.
		tmp.position.change(1.0,-1.0,-1.0);
		tmp.U=1;
		tmp.V=1;
		vertices.push_back(tmp);
		//4.
		tmp.position.change(1.0,1.0,-1.0);
		tmp.U=1;
		tmp.V=0;
		vertices.push_back(tmp);
		
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);		
		
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);
		quadInverted=new mesh(&vertices,&indices);
	}

	{
		std::vector<unsigned int> indices;
		std::vector<vertexData> vertices;
		vertexData tmp;
		//1.
		tmp.position.change(-1.0,1.0,-1.0);
		tmp.U=0;
		tmp.V=1;
		vertices.push_back(tmp);
		//2.
		tmp.position.change(-1.0,-1.0,-1.0);
		tmp.U=0;
		tmp.V=0;
		vertices.push_back(tmp);
		//3.
		tmp.position.change(1.0,-1.0,-1.0);
		tmp.U=1;
		tmp.V=0;
		vertices.push_back(tmp);
		//4.
		tmp.position.change(1.0,1.0,-1.0);
		tmp.U=1;
		tmp.V=1;
		vertices.push_back(tmp);
		
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);		
		
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);
		quad = new mesh(&vertices,&indices);
	}

	//initialize kernel Matrices for convolution Operation
	srand(time(NULL));	
	float varFloat;

	for(int itr_i = 0; itr_i < 9; itr_i++)
	{
		for(int itr_j = 0; itr_j < 3; itr_j++)
		{
			for(int itr_k = 0; itr_k < 3; itr_k++)
			 {
				kernelFirstMatrix[itr_i][itr_j][itr_k] = (float)(rand() % 50) / 1000.0;
			 }
		}
	}

	//load and store first Image in digitTexture
	 imgCount = digitSet->getImageCount();
	 digitImage = digitSet->getImage(imageID);
	 label = digitSet->getImageLabel(imageID);
	digitTexture = createTexture(digitSet->getImageWidth(), digitSet->getImageHeight(), digitImage);
	std::cout<<" imgCOunt "<<imgCount<<std::endl;

	//draw Operation
	glClearColor(0.0, 0.0, 0.0, 1);
	pipeline.matrixMode(MODEL_MATRIX);

imgCount = 0;
float sumVal = 0.0;
float sumV = 0.0;
int correctOut = 0;

	for(int itr = 0; itr < 20000; itr++)
	{

	 imageID = rand()%20000;
	 digitImage = digitSet->getImage(imageID);
	 label = digitSet->getImageLabel(imageID);
	 updateTextureContent(digitTexture, digitSet->getImageWidth(), digitSet->getImageHeight(), digitImage);
	 labelBuffer[label] = 1.0;

	//image of charType converted to image of floating point type with RGBA vals, output colorImage
	glBindFramebuffer(GL_FRAMEBUFFER,colorFBO);
		displayShades->useShader();
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, digitTexture);
		glUniform1i(glGetUniformLocation(displayShades->getProgramId(),"texture0"),0);

		glBindFragDataLocation(displayShades->getProgramId(), 0, "colorImage");

		pipeline.updateMatrices(displayShades->getProgramId());
		quad->draw(displayShades->getProgramId());
		glBindTexture(GL_TEXTURE_2D, 0);
		displayShades->delShader();
	glBindFramebuffer(GL_FRAMEBUFFER,0);


	//take colorImage and convolove, output convolutionFirstImage
	glBindFramebuffer(GL_FRAMEBUFFER,convolutionFirstFBO);
		convolutionFirstShades->useShader();
		glClear(GL_COLOR_BUFFER_BIT);

		//input Image for this instant
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorImage);
		glUniform1i(glGetUniformLocation(convolutionFirstShades->getProgramId(),"inputImage"),0);		

		//screen and texture size descriptions
		glUniform1f(glGetUniformLocation(convolutionFirstShades->getProgramId(),"TEX_WIDTH"), digitSet->getImageWidth());
		glUniform1f(glGetUniformLocation(convolutionFirstShades->getProgramId(),"TEX_HEIGHT"), digitSet->getImageHeight());
		
		glUniform1f(glGetUniformLocation(convolutionFirstShades->getProgramId(),"SCREEN_WIDTH"), SCREEN_WIDTH);
		glUniform1f(glGetUniformLocation(convolutionFirstShades->getProgramId(),"SCREEN_HEIGHT"), SCREEN_HEIGHT);

		//kernel Matrix to multiply input Image with
	glUniformMatrix3fv(glGetUniformLocation(convolutionFirstShades->getProgramId(),"kernelMatrix"), 9, GL_FALSE, &kernelFirstMatrix[0][0][0]);

		//output Images
		glBindFragDataLocation(convolutionFirstShades->getProgramId(), 0, "convolutionFirstImage");

		pipeline.updateMatrices(convolutionFirstShades->getProgramId());
		quad->draw(convolutionFirstShades->getProgramId());
		glBindTexture(GL_TEXTURE_2D, 0);		
		convolutionFirstShades->delShader();
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	
	//read contents of convolutionFirstImage and store it in convPixel Buffer for forward pass to MLP
	glBindFramebuffer(GL_FRAMEBUFFER, convolutionFirstFBO);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(0, 0, digitSet->getImageWidth(), digitSet->getImageHeight(), GL_RGB, GL_FLOAT, convPixelBuffer);		//123
	glReadBuffer(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//forward MLP
		imgCount++;
		sumVal = mlpObject->learnFromMLP(0.85, label, imageID);
		sumV += sumVal;
		if(sumVal < 0.2) correctOut++;
	
		if(imgCount >= 1000)
		{
			imgCount = 0;
			std::cout<<"itr "<<itr<<"P "<<sumV/1000.0<<" correction "<<correctOut<<std::endl;
			sumVal = 0.0;
			sumV = 0.0;
			correctOut = 0;
		}
	
	 labelBuffer[label] = 0.0;

	}

	correctOut = 0;
	//testing phase
	for(int itr = 0; itr < 50; itr++)
	{
		imageID = 20000 + (rand() % 500);
		 digitImage = digitSet->getImage(imageID);
		 label = digitSet->getImageLabel(imageID);
	 	updateTextureContent(digitTexture, digitSet->getImageWidth(), digitSet->getImageHeight(), digitImage);

	//image of charType converted to image of floating point type with RGBA vals, output colorImage
	glBindFramebuffer(GL_FRAMEBUFFER,colorFBO);
		displayShades->useShader();
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, digitTexture);
		glUniform1i(glGetUniformLocation(displayShades->getProgramId(),"texture0"),0);

		glBindFragDataLocation(displayShades->getProgramId(), 0, "colorImage");

		pipeline.updateMatrices(displayShades->getProgramId());
		quad->draw(displayShades->getProgramId());
		glBindTexture(GL_TEXTURE_2D, 0);
		displayShades->delShader();
	glBindFramebuffer(GL_FRAMEBUFFER,0);


	//take colorImage and convolove, output convolutionFirstImage
	glBindFramebuffer(GL_FRAMEBUFFER,convolutionFirstFBO);
		convolutionFirstShades->useShader();
		glClear(GL_COLOR_BUFFER_BIT);

		//input Image for this instant
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorImage);
		glUniform1i(glGetUniformLocation(convolutionFirstShades->getProgramId(),"inputImage"),0);		

		//screen and texture size descriptions
		glUniform1f(glGetUniformLocation(convolutionFirstShades->getProgramId(),"TEX_WIDTH"), digitSet->getImageWidth());
		glUniform1f(glGetUniformLocation(convolutionFirstShades->getProgramId(),"TEX_HEIGHT"), digitSet->getImageHeight());
		
		glUniform1f(glGetUniformLocation(convolutionFirstShades->getProgramId(),"SCREEN_WIDTH"), SCREEN_WIDTH);
		glUniform1f(glGetUniformLocation(convolutionFirstShades->getProgramId(),"SCREEN_HEIGHT"), SCREEN_HEIGHT);

		//kernel Matrix to multiply input Image with
	glUniformMatrix3fv(glGetUniformLocation(convolutionFirstShades->getProgramId(),"kernelMatrix"), 9, GL_FALSE, &kernelFirstMatrix[0][0][0]);

		//output Images
		glBindFragDataLocation(convolutionFirstShades->getProgramId(), 0, "convolutionFirstImage");

		pipeline.updateMatrices(convolutionFirstShades->getProgramId());
		quad->draw(convolutionFirstShades->getProgramId());
		glBindTexture(GL_TEXTURE_2D, 0);		
		convolutionFirstShades->delShader();
		glBindFramebuffer(GL_FRAMEBUFFER,0);

	
	//read contents of convolutionFirstImage and store it in convPixel Buffer for forward pass to MLP
	glBindFramebuffer(GL_FRAMEBUFFER, convolutionFirstFBO);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(0, 0, digitSet->getImageWidth(), digitSet->getImageHeight(), GL_RGB, GL_FLOAT, convPixelBuffer);		//123
	glReadBuffer(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	imgCount = mlpObject->testMLP();
	std::cout<<" itertation "<<itr<<" actual label "<<label<<" calc Label "<<imgCount<<std::endl;
	if(label == imgCount) correctOut++;
	}
	std::cout<<" correctCount "<<correctOut<<" percent "<<(float)correctOut/50.0<<std::endl;

/*
imgCount = 0;
float sumVal = 0.0;
float sumV = 0.0;
int correctOut = 0;
	for(int itr = 0; itr < 1000000; itr++)
	{
	 if(itr % 10000 == 0) srand(time(NULL));

	 imageID = rand()%8000;
	 digitImage = digitSet->getImage(imageID);
	 label = digitSet->getImageLabel(imageID);
//	 std::cout<<std::endl<<" label "<<label<<std::endl;
//	 updateTextureContent(digitTexture, digitSet->getImageWidth(), digitSet->getImageHeight(), digitImage);
//	 prevImageID = imageID;
		
	for(int it = 0; it < (digitSet->getImageWidth()) * (digitSet->getImageHeight()); it++)
		convPixelBuffer[it] = (float)digitImage[it]/255.0;

	labelBuffer[label] = 1.0;

	if(1) 
	{
		imgCount++;
		sumVal = mlpObject->learnFromMLP(0.85, label, imageID);
		sumV += sumVal;
		if(sumVal < 0.2) correctOut++;
	}
	if(imgCount >= 1000)
	{
		imgCount = 0;
		std::cout<<"itr "<<itr<<"P "<<sumV/1000.0<<" correction "<<correctOut<<std::endl;
		sumVal = 0.0;
		sumV = 0.0;
		correctOut = 0;
	}
	
	 labelBuffer[label] = 0.0;
	}

*/
}


void display()
{	


	//display ouput, less changes on produced results
	glClearColor(1, 0, 0, 1);
	displayShades->useShader();
	glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, convolutionFirstImage);
		glUniform1i(glGetUniformLocation(displayShades->getProgramId(),"texture0"),0);
	pipeline.updateMatrices(displayShades->getProgramId());
	quadInverted->draw(displayShades->getProgramId());
	displayShades->delShader();
	
}


int main()
{

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	gWindow = SDL_CreateWindow("SDL_COLLIDE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	SDL_GLContext gContext = SDL_GL_CreateContext(gWindow);
	glewExperimental = GL_TRUE;
	glewInit();
	SDL_GL_SetSwapInterval( 1 );
	gScreenSurface = SDL_GetWindowSurface( gWindow );

	bool running=true;
	SDL_Event event;	
	init();

	while(running)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
				running = false;
				break;
	
				case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running=false;
							break;
						case SDLK_RIGHT:	
							imageID++;
							break;
						case SDLK_LEFT:
							imageID--;
							break;

						
					}
	
			}
		}

		display();
		SDL_GL_SwapWindow(gWindow);

	}

	delete mlpObject;
	delete displayShades;
	delete convolutionFirstShades;
	delete scene;
	delete quad, quadInverted;
	delete digitSet;
	SDL_FreeSurface(gScreenSurface);
	SDL_GL_DeleteContext(gContext);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();

	return 0;
}

unsigned int createTexture(int w,int h, bool isFloatTex)
{
	unsigned int textureId;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1,&textureId);
	glBindTexture(GL_TEXTURE_2D,textureId);
	glTexImage2D(GL_TEXTURE_2D,0, (isFloatTex ? GL_RGBA32F : GL_RGBA8), w, h, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
	
	int i;
	i=glGetError();
	if(i!=0)
		std::cout << "Error happened while loading the texture: " << gluErrorString(i) << std::endl;
	glBindTexture(GL_TEXTURE_2D,0);
	return textureId;
}

unsigned int createTexture(int w, int h, unsigned char *pixels)
{
	unsigned int textureId;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1,&textureId);
	glBindTexture(GL_TEXTURE_2D,textureId);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RED,w,h,0,GL_RED,GL_UNSIGNED_BYTE,pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
	//int i;
	//i=glGetError();
	//if(i!=0)
	//	std::cout << "Error happened while loading the texture: " << gluErrorString(i) << std::endl;
	glBindTexture(GL_TEXTURE_2D,0);
	return textureId;

}

unsigned int createRGBTexture(int w, int h)
{
	unsigned int textureId;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1,&textureId);
	glBindTexture(GL_TEXTURE_2D,textureId);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,w,h,0,GL_RGB,GL_FLOAT,0);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
	//int i;
	//i=glGetError();
	//if(i!=0)
	//	std::cout << "Error happened while loading the texture: " << gluErrorString(i) << std::endl;
	glBindTexture(GL_TEXTURE_2D,0);
	return textureId;

}

void updateTextureContent(unsigned int textureId, int w, int h, float* pixels)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,textureId);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,w,h,0,GL_RGB,GL_FLOAT,pixels);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D,0);

}


void updateTextureContent(unsigned int textureId, int w, int h, unsigned char* pixels)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,textureId);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RED,w,h,0,GL_RED,GL_UNSIGNED_BYTE,pixels);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D,0);

}


