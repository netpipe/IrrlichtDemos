#ifndef __C_Q3MODELMESHFILELOADER_H_INCLUDED__
#define __C_Q3MODELMESHFILELOADER_H_INCLUDED__

#include <IMeshLoader.h>
#include <IFileSystem.h>
#include <IReadFile.h>
#include <IVideoDriver.h>

namespace irr
{
namespace scene
{

//! Meshloader capable of loading md3 files.
class CQ3ModelMeshFileLoader : public IMeshLoader
{
public:

	//! Constructor
	CQ3ModelMeshFileLoader(io::IFileSystem* fs, video::IVideoDriver* driver);

	//! destructor
	virtual ~CQ3ModelMeshFileLoader();

	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".bsp")
	virtual bool isALoadableFileExtension(const c8* fileName) const;

	//! creates/loads an animated mesh from the file.
	//! \return Pointer to the created mesh. Returns 0 if loading failed.
	//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
	//! See IUnknown::drop() for more information.
	virtual IAnimatedMesh* createMesh(irr::io::IReadFile* file, u32 firstFrame, u32 totalFrames );
	virtual IAnimatedMesh* createMesh(irr::io::IReadFile* file) { return createMesh(file, 0, 0); };

	virtual void showInformation(irr::io::IReadFile* file);

private:

	io::IFileSystem* FileSystem;
	video::IVideoDriver* Driver;

};

} // end namespace scene
} // end namespace irr

#endif // __C_Q3MODELMESHFILELOADER_H_INCLUDED__

