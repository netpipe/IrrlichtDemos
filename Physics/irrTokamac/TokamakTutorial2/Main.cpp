
// Tokamak Tutorial 2
//
// By Adam Dawes, www.adamdawes.com, September 2003


#include <windows.h>
#include <atlbase.h>
#include <stdlib.h>

#include "Main.h"
#include "DXUtil.h"


#include <tokamak.h>



#pragma comment(lib,"d3d8.lib")
#pragma comment(lib,"d3dx8.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "tokamak.lib")


// Function prototypes

void MessagePump(void);
bool GetFullscreen(void);
bool InitWindow(void);
void KillWindow(void);
bool InitD3D(void);
void KillD3D(void);
bool InitScene(void);
void KillScene(void);
bool InitObjects(void);
void KillObjects(void);
bool InitPhysics(void);
void KillPhysics(void);
D3DFORMAT FindDisplayMode(void);
bool InitTimer(void);
float GetElapsedTime(void);
bool ValidateDevice(void);
void Render(void);


LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM WParam, LPARAM LParam);


// Variables for this sourcefile
HINSTANCE ghInstance = NULL;
HWND ghAppWindow = NULL;

LPDIRECT3D8 gD3D=NULL;

D3DPRESENT_PARAMETERS gD3Dpp;

D3DLIGHT8 gLight;

bool gbFullscreen = false;
bool gbFinished = false;



// Global variables for DirectX
IDirect3DDevice8 *gD3DDevice=NULL;

// Global variables for Tokamak
neSimulator *gSim = NULL;
// The number of cubes to render in the simulation (try values between 2 and about 50)
#define CUBECOUNT 5
neRigidBody *gCubes[CUBECOUNT];
neAnimatedBody *gFloor = NULL;


// Global constants
#define APP_NAME "Tokamak Tutorial"
#define WINWIDTH 800
#define WINHEIGHT 600


// Timer variables
bool gbUseHFTimer;
INT64 gCurrentTime;
float gfTimeScale;


// Vertex buffers
IDirect3DVertexBuffer8 *vbCube = NULL;
IDirect3DVertexBuffer8 *vbFloor = NULL;

// Vertex data for the cubes
// Note that the cube's dimensions are 1 unit in each axis.

