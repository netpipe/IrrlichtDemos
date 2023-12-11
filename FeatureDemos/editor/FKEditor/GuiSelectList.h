void addIcon(IGUISpriteBank* sb, c8* fn, int sizex, int sizey)
{
    // load and add the texture to the bank
    Device->getVideoDriver()->makeColorKeyTexture(Device->getVideoDriver()->getTexture(fn), position2di(0,0));

    sb->addTexture(Device->getVideoDriver()->getTexture(fn));
    u32 textureIndex = sb->getTextureCount() - 1;

    // now lets get the sprite bank's rectangles and add some for our animation
    core::array<core::rect<s32> >& rectangles = sb->getPositions();
    u32 firstRect = rectangles.size();
    // remember that rectangles are not in pixels, they enclose pixels!
    // to draw a rectangle around the pixel at 0,0, it would rect<s32>(0,0, 1,1)
    rectangles.push_back(rect<s32>(0,0, sizex+1,sizey+1));


    // now we make a sprite..
    SGUISprite sprite;
    sprite.frameTime = 30;
    // add some frames of animation.
    SGUISpriteFrame frame;
    frame.rectNumber = firstRect;
    frame.textureNumber = textureIndex;

    // add this frame
    sprite.Frames.push_back(frame);
    // add the sprite
    u32 spriteIndex = sb->getSprites().size();
    sb->getSprites().push_back(sprite);
}

void createIconBank()
{
    IGUISpriteBank* sprites = Device->getGUIEnvironment()->addEmptySpriteBank("icons");
    //IGUISpriteBank* sprites = Device->getGUIEnvironment()->getSkin()->getSpriteBank();
    addIcon(sprites, "media/ui/am_solid.png",35,19);
    addIcon(sprites, "media/ui/am_hide.png",35,19);
    addIcon(sprites, "media/ui/am_wire.png",35,19);
    addIcon(sprites, "media/ui/ot_solid.png",35,19);
    addIcon(sprites, "media/ui/am_hide.png",35,19);
    addIcon(sprites, "media/ui/am_wire.png",35,19);
    addIcon(sprites, "media/ui/ms_solid.png",35,19);
    addIcon(sprites, "media/ui/ms_hide.png",35,19);
    addIcon(sprites, "media/ui/ms_wire.png",35,19);

    if (sprites) selList->setSpriteBank(sprites);


    // now let's use this as the X window button
    //Device->getGUIEnvironment()->getSkin()->setIcon(EGDI_WINDOW_CLOSE, spriteIndex);


}


IAnimatedMesh * retrieveMesh2(ISceneNode * node)
{
    io::IAttributes* attribs = Device->getFileSystem()->createEmptyAttributes();
    scene::ISceneManager* smgr = Device->getSceneManager();
    if (attribs)
    {
        // get the mesh name out
        node->serializeAttributes(attribs);
        core::stringc name = attribs->getAttributeAsString("Mesh");
        attribs->drop();
        // get the animated mesh for the object
        if (node->getType() ==   ESNT_ANIMATED_MESH )
        {
            scene::IAnimatedMesh* mesh = smgr->getMesh(name.c_str());
            return mesh;
        }
        if (node->getType() ==   ESNT_OCTREE )
        {
            scene::IAnimatedMesh* mesh = smgr->getMesh(name.c_str());
            return mesh;
        }
    }
    return 0;
}

void displayNode(ISceneNode * node)
{
    if (node)
    {
        core::stringw str="Model: ";
        str.append((stringw) node->getName());
        coords->setText(str.c_str());
    }
}

void displayMove(ISceneNode * node)
{
    if (node)
    {
        core::stringw str="Model: ";
        str.append((stringw) node->getName());
        str+="\nMove:\n  X: ";
        str.append((core::stringw)(float)(node->getPosition().X));
        str+="\n  Y: ";
        str.append((core::stringw)(float) node->getPosition().Y);
        str+="\n  Z: ";
        str.append((core::stringw)(float)(node->getPosition().Z));
        coords->setText(str.c_str());
    }
}

