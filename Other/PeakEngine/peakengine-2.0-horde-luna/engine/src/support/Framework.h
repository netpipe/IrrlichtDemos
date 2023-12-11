#pragma once

//#define OPENGL4 1

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <SDL2/SDL.h>
#undef main

#include "Horde3D.h"
#include "Horde3DUtils.h"
#include "utMath.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
const char *contentDir = "Content";
#else
const char *contentDir = "../Content";
#endif

class Movable
{
public:
	H3DNode node = -1;
	Horde3D::Vec3f pos, rot, scale = Horde3D::Vec3f(1, 1, 1);

	const float piOver180 = (float)(M_PI / 180.0f);

	Horde3D::Vec3f GetDirection(float scale)
	{
		Horde3D::Vec3f rotation = rot * piOver180;
		Horde3D::Vec3f direction;
		direction.x = (float)sin(rotation.y) * (float)cos(-rotation.x) * scale;
		direction.y = (float)sin(-rotation.x) * scale;
		direction.z = (float)cos(rotation.y) * (float)cos(-rotation.x) * scale;
		return -direction;
	}

	virtual void Update()
	{
		h3dSetNodeTransform(node,
			pos.x, pos.y, pos.z,
			rot.x, rot.y, rot.z,
			scale.x, scale.y, scale.z);
	}

	void Forward(float spd)
	{
		Horde3D::Vec3f rotation = rot * piOver180;
		pos.x -= (float)sin(rotation.y) * (float)cos(-rotation.x) * spd;
		pos.y -= (float)sin(-rotation.x) * spd;
		pos.z -= (float)cos(rotation.y) * (float)cos(-rotation.x) * spd;
	}

	void Strafe(float spd)
	{
		Horde3D::Vec3f rotation = rot * piOver180;
		pos.x -= (float)sin(rotation.y - 90.0f * piOver180) * spd;
		pos.z -= (float)cos(rotation.y - 90.0f * piOver180) * spd;
	}

	void Strafe(float spd, float angle)
	{
		angle *= piOver180;
		Horde3D::Vec3f rotation = rot * piOver180;
		pos.x -= (float)sin(rotation.y + angle) * spd;
		pos.z -= (float)cos(rotation.y + angle) * spd;
	}

};

class Model : public Movable
{
public:
	void Load(int parent, const char *fileName)
	{
		int resource = h3dAddResource((int)H3DResTypes::SceneGraph, fileName, 0);
		h3dutLoadResourcesFromDisk(contentDir);
		node = h3dAddNodes(parent, resource);
	}

};

class AnimatedModel : public Model
{
	float animTime = 0;
	int *anims;

public:
	void LoadAnimations(const char *fileNames[], int numOfNames)
	{
		//int numOfNames =  sizeof fileNames / sizeof fileNames[0];

		anims = new int[numOfNames];
		for (int q = 0; q < numOfNames; q++)
		{
			anims[q] = h3dAddResource((int)H3DResTypes::Animation, fileNames[q], 0);
			h3dutLoadResourcesFromDisk(contentDir);
			h3dSetupModelAnimStage(node, q, anims[q], 0, "", false);
		}
	}

	float UpdateAnimTime(float animtime)
	{
		animTime += animtime;
		return animTime;
	}

	void UpdateAnim(int animId, float animWeight)
	{
		h3dSetModelAnimParams(node, animId, animTime, animWeight);
		h3dUpdateModel(node, (int)(H3DModelUpdateFlags::Animation | H3DModelUpdateFlags::Geometry));
	}

	/*
	void UpdateMorphs(const char *targetNames[], float morph[], int morphs)
	{
		// TODO test
		for (int q = 0; q < morphs; q++)
			h3dSetModelMorpher(node, targetNames[q], morph[q]);

		h3dUpdateModel(node, (int)(H3DModelUpdateFlags::Geometry));
	}
	*/

	void Dispose()
	{
		animTime = -1;
		//TODO base.Dispose();
	}
};


class Light : public Movable
{
	int lightMatRes = -1;

public:
	float fov = 90;
	float radius = 200;
	Horde3D::Vec3f color = Horde3D::Vec3f(1, 1, 1);

	int shadowMapCountI = 2;
	float shadowSplitLambdaF = 0.5f, shadowMapBiasF = 0.001f;
	bool createShadow = true;