strVertex gCubeVertices[] =
{
	{-0.5f,-0.5f,-0.5f,   0.0f, 0.0f,-1.0f,   0xffff0000, 0.0f, 1.0f },  //Front face
	{-0.5f, 0.5f,-0.5f,   0.0f, 0.0f,-1.0f,   0xffffffff, 0.0f, 0.0f },
	{ 0.5f, 0.5f,-0.5f,   0.0f, 0.0f,-1.0f,   0xffffffff, 1.0f, 0.0f },
	{ 0.5f, 0.5f,-0.5f,   0.0f, 0.0f,-1.0f,   0xffffffff, 1.0f, 0.0f },
	{ 0.5f,-0.5f,-0.5f,   0.0f, 0.0f,-1.0f,   0xffffffff, 1.0f, 1.0f },
	{-0.5f,-0.5f,-0.5f,   0.0f, 0.0f,-1.0f,   0xffffffff, 0.0f, 1.0f },

	{ 0.5f,-0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0xff00ff00, 0.0f, 1.0f },  //Back face
	{ 0.5f, 0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0xffffffff, 0.0f, 0.0f },
	{-0.5f, 0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0xffffffff, 1.0f, 0.0f },
	{-0.5f, 0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0xffffffff, 1.0f, 0.0f },
	{-0.5f,-0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0xffffffff, 1.0f, 1.0f },
	{ 0.5f,-0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0xffffffff, 0.0f, 1.0f },

	{-0.5f, 0.5f,-0.5f,   0.0f, 1.0f, 0.0f,   0xff0000ff, 0.0f, 1.0f },  //Top face
	{-0.5f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   0xffffffff, 0.0f, 0.0f },
	{ 0.5f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   0xffffffff, 1.0f, 0.0f },
	{ 0.5f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   0xffffffff, 1.0f, 0.0f },
	{ 0.5f, 0.5f,-0.5f,   0.0f, 1.0f, 0.0f,   0xffffffff, 1.0f, 1.0f },
	{-0.5f, 0.5f,-0.5f,   0.0f, 1.0f, 0.0f,   0xffffffff, 0.0f, 1.0f },

	{ 0.5f,-0.5f,-0.5f,   0.0f,-1.0f, 0.0f,   0xffffff00, 0.0f, 1.0f },  //Bottom face
	{ 0.5f,-0.5f, 0.5f,   0.0f,-1.0f, 0.0f,   0xffffffff, 0.0f, 0.0f },
	{-0.5f,-0.5f, 0.5f,   0.0f,-1.0f, 0.0f,   0xffffffff, 1.0f, 0.0f },
	{-0.5f,-0.5f, 0.5f,   0.0f,-1.0f, 0.0f,   0xffffffff, 1.0f, 0.0f },
	{-0.5f,-0.5f,-0.5f,   0.0f,-1.0f, 0.0f,   0xffffffff, 1.0f, 1.0f },
	{ 0.5f,-0.5f,-0.5f,   0.0f,-1.0f, 0.0f,   0xffffffff, 0.0f, 1.0f },

	{-0.5f,-0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,   0xffff00ff, 0.0f, 1.0f },  //Left face
	{-0.5f, 0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,   0xffffffff, 0.0f, 0.0f },
	{-0.5f, 0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,   0xffffffff, 1.0f, 0.0f },
	{-0.5f, 0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,   0xffffffff, 1.0f, 0.0f },
	{-0.5f,-0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,   0xffffffff, 1.0f, 1.0f },
	{-0.5f,-0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,   0xffffffff, 0.0f, 1.0f },

	{ 0.5f,-0.5f,-0.5f,   1.0f, 0.0f, 0.0f,   0xff00ffff, 0.0f, 1.0f },  //Right face
	{ 0.5f, 0.5f,-0.5f,   1.0f, 0.0f, 0.0f,   0xffffffff, 0.0f, 0.0f },
	{ 0.5f, 0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   0xffffffff, 1.0f, 0.0f },
	{ 0.5f, 0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   0xffffffff, 1.0f, 0.0f },
	{ 0.5f,-0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   0xffffffff, 1.0f, 1.0f },
	{ 0.5f,-0.5f,-0.5f,   1.0f, 0.0f, 0.0f,   0xffffffff, 0.0f, 1.0f },

};
#define NUM_VERTICES_CUBE (sizeof(gCubeVertices)/sizeof(strVertex))

// Vertex data for the floor (change FLOORSIZE for larger/smaller floor area)
#define FLOORSIZE 30
strVertex gFloorVertices[] =
{
	{-(FLOORSIZE/2), 0, -(FLOORSIZE/2),   0.0f, 1.0f, -0.0f,   0xffff0000, 0.0f, 1.0f },  //Top face
	{-(FLOORSIZE/2), 0,  (FLOORSIZE/2),   0.0f, 1.0f, -0.0f,   0xffffffff, 0.0f, 0.0f },
	{ (FLOORSIZE/2), 0,  (FLOORSIZE/2),   0.0f, 1.0f, -0.0f,   0xffffffff, 1.0f, 0.0f },
	{ (FLOORSIZE/2), 0,  (FLOORSIZE/2),   0.0f, 1.0f, -0.0f,   0xffffffff, 1.0f, 0.0f },
	{ (FLOORSIZE/2), 0, -(FLOORSIZE/2),   0.0f, 1.0f, -0.0f,   0xffffffff, 1.0f, 1.0f },
	{-(FLOORSIZE/2), 0, -(FLOORSIZE/2),   0.0f, 1.0f, -0.0f,   0xffffffff, 0.0f, 1.0f },
};
#define NUM_VERTICES_FLOOR (sizeof(gFloorVertices)/sizeof(strVertex))



