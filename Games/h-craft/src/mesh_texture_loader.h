// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef MESH_TEXTURE_LOADER_H
#define MESH_TEXTURE_LOADER_H

#include <irrlicht.h>
#include <set>

// Provide a wrapper around another IMeshTextureLoader that remembers the meshes loaded for one mesh.
// It allows clearing all textures used in meshes from the cache.
class MeshTextureLoader : public irr::scene::IMeshTextureLoader
{
public:
	enum TextureCaching
	{
		IGNORE,
		ADD_TEXTURES,	// Add new requested textures to internal cache
		REMOVE_TEXTURES	// Remove new requested textures from internal cache
	};

	MeshTextureLoader(irr::scene::IMeshTextureLoader* baseLoader, irr::video::IVideoDriver* driver);

	virtual ~MeshTextureLoader();

	// When enabled all meshes loading through this loader will cache textures
	// By default it's disabled
	void setTextureCaching(TextureCaching tc);

	//! Remove all textures loaded via this loader from the texture-cache.
	void clearTextureCache();

	//! Reduce texture quality after loading to save memory.
	//! divider should be power of two
	void downscaleTexture(irr::u32 divider)	{ mTextureDivider = divider; }

	//! Set a custom texture path.
    /**	This is the first path the texture-loader should search.  */
	virtual void setTexturePath(const irr::io::path& path);

	//! Get the current custom texture path.
	virtual const irr::io::path& getTexturePath() const;

	//! Get the texture by searching for it in all paths that makes sense for the given textureName.
	/** Usually you do not have to use this method, it is used internally by IMeshLoader's.
	\param textureName Texturename as used in the mesh-format
	\return Pointer to the texture. Returns 0 if loading failed.*/
	virtual irr::video::ITexture* getTexture(const irr::io::path& textureName);

	//! Meshloaders will search paths relative to the meshFile.
	/** Usually you do not have to use this method, it is used internally by IMeshLoader's.
		Any values you set here will likely be overwritten internally. */
	virtual void setMeshFile(const irr::io::IReadFile* meshFile);

	//! Meshloaders will try to look relative to the path of the materialFile
	/** Usually you do not have to use this method, it is used internally by IMeshLoader's.
	Any values you set here will likely be overwritten internally.	*/
	virtual void setMaterialFile(const irr::io::IReadFile* materialFile);

private:
	irr::scene::IMeshTextureLoader* mBaseLoader;
	irr::video::IVideoDriver* mVideoDriver;
	std::set<irr::video::ITexture*> mTextureCache;
	TextureCaching mTextureCaching;
	irr::u32 mTextureDivider;
};

#endif // MESH_TEXTURE_LOADER_H
