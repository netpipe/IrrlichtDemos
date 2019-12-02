#ifndef _irrAn8Loader_
#define _irrAn8Loader_

#include <irrlicht.h>
#include <IMeshLoader.h>

class An8File;
class AN8XMATRIX;

namespace irr
{
namespace scene
{

class irrAn8Loader : public IMeshLoader
{
public:
	irrAn8Loader(scene::ISceneManager* smgr, io::IFileSystem* fs);
	~irrAn8Loader();

	virtual bool isALoadableFileExtension(const c8* fileName) const;
	virtual IAnimatedMesh* createMesh(io::IReadFile* file);

private:

	void AddMesh(SMesh* am,An8File* aniFile,int meshID,AN8XMATRIX* matrix);
virtual bool isALoadableFileExtension(const io::path& filename) const {};

	scene::ISceneManager* SceneManager;
	io::IFileSystem* FileSystem;

};

} // end namespace scene
} // end namespace irr


#endif // _irrAn8Loader_