// Application entry point
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pszCmdLine, int iShow)
{

    CoInitialize(NULL);

	// Store our instance handle
	ghInstance = hInstance;

	if (!GetFullscreen())
	{
		OutputDebugString("  User abort\n");
		exit(5);
	}

	if (!InitWindow())
	{
		OutputDebugString("  Window initialisation failed\n");
		exit(5);
	}

	if (!InitD3D())
	{
		OutputDebugString("  Direct3D initialisation failed\n");
		exit(5);
	}

	if (!InitScene())
	{
		OutputDebugString("  Scene initialisation failed\n");
		exit(5);
	}

	if (!InitTimer())
	{
		OutputDebugString("  Timer initialisation failed\n");
		exit(5);
	}

	if (!InitObjects())
	{
		OutputDebugString("  Object initialisation failed\n");
		exit(5);
	}

	if (!InitPhysics())
	{
		OutputDebugString("  Physics initialisation failed\n");
		exit(5);
	}



	// Find the current time so that initialisation time isn't included within the first frame
	GetElapsedTime();


	//Loop until we are closed
	while(!gbFinished)
	{
		//Check for window messages
		MessagePump();

		//Draw our graphics
		Render();
	}


	// Terminate everything
	KillPhysics();
	KillObjects();
	KillScene();
	KillD3D();
	KillWindow();


    CoUninitialize();

	//Exit happily
	return 0;
}


