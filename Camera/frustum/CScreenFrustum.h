
//! CScreenFrustum class
/// kiszuri a kepernyon lathato node-okat a Bounding Box-juk alapjan
class CScreenFrustum
{
    public:

        //! constructor
        CScreenFrustum(scene::ISceneManager* Smgr, scene::ICameraSceneNode* Cam);

        //! isNodeInside
        bool isNodeInside(scene::IMeshSceneNode* Node);

        //! DEBUG_draw
        void DEBUG_draw();

    private:
        //! quickTest
        bool quickTest(scene::IMeshSceneNode* Node);

        //! frustumBoxTest
        bool frustumBoxTest(scene::IMeshSceneNode* Node);

        //! isPointInside
        bool isPointInside(core::vector3df Point);

        //! frustumSphereTest
        bool frustumSphereTest(scene::IMeshSceneNode* Node);

        //! transform
        void transform(const core::matrix4& mat);

        //! rebuild
        void rebuild();

        // helyi instancok
        scene::ISceneManager* smgr;
        scene::ICameraSceneNode* cam;

        // W = ScreenWidth  H = ScreenHeight  n = near  f = far
        //              00n   W0n   0Hn   WHn     00f   W0f   0Hf   WHf
        core::vector3df r000, r100, r010, r110,   r001, r101, r011, r111;

        // screen frustum
        core::plane3df orgPlane[6], plane[6];

        // BB
        core::aabbox3df BB;

}; // end class CScreenFrustum 
