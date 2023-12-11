#include <irrlicht.h>
using namespace irr;
/** Sky of the world.
* A skydome is a large sphere with a panoramic texture on the inside and is drawn around the camera position.
* It is used a blue-sky texture.
*/
class Sky{
    private:
        /** Scene node of the sky. */
        scene::ISceneNode* sky;
    public:
        /** Default constructor that will build the sky.
        * @param smgr ISceneManager irrlicht ISceneManager.
        * @param driver IVideoDriver irrlicht IVideoDriver.
        * @param texture stringw Texture path.
        * @param radius f32 radius of the skydome.
        */
        Sky(scene::ISceneManager* smgr, video::IVideoDriver* driver, core::stringw texture, f32 radius);
};