// Process any messages that Windows has sent us
void MessagePump(void)
{
	MSG msg;

	if(PeekMessage(&msg, NULL, 0, 0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}



// Should we run in full-screen or windowed mode?
bool GetFullscreen(void)
{
	int iResult;
	bool bRet = true;

	iResult = MessageBox(NULL, "Run in fullscreen mode?", APP_NAME, MB_YESNOCANCEL|MB_ICONQUESTION);
	switch(iResult)
	{
		case IDCANCEL:
			bRet = false;
			break;
		case IDNO:
			gbFullscreen = false;
			break;
		case IDYES:
			gbFullscreen = true;
			break;
		case 0:
			OutputDebugString("Couldn't open MessageBox, dying");
			exit(10);
		break;
	}
	
	return bRet;
}



// Initialise a window (full-screen or otherwise) in which our graphics will be displayed.
bool InitWindow(void)
{
	int iWidth, iHeight;
	WNDCLASS WindowClass;
	DWORD dwStyle;

	// Fill in all the fields for the WNDCLASS structure.
	WindowClass.style          = CS_OWNDC;
	WindowClass.cbClsExtra     = 0;
	WindowClass.cbWndExtra     = 0;
	WindowClass.hInstance      = ghInstance;
	WindowClass.hIcon          = LoadIcon(NULL,IDI_APPLICATION);
	WindowClass.hCursor        = LoadCursor(NULL,IDC_ARROW);
	WindowClass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WindowClass.lpszMenuName   = NULL;
	WindowClass.lpszClassName  = APP_NAME;
	WindowClass.lpfnWndProc    = WindowProc;


	// Register the class with windows
	if(!RegisterClass(&WindowClass))
	{
		OutputDebugString("Error registering window class");
		return false;
	}

	// If we're running full screen, we cover the desktop with our window.
	// This isn't necessary, but it provides a smoother transition for the
	// user, especially when we're going to change screen modes.
	if(gbFullscreen)
	{
		iWidth = GetSystemMetrics(SM_CXSCREEN);
		iHeight = GetSystemMetrics(SM_CYSCREEN);
		dwStyle = WS_POPUP;

		ShowCursor(false);		// Hide Mouse Pointer
	}
	else
	{
		//In windowed mode, we just make the window whatever size we need.
		iWidth = WINWIDTH;
		iHeight = WINHEIGHT;
		dwStyle = WS_OVERLAPPED|WS_SYSMENU;
	}


	// Create the window
	ghAppWindow=CreateWindow(APP_NAME, APP_NAME, dwStyle,
			  0, 0, iWidth, iHeight, NULL, NULL,
			  ghInstance, NULL);

	// All ok?
	if(!ghAppWindow)
	{
		OutputDebugString("Error opening window");
		return false;
	}

	// Make sure that our window is visible and has the input focus.
	ShowWindow(ghAppWindow, SW_SHOW);
	UpdateWindow(ghAppWindow);
	SetFocus(ghAppWindow);

	return true;
}



// Terminate the window that we previously opened
void KillWindow(void)
{
	MSG msg;

	//Test if our window is valid
	if(ghAppWindow)
	{
		if(!DestroyWindow(ghAppWindow))
		{
			//We failed to destroy our window, this shouldn't ever happen
			OutputDebugString("   Failed to DestroyWindow\n");
			MessageBox(NULL, "Destroy Window Failed", APP_NAME, MB_OK|MB_ICONERROR|MB_TOPMOST);
		}
		else
		{
			//Clean up any pending messages
			while(PeekMessage(&msg, NULL, 0, 0,PM_REMOVE))
			{
				DispatchMessage(&msg);
			}
		}

		//Set our window handle to NULL just to be safe
		ghAppWindow = NULL;
	}

	ShowCursor(true);		// Show Mouse Pointer

	//Unregister our window, if we had opened multiple windows using this
	//class, we would have to close all of them before we unregistered the class.
	if(!UnregisterClass(APP_NAME, ghInstance))
	{
		OutputDebugString("   Failed to Unregister Window\n");
		MessageBox(NULL, "Unregister Failed", APP_NAME, MB_OK|MB_ICONERROR|MB_TOPMOST);
	}

}





bool InitD3D(void)
{
	HRESULT hr;
	D3DDISPLAYMODE DisplayMode;

	// Create Direct3D8
	gD3D = Direct3DCreate8( D3D_SDK_VERSION );
	if(!gD3D )
	{
		OutputDebugString("  Error getting Direct3D");
		return false;
	}

	// Get the current(desktop) display mode.  This is really only needed if we're running in a window.
	hr = gD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode);
	if(FAILED(hr))
	{
		OutputDebugString("Error getting display mode\n");
		return false;
	}

	// Clear out our D3DPRESENT_PARAMETERS structure.
	ZeroMemory(&gD3Dpp,sizeof(gD3Dpp));

	// Whether we're full-screen or windowed these are the same.
	gD3Dpp.SwapEffect     = D3DSWAPEFFECT_DISCARD;
	gD3Dpp.hDeviceWindow  = ghAppWindow;
	gD3Dpp.BackBufferCount= 1;

	//We need a Z-Buffer so everything will be drawn properly
	gD3Dpp.EnableAutoDepthStencil = TRUE;
	gD3Dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// BackBufferWidth/Height have to be set for full-screen apps
	// BackBufferFormat is the pixel format we want.  In windowed mode we use the same
	// format as the desktop (which we found by using GetAdapterDisplayMode() above).
	// In full-screen we need to find a pixel format we like, see find_16bit_mode()
	// below for more details.
	if(gbFullscreen)
	{
		gD3Dpp.Windowed          = FALSE;
		gD3Dpp.BackBufferWidth   = WINWIDTH;
		gD3Dpp.BackBufferHeight  = WINHEIGHT;
		gD3Dpp.BackBufferFormat  = FindDisplayMode();
	}
	else
	{
		gD3Dpp.Windowed          = TRUE;
		gD3Dpp.BackBufferFormat  = DisplayMode.Format;
	}

	hr = gD3D->CreateDevice(D3DADAPTER_DEFAULT,					//The default adapter, on a multimonitor system
						  D3DDEVTYPE_HAL,						//Use hardware acceleration rather than the software renderer
						  ghAppWindow,
						  D3DCREATE_SOFTWARE_VERTEXPROCESSING,	//Process vertices in software. This is slower than in hardware,
						  &gD3Dpp,								//Our D3DPRESENT_PARAMETERS structure, so it knows what we want to build
						  &gD3DDevice);							//This will be set to point to the new device
	if(FAILED(hr))
	{
		OutputDebugString("Error creating device\n");
		return false;
	}

	// All done
	return true;
}


// Terminate our D3D objects
void KillD3D(void){

	if(gD3DDevice)
	{
		gD3DDevice->Release();
		gD3DDevice=NULL;
	}

	if(gD3D)
	{
		gD3D->Release();
		gD3D=NULL;
	}

}


D3DFORMAT FindDisplayMode(void)
{
	HRESULT hr;

	// First we test for 32-bit modes (these obviously look best)
	hr = gD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, D3DFMT_A8R8G8B8, FALSE);
	if(SUCCEEDED(hr))
	{
		OutputDebugString("D3DFMT_A8R8G8B8\n");
		return D3DFMT_A8R8G8B8;
	}

	hr = gD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, FALSE);
	if(SUCCEEDED(hr))
	{
		OutputDebugString("D3DFMT_X8R8G8B8\n");
		return D3DFMT_X8R8G8B8;
	}

	// No 32-bit modes available so let's try a 16-bit mode
	hr = gD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_R5G6B5, D3DFMT_R5G6B5, FALSE);
	if(SUCCEEDED(hr))
	{
		OutputDebugString("D3DFMT_R5G6B5\n");
		return D3DFMT_R5G6B5;
	}

	// Next try X1R5G5B5.
	hr = gD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X1R5G5B5, D3DFMT_X1R5G5B5, FALSE);
	if(SUCCEEDED(hr))
	{
		OutputDebugString("D3DFMT_X1R5G5B5\n");
		return D3DFMT_X1R5G5B5;
	}

	// Nothing found that we can work with.
	OutputDebugString("Couldn't find a display mode\n");

	KillD3D();
	KillWindow();

	exit(5);

	//Won't actually hit this line since exit() kills us, but it makes the compiler happy.
	return (D3DFORMAT)NULL;
}



