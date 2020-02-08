// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "mesh_texture_loader.h"
#include <cassert>

MeshTextureLoader::MeshTextureLoader(irr::scene::IMeshTextureLoader* baseLoader, irr::video::IVideoDriver* driver)
: mBaseLoader(baseLoader), mVideoDriver(driver), mTextureCaching(IGNORE), mTextureDivider(1)
{
	assert(mBaseLoader);
	assert(mVideoDriver);
	mBaseLoader->grab();
}

MeshTextureLoader::~MeshTextureLoader()
{
	mBaseLoader->drop();
}

void MeshTextureLoader::setTextureCaching(TextureCaching tc)
{
	mTextureCaching = tc;
}

void MeshTextureLoader::clearTextureCache()
{
	if ( mVideoDriver )
	{
		for ( std::set<irr::video::ITexture*>::iterator it = mTextureCache.begin(); it != mTextureCache.end(); ++it )
		{
			mVideoDriver->removeTexture(*it);
		}
	}
	mTextureCache.clear();
}

void MeshTextureLoader::setTexturePath(const irr::io::path& path)
{
	mBaseLoader->setTexturePath(path);
}

const irr::io::path& MeshTextureLoader::getTexturePath() const
{
	return mBaseLoader->getTexturePath();
}

irr::video::ITexture* MeshTextureLoader::getTexture(const irr::io::path& textureName)
{
	irr::video::ITexture* texture = mBaseLoader->getTexture(textureName);
	if (texture)
	{
		if ( mTextureDivider != 1 && texture->getSource() == irr::video::ETS_FROM_FILE )
		{
			irr::io::path ignored("FB_hover"); // we never downscale hover textures - would save more memory, but that's looking just too ugly
			if ( !textureName.equalsn (ignored, ignored.size()) )
			{
				// Downscale textures (to save memory)
				const irr::core::dimension2d<irr::u32>& texSize = texture->getSize();
				if (texSize.Width/mTextureDivider >= 16 && texSize.Height/mTextureDivider >= 16)	// don't reduce tiny textures
				{
					// TODO: Check if we can add texture-scaling directly into Irrlicht. Creating 2 images + 1 texture just to downscale is a little bit ugly.
					texture->grab();
					mVideoDriver->removeTexture(texture);
					irr::video::IImage* image = mVideoDriver->createImage (texture, irr::core::position2d<irr::s32>(0,0), texSize);
					irr::video::IImage * imageSmall = mVideoDriver->createImage(texture->getColorFormat(), texSize/mTextureDivider);
					image->copyToScaling(imageSmall);
					irr::video::ITexture* texSmall = mVideoDriver->addTexture(texture->getName(), imageSmall);
					image->drop();
					imageSmall->drop();
					texture->drop();
					texture = texSmall;
				}
			}
		}

		if (mTextureCaching == ADD_TEXTURES)
			mTextureCache.insert(texture);
		else if ( mTextureCaching == REMOVE_TEXTURES)
			mTextureCache.erase(texture);
	}

	return texture;
}

void MeshTextureLoader::setMeshFile(const irr::io::IReadFile* meshFile)
{
	mBaseLoader->setMeshFile(meshFile);
}

void MeshTextureLoader::setMaterialFile(const irr::io::IReadFile* materialFile)
{
	mBaseLoader->setMaterialFile(materialFile);
}
