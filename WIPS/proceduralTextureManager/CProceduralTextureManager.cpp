// Copyright (C) 2006 Cestmir "CZestmyr" Houska
// This file uses the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#include "CProceduralTextureManager.h"

namespace irr{
namespace scene{

ProcTexture::ProcTexture(ITimer* Timer, video::IVideoDriver* Driver, s32 TexX, s32 TexY, u32 Step) : timer(Timer), driver(Driver), texX(TexX), texY(TexY), timer1(0), step(Step)
{
	prevTime = timer->getTime();
	texture = driver->addTexture(core::dimension2d<s32>(texX, texY), "proceduralTexture", video::ECF_R8G8B8);
	texY_1 = texY - 1;
	texX_1 = texX - 1;
}

ProcTexture::~ProcTexture() {driver->removeTexture(texture);}

u8 ProcTexture::avg(u8& a, u8& b, u8& c){
    s32 ret = 0;
    ret += a;
    ret += b;
    ret += c;
    ret /= 3;
    return (u8)ret;
}

void ProcTexture::animate()
{
	u32 dt = timer->getTime() - prevTime;
	prevTime += dt;
	timer1 += dt;
	while (timer1 > step){
		launchProcedure();
		timer1 -= step;
	}
}

TestProcTexture::TestProcTexture(ITimer* Timer, video::IVideoDriver* Driver, s32 TexX, s32 TexY, u32 Step) : ProcTexture(Timer, Driver, TexX, TexY, Step)
{
	s32 pixelSize = texture->getPitch() / texX;
	u8* pixels = (u8*)texture->lock();
	for (s32 i = 0; i < texY; i++){
		u8 pixVal = i%255;
		for (s32 j = 0; j < texX; j++){
			for (u8 k = 0; k != 3; k++){
				*pixels = pixVal;
				pixels++;
			}
			pixels++;
		}
	}
	texture->unlock();
}

void TestProcTexture::launchProcedure()
{
	s32 texPitch = texture->getPitch();
	s32 pixelSize = texPitch / texX;
	u8* pixels = (u8*)texture->lock();
	s32 pixelAddr = 0;
	s32 pixelAddr1 = 1;
	s32 pixelAddr2 = 2;
	for (s32 i = 0; i < texY; i++){
		for (s32 j = 0; j < texX; j++){
			if (i != texY_1)
			pixels[pixelAddr] = pixels[pixelAddr1] = pixels[pixelAddr2] = pixels[pixelAddr+texPitch];
			else
			pixels[pixelAddr] = pixels[pixelAddr1] = pixels[pixelAddr2] = pixels[0];
			pixelAddr += pixelSize;
			pixelAddr1 += pixelSize;
			pixelAddr2 += pixelSize;
		}
	}
	texture->unlock();
}

FireProcTexture::FireProcTexture(ITimer* Timer, video::IVideoDriver* Driver, s32 TexX, s32 TexY, u32 Step, u8 fadecol, f32 fr, c8* coolingTextureFilename) : ProcTexture(Timer, Driver, TexX, TexY, Step), offsetY(0), faderate(fr), fadecolor(fadecol)
{
	printf("Adding FireProcTexture %s\n", coolingTextureFilename);
	faderate_2 = faderate*faderate;
	coolingTexture = driver->getTexture(coolingTextureFilename);
	coolTexDimension = coolingTexture->getSize();
	s32 pixelSize = texture->getPitch() / texX;
	u8* pixels = (u8*)texture->lock();
	for (s32 i = 0; i < texY; i++){
		for (s32 j = 0; j < texX; j++){
			for (u8 k = 0; k != 3; k++){
				*pixels = 0;
				pixels++;
			}
			pixels++;
		}
	}
	texture->unlock();
}

void FireProcTexture::launchProcedure()
{
	u8* pixels = (u8*)texture->lock();
	u8* coolingPixels = (u8*)coolingTexture->lock();
	for (s32 i = 1; i < texY; i++){
		pixels+=4;
		for (s32 j = 1; j < texX_1; j++){
			for (u8 c = 0; c < 3; c++){
				u8* prevRow = pixels + texX*4;
				if (c == fadecolor)
				*pixels = (u8)(avg(*(prevRow-4),*(prevRow),*(prevRow+4))*faderate);
				else
				*pixels = (u8)(avg(*(prevRow-4),*(prevRow),*(prevRow+4))*faderate_2);
				pixels++;
			}
			pixels++;
		}
		pixels+=4;
	}
	coolingPixels += offsetY*coolTexDimension.Width*4;
	for (s32 i = 0; i < texX; i++){
		for (u8 c = 0; c < 4; c++){
			*pixels = *coolingPixels;
			pixels++;
			coolingPixels++;
		}
		if (i % coolTexDimension.Width == (coolTexDimension.Width-1)) coolingPixels -= coolTexDimension.Width*4;
	}
	offsetY++;
	offsetY %= coolTexDimension.Height;
	texture->unlock();
	coolingTexture->unlock();
}

CProceduralTextureManager::CProceduralTextureManager(ISceneNode* parent, ISceneManager* Smgr, ITimer* Timer, s32 Id) :
ISceneNode(parent, Smgr, Id), timer(Timer), bbox(core::aabbox3d<f32>(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f))
{
	textures = new ProcTexture*[_MX_PROC_TEXTURES_];
	for (u8 i = 0; i < _MX_PROC_TEXTURES_; i++){
		textures[i] = NULL;
	}
}

CProceduralTextureManager::~CProceduralTextureManager()
{
	for (u8 i = 0; i < _MX_PROC_TEXTURES_; i++){
		delete textures[i];
	}
	delete[] textures;
}

const core::aabbox3d<f32>& CProceduralTextureManager::getBoundingBox() const
{
	return bbox;
}

void CProceduralTextureManager::render()
{
	return;
}

//void CProceduralTextureManager::OnPreRender()
void CProceduralTextureManager::OnRegisterSceneNode()
{
//OnPreRender is called, even if the node is not visible. Thus, animation is done here
	for (u8 i = 0; i < _MX_PROC_TEXTURES_; i++){
		if (textures[i] != NULL){
			textures[i]->animate();
		}
	}
}

video::ITexture* CProceduralTextureManager::getTexture(u8 num)
{
	if (num >= _MX_PROC_TEXTURES_ || textures[num] == NULL){
		printf("textures[num] == NULL\n");
		return NULL;
	}
	else
		printf("\n\tReturning texture: %s\n\n", textures[num]->texture->getName().c_str());
	return textures[num]->texture;
}

bool CProceduralTextureManager::deleteTexture(u8 num)
{
	if (num >= _MX_PROC_TEXTURES_ || textures[num] == NULL) return false;
	delete textures[num];
	return true;
}
	
u8 CProceduralTextureManager::addTestTexture(s32 x, s32 y, u32 step)
{
	for (u8 i = 0; i < _MX_PROC_TEXTURES_; i++){
		if (textures[i] == NULL){
			textures[i] = new TestProcTexture(timer, SceneManager->getVideoDriver(), x, y, step);
			return i;
		}
	}
	return _MX_PROC_TEXTURES_;
}

u8 CProceduralTextureManager::addFireTexture(c8* coolingTextureFilename, s32 x, s32 y, f32 fadeRate, u8 fadeColor, u32 step)
{
	for (u8 i = 0; i < _MX_PROC_TEXTURES_; i++){
		if (textures[i] == NULL){
			textures[i] = new FireProcTexture(timer, SceneManager->getVideoDriver(), x, y, step, fadeColor, fadeRate, coolingTextureFilename);
			return i;
		}
	}
	return _MX_PROC_TEXTURES_;
}
	
}
}