// Initialise DirectX ready for us to start rendering
bool InitScene(void)
{
	D3DXMATRIX view_matrix;
	D3DXMATRIX matProj;

	// Set the camera position (it won't change after this)
	// The camera is positioned a short way back from the origin, looking at the origin.
	D3DXMatrixLookAtLH(&view_matrix,&D3DXVECTOR3( 0.0f, 0.0f, -23.0f ),
							   &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
							   &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ));
	gD3DDevice->SetTransform(D3DTS_VIEW,&view_matrix);

	// The Projection matrix won't change either, so we set it now and never touch it again.
	D3DXMatrixPerspectiveFovLH(&matProj, //Result Matrix
								  D3DX_PI/4,//Field of View, in radians. (PI/4) is typical
								  ((float)WINWIDTH / (float)WINHEIGHT),     //Aspect ratio
								  1.0f,     //Near view plane
								  100.0f ); // Far view plane
	gD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	// Set the device state
	gD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	// Zero out our light structure and fill in the fields we need
	ZeroMemory( &gLight, sizeof(D3DLIGHT8) );
	gLight.Type=D3DLIGHT_DIRECTIONAL;
	gLight.Diffuse.r = gLight.Diffuse.g = gLight.Diffuse.b = 1.0f;
	gLight.Range=1000.0f;
	gLight.Direction=D3DXVECTOR3(-1, -1, 1);
	gD3DDevice->SetLight( 0, &gLight );
	gD3DDevice->LightEnable( 0, TRUE);

	//Ambient light is the default lighting level of the area.  Anything not hit
	//by our light will have this level of lighting.
	gD3DDevice->SetRenderState(D3DRS_AMBIENT,0x00202020);

	// Turn on lighting.
	gD3DDevice->SetRenderState(D3DRS_LIGHTING,TRUE);

	// Set the COLOROP state to take colour values from the texture data
	gD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	gD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_TEXTURE);

	// Disable alpha blending for extra speed
	gD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

	// Set the magnification filter to Linear for good-quality filtering
	gD3DDevice->SetTextureStageState(0,D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	gD3DDevice->SetTextureStageState(0,D3DTSS_MINFILTER, D3DTEXF_LINEAR);

	// Set the custom vertex shader that we've defined
	gD3DDevice->SetVertexShader(D3D8T_CUSTOMVERTEX);

	return true;
}




