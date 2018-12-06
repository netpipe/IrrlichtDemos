/*
 * mesh.cpp

    Copyright © 2010 John Serafino
    This file is part of ray3d.

    Ray3d is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ray3d v0.01 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with ray3d If not, see <http://www.gnu.org/licenses/>.
 */

#include "mesh.h"


  HWSkinCB hwSkinInstance;

  Entity::Entity() {}


  // Transformations //
  /////////////////////

  // local
  void Entity::rotate(float x, float y, float z)
  {
	if(type == LIGHT_TYPE || type == MAP_TYPE)
	  rotateNode(lightNode, vector3df(x,y,z));
	else if(type == ANIM_TYPE)
	  rotateNode(animNode, vector3df(x,y,z));
	else
	{
      rotateNode(sceneNode, vector3df(x,y,z));
      sceneNode->updateAbsolutePosition();
	}
  }
  // local
  void Entity::translate(float x, float y, float z)
  {
    vector3df pos;
	if(type == LIGHT_TYPE)
	{
	  translateNode(lightNode, vector3df(x,y,z));
	  lightNode->updateAbsolutePosition();
	  pos = lightNode->getPosition();
	}
	else if(type == ANIM_TYPE)
	{
	  translateNode(animNode, vector3df(x,y,z));
	  animNode->updateAbsolutePosition();
	  pos = animNode->getPosition();
	}
	else
	{
      translateNode(sceneNode, vector3df(x,y,z));
      sceneNode->updateAbsolutePosition();
      pos = sceneNode->getPosition();
	}

	x = pos.X;
	y = pos.Y;
	z = pos.Z;
  }

  // global or relative to parent
  void Entity::setScale(float x, float y, float z)
  {
    if(type == MESH_TYPE || type == MAP_TYPE)
      sceneNode->setScale(vector3df(x,y,z));
    else if(type == ANIM_TYPE)
      animNode->setScale(vector3df(x,y,z));
  }
  // global or relative to parent
  void Entity::translateGlobal(f32 x, f32 y, f32 z)
  {
	if(type == LIGHT_TYPE)
	  lightNode->setPosition(vector3df(x,y,z) + lightNode->getPosition());
	else if(type == ANIM_TYPE)
	  animNode->setPosition(vector3df(x,y,z) + animNode->getPosition());
	else
      sceneNode->setPosition(vector3df(x,y,z) + sceneNode->getPosition());
  }
  void Entity::rotateGlobal(f32 x, f32 y, f32 z)
  {
	if(type == LIGHT_TYPE)
	  lightNode->setRotation(vector3df(x,y,z) + lightNode->getRotation());
	else if(type == ANIM_TYPE)
	  animNode->setRotation(vector3df(x,y,z) + animNode->getRotation());
	else
      sceneNode->setRotation(vector3df(x,y,z) + sceneNode->getRotation());
  }
  void Entity::setRotation(float x, float y, float z)
  {
	if(type == LIGHT_TYPE)
	  lightNode->setRotation(vector3df(x,y,z));
	else if(type == ANIM_TYPE)
	  animNode->setRotation(vector3df(x,y,z));
	else
      sceneNode->setRotation(vector3df(x,y,z));
  }
  // global or relative to parent
  void Entity::setPosition(float x, float y, float z)
  {
	vector3df pos;

	if(type == LIGHT_TYPE)
	{
	  lightNode->setPosition(vector3df(x,y,z));
	  pos = lightNode->getPosition();
	}
	else if(type == ANIM_TYPE)
	{
	  animNode->setPosition(vector3df(x,y,z));
	  pos = animNode->getPosition();
	}
	else
	{
      sceneNode->setPosition(vector3df(x,y,z));
      pos = sceneNode->getPosition();
	}

	x = pos.X;
	y = pos.Y;
	z = pos.Z;
  }


  void Entity::setRadius(f32 r)
  {
    if(type == LIGHT_TYPE)
      lightNode->setRadius(r);
  }

  // Creation //
  //////////////

  void Entity::createMap()
  {
    type = MAP_TYPE;

    rx=ry=rz=0;
    isVisible = true;
    lit = true;
  }
  void Entity::createMesh()
  {
    type = MESH_TYPE;
    rx=ry=rz=0;
    isVisible = true;
    lit = true;

    //sceneNode = Scene->addSceneNode();
  }
  void Entity::createLight(E_LIGHT_TYPE typ, f32 r, f32 g, f32 b, f32 a)
  {
    type = LIGHT_TYPE;
    lightNode = Scene->addLightSceneNode(0, vector3df(0,0,0), SColorf(r,g,b,a), 100.0f);
    lightNode->setLightType(typ);
    lightNode->enableCastShadow();
    rx=ry=rz=0;
    isVisible = true;
  }
  void Entity::createAnimMesh()
  {
    type = ANIM_TYPE;
    rx=ry=rz=0;
    isVisible = true;
    lit = true;
  }

  void Entity::createParticleSystem(Entity *parent, bool def, f32 sizeX, f32 sizeY, int fadetime)
  {
	type = PARTICLE_SYS_TYPE;
	rx=ry=rz=x=y=z=0;
	lit = false;
	isVisible = true;

	if(parent == NULL)
		  pSys = Scene->addParticleSystemSceneNode(def, 0);
	else if(parent->type == ANIM_TYPE)
		  pSys = Scene->addParticleSystemSceneNode(def, parent->animNode);
	else if(parent->type == MESH_TYPE || parent->type == MAP_TYPE)
		  pSys = Scene->addParticleSystemSceneNode(def, parent->sceneNode);
	else if(parent->type == PARTICLE_SYS_TYPE)
		  pSys = Scene->addParticleSystemSceneNode(def, parent->pSys);
	else if(parent->type == LIGHT_TYPE)
		  pSys = Scene->addParticleSystemSceneNode(def, parent->lightNode);
	else
		return;

	//pSys->setParticleSize(dimension2d<float>(sizeX, sizeY));
	//pSys->setMaterialTexture(0, Video->getTexture("blueTrail1.png"));
	pSys->setMaterialFlag(EMF_LIGHTING, false);
	//pSys->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	pSys->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	pSys->addAffector(pSys->createFadeOutParticleAffector(SColor(0,0,0,0), fadetime));
  }

  void Entity::addBoxEmitter(vector3df boxDim, vector3df dir, vector2df pps, vector2df size, vector2df life, f32 angle)
  {
	  if(type != PARTICLE_SYS_TYPE)
		  return;

	  vector3df globPos = pSys->getAbsolutePosition();
	  boxDim /= 2;


	 pSys->setEmitter(pSys->createBoxEmitter(aabbox3df(globPos.X - boxDim.X, globPos.Y - boxDim.Y, globPos.Z - boxDim.Z, globPos.X + boxDim.X, globPos.Y + boxDim.Y, globPos.Z + boxDim.Z), dir, pps.X, pps.Y, SColor(255, 0, 0, 0), SColor(255, 255, 255, 255), life.X, life.Y, angle, dimension2df(size.X, size.X), dimension2df(size.Y, size.Y)));
  }
  void Entity::addPointEmitter()
  {
	  if(type != PARTICLE_SYS_TYPE)
		  return;
  }
  void Entity::addSphereEmitter()
  {
	  if(type != PARTICLE_SYS_TYPE)
		  return;
  }
  void Entity::addCylinderEmitter()
  {
	  if(type != PARTICLE_SYS_TYPE)
		  return;
  }
  void Entity::addRingEmitter()
  {
	  if(type != PARTICLE_SYS_TYPE)
		  return;
  }

  void Entity::resetPSys()
  {
	  //pSys->setEmitter(0);
	  pSys->clearParticles();
  }
  void Entity::stopEmitting()
  {
	  pSys->setEmitter(0);
  }


  // data loading //
  //////////////////

  void Entity::copyFrom(Entity ent)
  {
	  /*
	  type = ent.type;
	  tex = ent.tex;

	  sceneNode = Scene->addMeshSceneNode(ent.tmesh);
	  sceneNode->setMaterialFlag(EMF_LIGHTING, lit);
	  sceneNode->setMaterialTexture(0, tex);
	  */

	  type = ent.type;

	  if(type == MESH_TYPE || type == MAP_TYPE)
	  {
		  sceneNode = ent.sceneNode->clone();
	  }
	  if(type == LIGHT_TYPE)
	  {
		  lightNode = (ILightSceneNode *)ent.lightNode->clone();
	  }
	  if(type == ANIM_TYPE)
	  {
		  animNode = (IAnimatedMeshSceneNode *)ent.animNode->clone();
	  }
  }
  void Entity::loadMesh(char *filename, bool planar, bool hwSkin, int skinSpeed)
  {
	if(type == MESH_TYPE || type == MAP_TYPE)
	{
      IAnimatedMesh *mesh;
      mesh = Scene->getMesh(filename);
      if(planar == true)
      {
        Scene->getMeshManipulator()->makePlanarTextureMapping(mesh->getMesh(0), 0.003f);
      }

      tmesh = Scene->getMeshManipulator()->createMeshWithTangents(mesh->getMesh(0));

      sceneNode = Scene->addMeshSceneNode(tmesh);
      sceneNode->setMaterialFlag(EMF_LIGHTING, lit);
	}
	else if(type == ANIM_TYPE)
	{
	  mesh = Scene->getMesh(filename);
      if(planar == true)
      {
        Scene->getMeshManipulator()->makePlanarTextureMapping(mesh->getMesh(0), 0.003f);
      }
      Scene->getMeshManipulator()->createMeshWithTangents(mesh->getMesh(0));

	  animNode = Scene->addAnimatedMeshSceneNode(mesh);
	  animNode->setMaterialFlag(EMF_LIGHTING, lit);

	  if(hwSkin)
		  //hwSkinInstance.getInstance->setupNode(irrlicht, animNode);
		  HWSkinCB::getInstance()->setupNode(irrlicht, animNode, 60);
	}
  }
  void Entity::loadTex(char *filename)
  {
	tex = Video->getTexture(filename);

    if(type == MESH_TYPE || type == MAP_TYPE)
      sceneNode->setMaterialTexture(0, tex);
    else if(type == ANIM_TYPE)
      animNode->setMaterialTexture(0, tex);
    else if(type == PARTICLE_SYS_TYPE)
    {
    	pSys->setMaterialTexture(0, tex);
    }
  }
  void Entity::loadBsp(char *filename)
  {
    if(type == MESH_TYPE || type == MAP_TYPE)
    {
      IAnimatedMesh *mesh = Scene->getMesh(filename);

      sceneNode = Scene->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
      sceneNode->setMaterialFlag(EMF_LIGHTING, true);

      bmesh = mesh;
    }
  }
  void Entity::loadBumpmap(char *filename, f32 h)
  {
    ITexture *normalMap = Video->getTexture(filename);
    Video->makeNormalMapTexture(normalMap, h);

    if(type == MESH_TYPE || type == MAP_TYPE)
    {
      sceneNode->setMaterialTexture(1, normalMap);
      sceneNode->setMaterialType(EMT_PARALLAX_MAP_SOLID);
    }
    if(type == ANIM_TYPE)
    {
      animNode->setMaterialTexture(1, normalMap);
      animNode->setMaterialType(EMT_PARALLAX_MAP_SOLID);
    }
  }

  void Entity::loadBillboard(char *filename, f32 w, f32 h)
  {
    ISceneNode *billboard;

    if(type == LIGHT_TYPE)
    {
      billboard = Scene->addBillboardSceneNode(lightNode, dimension2d<f32>(w,h));
      billboard->setMaterialFlag(EMF_LIGHTING, false);
      billboard->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
      billboard->setMaterialTexture(0, Video->getTexture(filename));
    }
    else if(type == MESH_TYPE)
    {
      sceneNode = Scene->addBillboardSceneNode(0, dimension2d<f32>(w,h));
      sceneNode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
      sceneNode->setMaterialTexture(0, Video->getTexture(filename));
      setLit(false);
    }
    else
      return;
  }


  // spiffy routines //
  /////////////////////

  void Entity::setFrameLoop(int b, int e)
  {
    if(type == ANIM_TYPE)
      animNode->setFrameLoop(b,e);
  }
  void Entity::hide(void)
  {
    if(type == LIGHT_TYPE)
      lightNode->setVisible(false);
    else if(type == ANIM_TYPE)
      animNode->setVisible(false);
    else
      sceneNode->setVisible(false);

    isVisible = false;
  }
  void Entity::show(void)
  {
	if(type == LIGHT_TYPE)
	  lightNode->setVisible(true);
	else if(type == ANIM_TYPE)
	  animNode->setVisible(true);
    else
      sceneNode->setVisible(true);

	isVisible = true;
  }
  void Entity::enableShadow()
  {
	if(type == ANIM_TYPE)
	  animNode->addShadowVolumeSceneNode();
	//else
	  //sceneNode->addShadowVolumeSceneNode();
  }

  void Entity::addChild(Entity &child)
  {
    if(type == MESH_TYPE || type == MAP_TYPE)
    {
      if(child.type == MESH_TYPE || child.type == MAP_TYPE)
        sceneNode->addChild(child.sceneNode);
      if(child.type == ANIM_TYPE)
        sceneNode->addChild(child.animNode);
      if(child.type == LIGHT_TYPE)
        sceneNode->addChild(child.lightNode);
    }
    if(type == ANIM_TYPE)
    {
      if(child.type == MESH_TYPE || child.type == MAP_TYPE)
        animNode->addChild(child.sceneNode);
      if(child.type == ANIM_TYPE)
        animNode->addChild(child.animNode);
      if(child.type == LIGHT_TYPE)
        animNode->addChild(child.lightNode);
    }
    if(type == LIGHT_TYPE)
    {
      if(child.type == MESH_TYPE || child.type == MAP_TYPE)
        lightNode->addChild(child.sceneNode);
      if(child.type == ANIM_TYPE)
        lightNode->addChild(child.animNode);
      if(child.type == LIGHT_TYPE)
        lightNode->addChild(child.lightNode);
    }
  }

  void Entity::setLit(bool toggle)
  {
	if(type == ANIM_TYPE)
      animNode->setMaterialFlag(EMF_LIGHTING, toggle);
	if(type == MESH_TYPE || type == MAP_TYPE)
	  sceneNode->setMaterialFlag(EMF_LIGHTING, toggle);
  }
  void Entity::setWire(bool toggle)
  {
	  if(type == MESH_TYPE || type == MAP_TYPE)
		  sceneNode->setMaterialFlag(EMF_WIREFRAME, toggle);
	  if(type == ANIM_TYPE)
		  animNode->setMaterialFlag(EMF_WIREFRAME, toggle);
  }

