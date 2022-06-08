
//! constructor
CScreenFrustum::CScreenFrustum(scene::ISceneManager* Smgr, scene::ICameraSceneNode* Cam)
{
    // instancok
    smgr = Smgr;
    cam = Cam;

    // plane-ek alkotoelemei
    core::matrix4 mat = cam->getAbsoluteTransformation();
    float nearv = cam->getNearValue();
    float farv = cam->getFarValue();
    float aspr = cam->getAspectRatio();
    float fov = cam->getFOV();

    float tang = tan(fov / 2);
    float nearh = 2 * tang * nearv;
    float nearw = nearh * aspr;
    float farh = 2 * tang * farv;
    float farw = farh * aspr;

    float nearh2 = nearh / 2;
    float nearw2 = nearw / 2;
    float farh2 = farh / 2;
    float farw2 = farw / 2;

    // near coords
    r000 = core::vector3df(-nearw2,  nearh2, nearv);
    r100 = core::vector3df( nearw2,  nearh2, nearv);
    r010 = core::vector3df(-nearw2, -nearh2, nearv);
    r110 = core::vector3df( nearw2, -nearh2, nearv);

    mat.transformVect(r000);
    mat.transformVect(r100);
    mat.transformVect(r010);
    mat.transformVect(r110);

    // far coords
    r001 = core::vector3df(-farw2,  farh2, farv);
    r101 = core::vector3df( farw2,  farh2, farv);
    r011 = core::vector3df(-farw2, -farh2, farv);
    r111 = core::vector3df( farw2, -farh2, farv);
    mat.transformVect(r001);
    mat.transformVect(r101);
    mat.transformVect(r011);
    mat.transformVect(r111);

    // BB
    BB.reset(cam->getAbsolutePosition());
    BB.addInternalPoint(r000);
    BB.addInternalPoint(r100);
    BB.addInternalPoint(r010);
    BB.addInternalPoint(r110);
    BB.addInternalPoint(r001);
    BB.addInternalPoint(r101);
    BB.addInternalPoint(r011);
    BB.addInternalPoint(r111);

    // eredeti plane-ek
    orgPlane[0].setPlane(r100, r000, r010); // near
    orgPlane[1].setPlane(r001, r101, r011); // far

    orgPlane[2].setPlane(r011, r111, r110); // bottom
    orgPlane[3].setPlane(r000, r100, r101); // top

    orgPlane[4].setPlane(r000, r001, r011); // left
    orgPlane[5].setPlane(r101, r100, r110); // right

}

//! quickTest
/// egyszeru bouningbox-tesz a frustum tesztek elott
bool CScreenFrustum::quickTest(scene::IMeshSceneNode* Node)
{
    // trans node BB
    core::aabbox3df nodeTransBB = Node->getTransformedBoundingBox();

    // ha a kamera a nodeTRBB-n belul
    if (nodeTransBB.isPointInside(cam->getAbsolutePosition())) return true;

    // ha nincs a BB-n belul
    if (!BB.intersectsWithBox(nodeTransBB)) return false;

    return true;
}

//! frustumBoxTest
/// frustum vs. bounding box
bool CScreenFrustum::frustumBoxTest(scene::IMeshSceneNode* Node)
{
    // ujraepitjuk a planeket
    rebuild();

    // atvisszuk a frustumot a node absz. transzf. matrixjaba
    core::matrix4 invTrans(Node->getAbsoluteTransformation());
    invTrans.makeInverse();
    transform(invTrans);

    // nodeBB pontjai
    core::aabbox3df nodeBB = Node->getBoundingBox();
    core::vector3df p[8];
    nodeBB.getEdges(&p[0]);

    // ha akar csak egy pont benne van, akkor lathato a node
    for (int i = 0; i < 8; ++i)
        if (isPointInside(p[i])) return true;

    return false;
}

//! isPointInside
/// pont a plane-ek kozott van-e?
bool CScreenFrustum::isPointInside(core::vector3df Point)
{
    for (int i = 0; i < 6; ++i)
        if (plane[i].classifyPointRelation(Point) == core::ISREL3D_BACK) return false;

    return true;
}

//! frustumSphereTest
/// frustum vs. bounding sphere
bool CScreenFrustum::frustumSphereTest(scene::IMeshSceneNode* Node)
{
   // node transzformalt bb
    core::aabbox3df nodeTransBB = Node->getTransformedBoundingBox();

    // plane-ek ujraepitese
    rebuild();

    // sphere test
    core::vector3df center = nodeTransBB.getCenter();
    float radius = (nodeTransBB.MaxEdge - center).getLength();
    float dist;

    for (int i = 0; i < 6; ++i)
    {
        //plane[i].Normal.dotProduct(center) + plane[i].D;
        dist = plane[i].getDistanceTo(center);

        // kint van
        if (dist <= -radius) return false;
    }

    return true;
}

//! isNodeInside
/// a node a kepernyo frustumjaban van-e?
bool CScreenFrustum::isNodeInside(scene::IMeshSceneNode* Node)
{
    bool ret;

    if (ret = quickTest(Node))  // ha a gyorstszten atment
        if (!(ret = frustumBoxTest(Node)))  // es a boxteszten bukott
            if (ret = frustumSphereTest(Node)); // attol a sphereteszten meg remekelhet

    return ret;
}

//! transform
/// matrixxal transzformalja az oldalakat
void CScreenFrustum::transform(const core::matrix4& mat)
{
    for (int i = 0; i < 6; ++i) mat.transformPlane(plane[i]);
}

//! rebuild
/// helyreallitja plane-eket
void CScreenFrustum::rebuild()
{
    // vissza az eredeti allapotba
    for (int i = 0; i < 6; ++i) plane[i].setPlane(orgPlane[i].Normal, orgPlane[i].D);
}


//! DEBUG_draw
void CScreenFrustum::DEBUG_draw()
{
    video::IVideoDriver* driver = smgr->getVideoDriver();
    core::matrix4 world;
    world.makeIdentity();
    driver->setTransform(video::ETS_WORLD, world);

    // near
    driver->draw3DLine(r000, r100, video::SColor(255, 255, 0, 0));
    driver->draw3DLine(r100, r110, video::SColor(255, 255, 0, 0));
    driver->draw3DLine(r110, r010, video::SColor(255, 255, 0, 0));
    driver->draw3DLine(r010, r000, video::SColor(255, 255, 0, 0));

    // edges
    driver->draw3DLine(r000, r001, video::SColor(255, 0, 255, 0));
    driver->draw3DLine(r100, r101, video::SColor(255, 0, 255, 0));
    driver->draw3DLine(r010, r011, video::SColor(255, 0, 255, 0));
    driver->draw3DLine(r110, r111, video::SColor(255, 0, 255, 0));

    // far
    driver->draw3DLine(r001, r101, video::SColor(255, 0, 0, 255));
    driver->draw3DLine(r101, r111, video::SColor(255, 0, 0, 255));
    driver->draw3DLine(r111, r011, video::SColor(255, 0, 0, 255));
    driver->draw3DLine(r011, r001, video::SColor(255, 0, 0, 255));
} 