void KillScene(void)
{
}


// Initialise the 3d objects we will be using
bool InitObjects(void)
{
	// Create the Vertex Buffers and copy the vertex data to them
	BuildVertexBuffer(&vbCube, gCubeVertices, NUM_VERTICES_CUBE);
	BuildVertexBuffer(&vbFloor, gFloorVertices, NUM_VERTICES_FLOOR);

	return true;
}

// Dispose of the objects we previously initialised
void KillObjects(void)
{
	// Release anything previously set into the vertex buffer
	if(vbCube)
	{
		vbCube->Release();
		vbCube=NULL;
	}
	if(vbFloor)
	{
		vbFloor->Release();
		vbFloor=NULL;
	}
}


// Initialise the Tokamak physics engine.
// Here's where the interesting stuff starts.
bool InitPhysics(void)
{
	neGeometry *geom;	// Pointer to a Geometry object which we'll use to define the shape/size of each cube
	neV3 boxSize1;		// A variable to store the length, width and height of the cube
	neV3 gravity;		// A vector to store the direction and intensity of gravity
	neV3 pos;			// The position of a cube
	f32 mass;			// The mass of our cubes
	neSimulatorSizeInfo sizeInfo;	// SizeInfo stores data about how many objects we are going to model
	int i;

	// Create and initialise the simulator
	// Tell the simulator how many rigid bodies we have
	sizeInfo.rigidBodiesCount = CUBECOUNT;
	// Tell the simulator how many animated bodies we have
	sizeInfo.animatedBodiesCount = 1;
	// Tell the simulator how many bodies we have in total
	s32 totalBody = sizeInfo.rigidBodiesCount + sizeInfo.animatedBodiesCount;
	sizeInfo.geometriesCount = totalBody;
	// The overlapped pairs count defines how many bodies it is possible to be in collision
	// at a single time. The SDK states this should be calculated as:
	//   bodies * (bodies-1) / 2
	// So we'll take its word for it. :-)
	sizeInfo.overlappedPairsCount = totalBody * (totalBody - 1) / 2;
	// We're not using any of these so set them all to zero
	sizeInfo.rigidParticleCount = 0;
	sizeInfo.constraintsCount = 0;
	sizeInfo.terrainNodesStartCount = 0;

	// Set the gravity. Try changing this to see the effect on the objects
	gravity.Set(0.0f, -10.0f, 0.0f);

	// Ready to go, create the simulator object
	gSim = neSimulator::CreateSimulator(sizeInfo, NULL, &gravity);

	// Now we need to add some other elements to the physics engine, namely
	// the cubes and the floor.

	// Create rigid bodies for the cubes
	for (i=0; i<CUBECOUNT; i++)
	{
		// Create a rigid body
		gCubes[i] = gSim->CreateRigidBody();
		// Add geometry to the body and set it to be a box of dimensions 1, 1, 1
		geom = gCubes[i]->AddGeometry();
		boxSize1.Set(1.0f, 1.0f, 1.0f);
		geom->SetBoxSize(boxSize1[0], boxSize1[1], boxSize1[2]);
		// Update the bounding info of the object -- must always call this
		// after changing a body's geometry.
		gCubes[i]->UpdateBoundingInfo();

		// Set other properties of the object (mass, position, etc.)
		mass = 1.0f;
		gCubes[i]->SetInertiaTensor(neBoxInertiaTensor(boxSize1[0], boxSize1[1], boxSize1[2], mass));
		gCubes[i]->SetMass(mass);
		// Vary the position so the cubes don't all exactly stack on top of each other
		// (makes for a more interesting simulation)
		pos.Set((float)(rand()%10) / 100, 4.0f + i*2.0f, (float)(rand()%10) / 100);
		gCubes[i]->SetPos(pos);
	}

	// Create an animated body for the floor
	gFloor = gSim->CreateAnimatedBody();
	// Add geometry to the floor and set it to be a box with size as defined by the FLOORSIZE constant
	geom = gFloor->AddGeometry();
	boxSize1.Set(FLOORSIZE, 0.2f, FLOORSIZE);
	geom->SetBoxSize(boxSize1[0],boxSize1[1],boxSize1[2]);
	gFloor->UpdateBoundingInfo();
	// Set the position of the box within the simulator
	pos.Set(0.0f, -3.0f, 0.0f);
	gFloor->SetPos(pos);

	// All done
	return true;
}