/*
  void Entity::addCameraChild(Camera &child)
  {
    if(type == MESH_TYPE)
      sceneNode->addChild(child.camera);
    if(type == ANIM_TYPE)
      animNode->addChild(child.camera);
    if(type == LIGHT_TYPE)
      lightNode->addChild(child.camera);
  }
*/

  void Entity::addCollision(Entity coll, f32 tx, f32 ty, f32 tz,  f32 sx, f32 sy, f32 sz, f32 gx, f32 gy, f32 gz)
  {
    ITriangleSelector* selector = 0;

    if(coll.type == MESH_TYPE)
    {
      selector = Scene->createTriangleSelector(coll.tmesh, coll.sceneNode);
      coll.sceneNode->setTriangleSelector(selector);
    }
    if(coll.type == ANIM_TYPE)
    {
      selector = Scene->createTriangleSelector(coll.animNode->getMesh(), coll.animNode);
      coll.animNode->setTriangleSelector(selector);
    }
    if(coll.type == MAP_TYPE)
    {
      selector = Scene->createOctreeTriangleSelector(coll.bmesh->getMesh(0), coll.sceneNode);
      coll.sceneNode->setTriangleSelector(selector);
    }


    ISceneNodeAnimator* anim;

    if(type == MESH_TYPE)
    {
      anim = Scene->createCollisionResponseAnimator(selector, sceneNode, vector3df(sx,sy,sz), \
        vector3df(gx,gy,gz), vector3df(tx,ty,tz));
      sceneNode->addAnimator(anim);
    }
    if(type == ANIM_TYPE)
    {
      anim = Scene->createCollisionResponseAnimator(selector, animNode, vector3df(sx,sy,sz), \
        vector3df(gx,gy,gz), vector3df(tx,ty,tz));
      animNode->addAnimator(anim);
    }
    if(type == MAP_TYPE)
    {
      anim = Scene->createCollisionResponseAnimator(selector, sceneNode, vector3df(sx,sy,sz), \
        vector3df(gx,gy,gz), vector3df(tx,ty,tz));
      sceneNode->addAnimator(anim);
    }
    else
      return;

    selector->drop();
    anim->drop();
  }