	void Init(int parent, const char *name, Horde3D::Vec3f position, Horde3D::Vec3f rotation)
	{
		pos = position;
		rot = rotation;

		lightMatRes = h3dAddResource((int)H3DResTypes::Material, "materials/light.material.xml", 0);

		node = h3dAddLightNode(parent, name, lightMatRes, "LIGHTING", "SHADOWMAP");
		h3dSetNodeTransform(node, position.x, position.y, position.z, rotation.x, rotation.y, rotation.z, 1, 1, 1);
		h3dSetNodeParamF(node, (int)H3DLight::RadiusF, 0, radius);
		h3dSetNodeParamF(node, (int)H3DLight::FovF, 0, fov);
		h3dSetNodeParamF(node, (int)H3DLight::ColorF3, 0, color.x);
		h3dSetNodeParamF(node, (int)H3DLight::ColorF3, 1, color.y);
		h3dSetNodeParamF(node, (int)H3DLight::ColorF3, 2, color.z);

		if (createShadow)
		{
			h3dSetNodeParamI(node, (int)H3DLight::ShadowMapCountI, shadowMapCountI);
			h3dSetNodeParamF(node, (int)H3DLight::ShadowSplitLambdaF, 0, shadowSplitLambdaF);
			h3dSetNodeParamF(node, (int)H3DLight::ShadowMapBiasF, 0, shadowMapBiasF);
		}
	}

};


class Camera : public Movable
{
	H3DNode pipeline;
	Model skybox;

public:
	float near = 0.1f, far = 1000, fov = 45;

	float speed = 0.005f;
	float deltaX = 0, deltaY = 0;

	void Init(int width, int height)
	{
		pipeline = h3dAddResource(H3DResTypes::Pipeline, "pipelines/forward.pipeline.xml", 0);
		node = h3dAddCameraNode(H3DRootNode, "Camera", pipeline);
		Resize(width, height);
	}

	void Resize(int width, int height)
	{
		// Resize viewport
		h3dSetNodeParamI(node, H3DCamera::ViewportXI, 0);
		h3dSetNodeParamI(node, H3DCamera::ViewportYI, 0);
		h3dSetNodeParamI(node, H3DCamera::ViewportWidthI, width);
		h3dSetNodeParamI(node, H3DCamera::ViewportHeightI, height);

		// Set virtual camera parameters
		h3dSetupCameraView(node, fov, (float)width / height, near, far);
		h3dResizePipelineBuffers(pipeline, width, height);
	}

	void SetSkyBox(const char *name)
	{
		skybox.Load(node, name);
		skybox.scale = Horde3D::Vec3f(100, 100, 100);
		h3dSetNodeFlags(skybox.node, H3DNodeFlags::NoCastShadow, true);
		skybox.Update();
	}
};

struct Program
{
	SDL_Window *window;
	SDL_Renderer *renderer;

	bool mouseButtons[3] = { 0,0,0 };
	int mouseX = 0, mouseY = 0;
	int oldMouseX = 0, oldMouseY = 0;
	int mouseDX = 0;
	int mouseDY = 0;

	int shadowMapSize = 1024;

	bool running = true;

	int Init(const char *windowTitle, int screenWidth, int screenHeight)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			printf("SDL_Init failed: %s\n", SDL_GetError());
			return 0;
		}

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
#ifdef OPENGL4
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
#else
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#endif
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		int flags = SDL_WINDOW_OPENGL; // | SDL_WINDOW_FULLSCREEN
		if (SDL_CreateWindowAndRenderer(screenWidth, screenHeight, flags, &window, &renderer) == -1)
		{
			printf("SDL_CreateWindowAndRenderer failed: %s\n", SDL_GetError());
			return 0;
		}

		SDL_GLContext glcontext = SDL_GL_CreateContext(window);
		SDL_SetWindowTitle(window, windowTitle);

#ifdef __EMSCRIPTEN__
		h3dInit(0);
#else
#ifdef OPENGL4
		h3dInit(H3DRenderDevice::OpenGL4);
#else
		h3dInit(H3DRenderDevice::OpenGL2);
#endif
#endif

		h3dSetOption(H3DOptions::LoadTextures, 1);
		h3dSetOption(H3DOptions::MaxAnisotropy, 4);
		h3dSetOption(H3DOptions::ShadowMapSize, shadowMapSize);
		h3dSetOption(H3DOptions::FastAnimation, 0);

		return 1;
	}

	void Dispose()
	{
		SDL_Quit();
	}

	void SwapWindow()
	{
		SDL_GL_SwapWindow(window);
	}


	unsigned int lastTime = 0, currentTime, deltaTime = 0;
	int mouseOK = 0;
	float OnUpdate()
	{
		SDL_Event e;

		currentTime = SDL_GetTicks();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;


		if (mouseOK)
		{
			mouseDX = mouseX - oldMouseX;
			mouseDY = mouseY - oldMouseY;
		}
		oldMouseX = mouseX;
		oldMouseY = mouseY;

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				running = false;
				break;


			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = false;
					break;
				}
				break;

			case SDL_MOUSEMOTION:
				if (oldMouseX != 0 && oldMouseY != 0) mouseOK = 1;
				mouseX = e.motion.x;
				mouseY = e.motion.y;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mouseButtons[e.button.button] = 1;
				break;

			case SDL_MOUSEBUTTONUP:
				mouseButtons[e.button.button] = 0;
				break;

			}
		}

		return (float)deltaTime / 1000.0f;
	}

	void EnterLoop(void(*update)())
	{
#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(update, 0, 0);
#else
		while (running)
		{
			update();
		}
		Dispose();
#endif
	}

};