void KillPhysics(void)
{
	if (gSim)
	{
		// Destroy the simulator.
		// Note that this will release all related resources that we've allocated.
		neSimulator::DestroySimulator(gSim);
		gSim = NULL;
	}
}



// Initialise a timer for use in our application.
// Wherever possible use a Performance timer as this
// gives the best accuracy.
// If this isn't available, fall back to the multimedia timer.
// In each case, note the number of "ticks" per second so that
// we can divide the elapsed time by this value to determine
// how much real time has actually passed.
// For more information on using these timers, see the articles at
// www.mvps.org/directx
bool InitTimer(void)
{
	INT64 TimerFrequency;

	// Can we use the high performance timer?
	if (QueryPerformanceFrequency( (LARGE_INTEGER*)&TimerFrequency))
	{
		// High performance timer available, let's get ready to use it
		gfTimeScale = 1.0f / TimerFrequency;
		QueryPerformanceCounter((LARGE_INTEGER *) &gCurrentTime);
		gbUseHFTimer = true;
		return true;
	}
	else
	{
		// No high precision timer to be seen, let's use the Windows multimedia timer instead
		gfTimeScale = 0.001f;
		gCurrentTime = timeGetTime();
		gbUseHFTimer = false;
		return true;
	}

	// No timer available at all (should never happen)
	return false;
}


// Use the timer we previously initialised to determine
// how much time (in seconds, or a fraction thereof) has
// passed since this function was last called.
float GetElapsedTime()
{
	INT64 newTime;
	float fElapsed;

	if (gbUseHFTimer) 
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &newTime);
	}
	else 
	{
		newTime=timeGetTime();
	}

	// Scale accordingly
	fElapsed = (float)((newTime - gCurrentTime) * gfTimeScale);

	// Save the new time value for the next time we're called
	gCurrentTime = newTime;

	return fElapsed;
}




// Windows message processor
LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM WParam, LPARAM LParam)
{
	HDC hDc;
	PAINTSTRUCT ps;
   
	switch(Msg)
	{
		case WM_KEYDOWN:  // A key has been pressed, end the app
			gbFinished = true;
			return 0;

		case WM_PAINT:    //Repaint our window
			hDc=BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			return 0;

		case WM_CLOSE:    //User hit the Close Window button, end the app
			gbFinished = true;
			return 0;

		case WM_DESTROY:  //This window is being destroyed, tell Windows we're quitting
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWnd, Msg, WParam, LParam);
}



// If the DirectX device is lost for some reason (e.g.,
// the user presses Alt-Tab in a full-screen app),
// we'll use this function to determine whether the
// device is ready to resume its previous display
// processing. If so we'll reinitialise everything
// and return true, otherwise we'll return false.
bool ValidateDevice(void)
{

	HRESULT hr;

	hr = gD3DDevice->TestCooperativeLevel();
	if (FAILED(hr))
	{
		// If the device is lost, return False
		if (hr == D3DERR_DEVICELOST) return false;

		// If the device is waiting to be reset,
		// reset it, reinitialise the scene and return
		// true.
		if (hr == D3DERR_DEVICENOTRESET)
		{
			gD3DDevice->Reset(&gD3Dpp);

			InitScene();

			return true;
		}
	}

	// The device is still working normally, nothing to do.
	return true;
}


