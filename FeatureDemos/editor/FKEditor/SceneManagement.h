//
//  This retrieve the IAnimatedMesh pointer based on the node
//
IAnimatedMesh * retrieveMesh(ISceneNode * node)
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
        else return 0;
    }
    return 0;
}

s32 setIcons(s32 action)
{
    video::ITexture * tex = Device->getVideoDriver()->getTexture("media/ui/am_solid.png");
    if (action==1)
    {
        icons->addTexture(tex);
        return icons->getTextureCount();
    }
    if (action==0) return icons->getTextureCount();
}

stringc checkDuplicate(stringc name, int range)
{
    int incounter=0;
    stringc add = "";
    for (s32 i=0; i < (range) ; i++)
    {
        add = selName[i].c_str();
        //selList->getListItem(i);
        if (((stringc)add.c_str()) == ((stringc)name.c_str()))
        {
            incounter++;
            add+="(";
            add.append((stringc)incounter);
            add+=")";
            printf("Found a duplicate name\n");
        }
    }
    if (incounter>0)
    {
        return add.c_str();
    }
    return name;
}

stringc stripName(stringc filename)
{


    core::stringc extension,fileNm;
    core::getFileNameExtension ( extension, filename );
    extension.make_lower();
    core::cutFilenameExtension ( fileNm, filename );
//    fileNm.make_lower();
    s32 start =  fileNm.findLast('\\')+1; // Ok for windows, mac or other should check this
    fileNm=fileNm.subString(start,(fileNm.size()-start));
    //printf("String defore: %s\n",fileNm.c_str());
    selName[lastNode]=fileNm.c_str();
    fileNm=checkDuplicate(fileNm.c_str(), lastNode);
    /*fileNm+="(";
    core::stringc p=lastNode;
    fileNm.append(p);
    fileNm+=")";*/
    return fileNm;
}

void SceneAnalys(scene::ISceneNode* node )
{
    //
    // NODE TREE BASIC META TRIANGLE COLLECTOR / First position of the camera
    // This will start from the root node to the last node in the tree, to check for some specified node and setup
    // the metatriangleselector for defining the collision and setup the FPS position based on the last std camera
    // defined in the scene.
    // Code based on Vitek (Travis) informations
    // To do: Adding also land to the meta collision
    // Basic Metaselector application for Basic Collision
    // Put all the occtree in the  Meta Selector
    // Set the FPS camera if a camera is defined there. (Still need cleanup of the old Camera)
    //printf ("Node name is: %s \n",node->getName());
    //printf ("Node id is: %d \n",node->getID());
    //printf ("Node type:");
    //node=smgr->getRootSceneNode();
    // now recurse on children...
    if (fakecounter==0)
    {
        loading=false;
        selList->addItem(L" --- NO SELECTION ---");
        selName[0]=" --- NO SELECTION ---";
        fakecounter++;
        lastNode++;
    }
    io::IAttributes* attribs = Device->getFileSystem()->createEmptyAttributes();
    if (node->getType() == ESNT_SPHERE ||
				node->getType() == ESNT_CUBE)
    {
        selName[fakecounter] = node->getName();

        selList->addItem(selName[fakecounter].c_str());
        selList->setSelected(fakecounter);
        selList->setItemOverrideColor(fakecounter,EGUI_LBC_TEXT ,SColor(255,64,64,64));

        if (!node->isVisible())
            selList->setItemOverrideColor(fakecounter,EGUI_LBC_TEXT ,SColor(255,180,64,64));

        fakecounter++;
        lastNode = fakecounter;
        selNode = fakecounter;

				node->setMaterialFlag(EMF_LIGHTING, true);
    }
    if (node->getType() ==   ESNT_MESH)
    {
        printf("This is a mesh model\n");
        selName[fakecounter]=node->getName();
        selList->addItem(selName[fakecounter].c_str());
        selList->setSelected(fakecounter);
        selList->setItemOverrideColor(fakecounter,EGUI_LBC_TEXT ,SColor(255,64,64,64));
        if (!node->isVisible()) selList->setItemOverrideColor(fakecounter,EGUI_LBC_TEXT ,SColor(255,180,64,64));
        fakecounter++;
        lastNode=fakecounter;
        selNode=fakecounter;
    }
    if (node->getType() ==   ESNT_ANIMATED_MESH )
    {
        printf("This is a Animated mesh model\n");

        stringc txt="";
        if (attribs) // Get the filename from the attributes
        {
            node->serializeAttributes(attribs);
            txt = attribs->getAttributeAsString("Mesh");
            attribs->drop();
        }
        else txt="Animesh" ;
        txt=stripName(txt);
        //selName[fakecounter]=txt.c_str();
        stringw sel=txt.c_str();

        selList->addItem(sel.c_str(),0);

        //selName[fakecounter] = node->getName();
        //selList->addItem(selName[fakecounter].c_str(), 0);
        selList->setSelected(fakecounter);
        node->setName((c8*)txt.c_str());
        //printf("Name set as: %s \n",node->getName());
        selList->setItemOverrideColor(fakecounter,EGUI_LBC_TEXT ,SColor(255,64,64,64));
        if (!node->isVisible())
        {
            selList->setItemOverrideColor(fakecounter,EGUI_LBC_TEXT ,SColor(255,180,64,64));
            selList->setItem(selNode,selList->getListItem(selNode),2);
        }
        fakecounter++;
        lastNode=fakecounter;
        selNode=fakecounter;
        // Set the node for basic of animation
        IAnimatedMeshSceneNode * ani = (IAnimatedMeshSceneNode *)node;
        ani->setMaterialFlag(video::EMF_LIGHTING, false);
        ani->setAnimationSpeed(30);
        ani->setFrameLoop(0,0);
    }
    if (node->getType() ==   ESNT_OCTREE)
    {
        //node->setMaterialFlag(EMF_WIREFRAME, true);
        //node->setMaterialFlag(EMF_FOG_ENABLE,false); // Disable fog setting on occtrees
        // Occ Trees are for objects that will use collision, put the triangle selector in a metaSelector
        stringc txt="";
        if (attribs) // Get the filename from the attributes
        {
            node->serializeAttributes(attribs);
            txt = attribs->getAttributeAsString("Mesh");
            attribs->drop();
            scene::IAnimatedMesh* mesh = Device->getSceneManager()->getMesh(txt.c_str());
            ModelMs=mesh;
        }
        else txt="OctTree" ;

        txt=stripName(txt);
        selName[fakecounter]=txt.c_str();
        selList->addItem(selName[fakecounter].c_str(),3);
        selList->setSelected(fakecounter);
        node->setName((c8*)txt.c_str());
        //printf("Name set as: %s \n",node->getName());
        selList->setItemOverrideColor(fakecounter,EGUI_LBC_TEXT ,SColor(255,64,64,64));
        if (!node->isVisible())
        {
            selList->setItemOverrideColor(fakecounter,EGUI_LBC_TEXT ,SColor(255,180,64,64));
            selList->setItem(selNode,selList->getListItem(selNode),4);
        }
        fakecounter++;
        lastNode=fakecounter;
        selNode=fakecounter;
    }

    core::list<scene::ISceneNode*>::ConstIterator begin = node->getChildren().begin();
    core::list<scene::ISceneNode*>::ConstIterator end   = node->getChildren().end();
    for (; begin != end; ++begin)
        SceneAnalys(*begin);
}
