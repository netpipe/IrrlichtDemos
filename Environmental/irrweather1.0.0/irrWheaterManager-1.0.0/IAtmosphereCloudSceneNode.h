//! The class was written by Gawaboumga (Copyright (C) 20013-2014 Youri Hubaut).

#ifndef __IAtmosphereCloudSceneNode_H_INCLUDED__
#define __IAtmosphereCloudSceneNode_H_INCLUDED__

#include "CloudSceneNode.h"

namespace irr
{

class IAtmosphereCloudSceneNode
{
    public:
        //! Constructor.
        IAtmosphereCloudSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr,
            video::ITexture* cloud1, video::ITexture* cloud2, video::ITexture* cloud3);

        //! Destructor.
		~IAtmosphereCloudSceneNode();

        void ShowClouds(bool cloud1, bool cloud2, bool cloud3);

	private:

	    scene::ISceneNode* Parent;
	    scene::ISceneManager* smgr;

	    video::ITexture* CloudTexture1;
	    video::ITexture* CloudTexture2;
	    video::ITexture* CloudTexture3;

	    CCloudSceneNode* CloudLayer1;
        CCloudSceneNode* CloudLayer2;
        CCloudSceneNode* CloudLayer3;
};

} // namespace

#endif // __IAtmosphereCloudSceneNode_H_INCLUDED__