void Entity::update()
{
	/*
	vector3df pos,rot;
	matrix4 m;

	if(type == ANIM_TYPE)
	{
		//pos = animNode->getAbsolutePosition();
		rot = animNode->getRotation();
	}
	if(type == MESH_TYPE || type == MAP_TYPE)
	{
		//pos = sceneNode->getAbsolutePosition();
		rot = sceneNode->getRotation();
	}
	if(type == LIGHT_TYPE)
	{
		//pos = lightNode->getAbsolutePosition();
		rot = lightNode->getRotation();
	}

	x = pos.X;
	y = pos.Y;
	z = pos.Z;

	rx = rot.X;
	ry = rot.Y;
	rz = rot.Z;
	*/
}

vector3df Entity::getRotation()
{
	if(type == MESH_TYPE || type == MAP_TYPE)
		return sceneNode->getAbsoluteTransformation().getRotationDegrees();
	if(type == ANIM_TYPE)
		return animNode->getAbsoluteTransformation().getRotationDegrees();
	else
		return lightNode->getAbsoluteTransformation().getRotationDegrees();
}

vector3df Entity::getPosition()
{
	if(type == MESH_TYPE || type == MAP_TYPE)
		return sceneNode->getPosition();
	if(type == ANIM_TYPE)
		return animNode->getPosition();
	else
		return lightNode->getPosition();
}

vector3df Entity::getGlobalPosition()
{
	if(type == MESH_TYPE || type == MAP_TYPE)
		return sceneNode->getAbsolutePosition();
	if(type == ANIM_TYPE)
		return animNode->getAbsolutePosition();
	else
		return lightNode->getAbsolutePosition();
}
