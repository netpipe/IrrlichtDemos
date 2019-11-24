// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef __IRRBOX__IRRNWET_HPP
#define __IRRBOX__IRRNWET_HPP

#include "irrtonewt.hpp"
#include "irrnewt_base.hpp"

#include <irrlicht.h>
using namespace irr;


class IrrBox : public scene::ISceneNode, public newton::IrrNewt_Base
{

protected:

	//don't call delete since this is a ISceneNode derived class!
   virtual bool call_delete() {return false;}

   void reserved_destroy() { /*the obj is alredy deleted when arrive here*/ this->~IrrBox(); }

   core::aabbox3d<f32> Box;
   video::S3DVertex Vertices[24];
   video::SMaterial Material;

public:

   IrrBox (scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, float x, float y, float z);
   ~IrrBox() {}

   virtual void render();
   virtual const core::aabbox3d<f32>& getBoundingBox() const { return Box; }

   //see irrtonewt.hpp to choose your irrlicht version

     #if defined (COMPILE_WITH_1_5) && defined (COMPILE_WITH_1_3)

    //1.5 implementation
    //if get an error here, see irrtonewt.hpp

    virtual void OnRegisterSceneNode();
    virtual u32 getMaterialCount() const { return 1; }
	virtual video::SMaterial& getMaterial(u32 i) { return Material; }

	#endif

    #if defined (COMPILE_WITH_1_3) && !defined (COMPILE_WITH_1_5)

    //1.3 implementation
	//if get an error here, see irrtonewt.hpp

    virtual void OnAnimate(u32 timeMs);
    virtual u32 getMaterialCount() { return 1; }
	virtual video::SMaterial& getMaterial(u32 i) { return Material; }

    #endif

};


#endif
