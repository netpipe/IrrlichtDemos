http://irrlicht.sourceforge.net/forum/viewtopic.php?f=1&t=52395

void test(IAnimatedMeshSceneNode* gridMesh)
{
    // Grab the tileset image and convert to an IImage
    ITexture* tilesetTex = Game::driver->getTexture("Tilesets/tileset2.png");
    IImage* tilesetImg = Game::driver->createImageFromData (
        tilesetTex->getColorFormat(),
        tilesetTex->getSize(),
        tilesetTex->lock(),
        false
    );
 
    // Create a blank grid image
    const dimension2d<unsigned int> d(640, 480); // Width and Height of a grid
    IImage* gridImg = Game::driver->createImage (
        tilesetTex->getColorFormat(),
        d
    );
 
    // Loop through the amount of tiles needed to fill the new grid image
    for (int x = 0; x < 640/16; x++)
    {
        for (int y = 0; y < 480/16; y++)
        {
            // Copy a single tile to the new grid image
            tilesetImg->copyTo(gridImg, vector2d<int>(x*16,y*16), rect<s32>(0,0,16,16));
        }
    }
    tilesetImg->drop();
 
    // Now I could grab this texture without re-creating it
    video::ITexture* newTexture = Game::driver->addTexture("newGrid.png",gridImg);
 
    gridMesh->setMaterialTexture(0, newTexture);
}