// Here comes the graphics bit.
void Render(void)
{
	float fElapsed;
	static float fLastElapsed;
	neT3 t;
	D3DXMATRIX dxTrans;
	int i;


	// Ensure the DirectX device is still ok. If not, sleep for a couple
	// of seconds (to avoid using 100% CPU when we've not got the focus)
	// and then stop until the next time we're called.
	if (!ValidateDevice())
	{
		Sleep(100);
		GetElapsedTime();
		return;
	}

	// Find out how much time has elapsed since we were last called
	fElapsed = GetElapsedTime();

	// In order to keep passing a reasonably stable value into the simulator's Advance
	// method, we'll try to stop occasional variations in the elapsed time from occurring.
	// We'll do this by preventing the elapsed time from being more than 20% greater or
	// less than the previous elapsed time.
	if (fLastElapsed != 0)
	{
		if (fElapsed > fLastElapsed * 1.2f) fElapsed = fLastElapsed * 1.2f;
		if (fElapsed < fLastElapsed * 0.8f) fElapsed = fLastElapsed * 0.8f;
	}
	// To avoid particularly large spikes, we'll stop the elapsed time from exceeding
	// 1/45th of a second. Values larger than this can lead to an unstable simulation.
	if (fElapsed > 1.0f / 45.0f) fElapsed = 1.0f / 45.0f;
	// Store the elapsed time so that we can use it the next time around
	fLastElapsed = fElapsed;

	// Tell the Tokamak simulator to advance all its calculations by one frame.
	// Note that we pass the elapsed time here so that the simulation speed
	// is constant regardless of the refresh frequency, frame rate, etc.
	gSim->Advance(fElapsed);


	//Clear the screen and Z-buffer (make the screen a nice blue colour)
	gD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB(64,64,128), 1.0f, 0 );

	// Notify DX that we're ready to render
	if(SUCCEEDED(gD3DDevice->BeginScene()))
	{

		// Set the vertex stream for the floor
		gD3DDevice->SetStreamSource(0,vbFloor,sizeof(strVertex));

		// Draw the floor...
		// Get the transformation matrix for the floor object
		t = gFloor->GetTransform(); 
		// Transfer the values to a D3DMatrix that we can pass to DirectX
		dxTrans = D3DXMATRIX(	t.rot[0][0], t.rot[0][1], t.rot[0][2], 0.0f, 
								t.rot[1][0], t.rot[1][1], t.rot[1][2], 0.0f, 
								t.rot[2][0], t.rot[2][1], t.rot[2][2], 0.0f, 
								t.pos[0],t.pos[1], t.pos[2], 1.0f );
		// Set the world transformation so that we can draw the floor at the correct position
		gD3DDevice->SetTransform(D3DTS_WORLD, &dxTrans );
		// Render the floor
		gD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);



		// Set the vertex stream for the cube
		gD3DDevice->SetStreamSource(0,vbCube,sizeof(strVertex));

		// Draw the cubes
		for (i=0; i<CUBECOUNT; i++)
		{
			// Get the transformation matrix for this cube
			t = gCubes[i]->GetTransform(); 
			// Transfer the values to a D3DMATRIX that we can pass to DirectX
			dxTrans = D3DXMATRIX(	t.rot[0][0], t.rot[0][1], t.rot[0][2], 0.0f, 
									t.rot[1][0], t.rot[1][1], t.rot[1][2], 0.0f, 
									t.rot[2][0], t.rot[2][1], t.rot[2][2], 0.0f, 
									t.pos[0],t.pos[1], t.pos[2], 1.0f );
			// Set the world transformation so that we can draw the cube at the correct position
			gD3DDevice->SetTransform(D3DTS_WORLD, &dxTrans );
			// Render the cube
			gD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,12);
		}


		//Notify the device that we're finished rendering for this frame
		gD3DDevice->EndScene();
	}

	//Show the results
	gD3DDevice->Present( NULL, NULL, NULL, NULL );

	// All done!
}