void displayRotate(ISceneNode * node)
{
    if (node)
    {
        core::stringw str="Model: ";
        str.append((stringw) node->getName());
        str+="\nRotate:\n  X: ";
        str.append((core::stringw)(float)(node->getRotation().X));
        str+="\n  Y: ";
        str.append((core::stringw)(float) node->getRotation().Y);
        str+="\n  Z: ";
        str.append((core::stringw)(float)(node->getRotation().Z));
        coords->setText(str.c_str());
    }
}

void displayScale(ISceneNode * node)
{
    if (node)
    {
        core::stringw str="Model: ";
        str.append((stringw) node->getName());
        str+="\nScale:\n  X: ";
        str.append((core::stringw)(float)(node->getScale().X));
        str+="\n  Y: ";
        str.append((core::stringw)(float) node->getScale().Y);
        str+="\n  Z: ";
        str.append((core::stringw)(float)(node->getScale().Z));
        coords->setText(str.c_str());
    }
}

void getListNode()
{
    int value = selList->getSelected();
    //value-=1;
    //core::stringc name = selName[value+1].c_str();
    {
        core::stringc name = selList->getListItem((u32)value);
        // printf("Selected node %i \n",value);
        selNode=value;
        if (value<1 && Model) Model->setDebugDataVisible(scene::EDS_OFF);
        if (value==0)
        {
            Model=0;
            ModelMs=0;
        }
        if (value>0)
        {
            ISceneNode * T = Device->getSceneManager()->getSceneNodeFromName(name.c_str());
            if (T)
            {
                //
                if (Model)
                {
                    Model->setDebugDataVisible(scene::EDS_OFF);
                }
                if (T->getType() == ESNT_ANIMATED_MESH)
                {
                    Model = (IAnimatedMeshSceneNode *)T;
                    ModelMs = retrieveMesh2(Model);
                    animated=true;
                } // Retrieve the mesh from an Animated Mesh type
                if (T->getType() == ESNT_OCTREE || T->getType() == ESNT_MESH)
                {
                    Model=(IAnimatedMeshSceneNode *)T;
                    ModelMs = retrieveMesh2(Model);
                    animated = false;
                }

                if (T->getType() == ESNT_SPHERE ||
										T->getType() == ESNT_CUBE)
                {
                	Model = (IAnimatedMeshSceneNode *)T;
                	ModelMs = retrieveMesh2(Model);
                	animated = false;
								}

								if (ModelMs)
										core::aabbox3d<f32> ModelBBox = ModelMs->getMesh(0)->getBoundingBox();

								if (Model)
								{
									Model->setDebugDataVisible(scene::EDS_BBOX);
									displayNode(Model);
									viewp.Model = Model;
									refreshanimwin=true;
								}

            }
            else printf("error, this model was not found in the db!!!\n"); // refresh the anim windows with the new values
        }
    }
}

bool createSelList()
{
    IGUIEnvironment * env = Device->getGUIEnvironment();
    IGUIElement* root = env->getRootGUIElement();
    IGUIElement* e = root->getElementFromId(5000, true);
    bool a=true;
    if (a)
    {
        selList->clear();
        //selList->addItem(L"Unselect item");
        for (int begin=0;begin < lastNode; begin++)
        {
            selList->addItem(selName[begin].c_str());
            selList->setItemOverrideColor(begin,EGUI_LBC_TEXT ,SColor(255,64,64,128));
            selList->setSelected(begin);
        }
        return true;
    }
    return false;
}
// Used when you remove a selected item from the list
bool removeItemfromList(s32 item, s32 lastitem)
{
    for (s32 i=item; i < (lastitem+1) ; i++)
    {
        selName[i]=selName[i+1];
    }
    lastNode-=1;
    return true;
}
