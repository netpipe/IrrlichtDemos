
/*
   The second function loadModel() loads a model and displays it using an
   addAnimatedMeshSceneNode and the scene manager. Nothing difficult. It also
   displays a short message box, if the model could not be loaded.
*/
    core::stringc filename;
void loadModel(const c8* fn)
{
    // modify the name if it a .pk3 file
    core::stringc filename ( fn );
    loading=false;
    core::stringc extension,fileNm;
    core::getFileNameExtension ( extension, filename );
    extension.make_lower();
    core::cutFilenameExtension ( fileNm, filename );
  //  fileNm.make_lower();
    s32 start =  fileNm.findLast('/')+1;
    fileNm=fileNm.subString(start,(fileNm.size()-start));

    stringc workdir=Device->getFileSystem()->getWorkingDirectory(); // get the current working dir

    stringc absolute=Device->getFileSystem()->getAbsolutePath(filename); // Absolute path
    absolute=absolute.subString(workdir.size()+1,absolute.size()); // relative path from the application folder
   // Device->getFileSystem()->changeWorkingDirectoryTo(appPath.c_str());

   //just to test printf("path %s",absolute.c_str());

    if (   extension == ".jpg" ||
            extension == ".pcx" ||
            extension == ".png" ||
            extension == ".ppm" ||
            extension == ".pgm" ||
            extension == ".pbm" ||
            extension == ".psd" ||
            extension == ".tga" ||
            extension == ".bmp"
       )
    {
        video::ITexture * texture = Device->getVideoDriver()->getTexture( absolute.c_str() );
        if ( texture && Model )
        {
            // always reload texture
            Device->getVideoDriver()->removeTexture ( texture );
            texture = Device->getVideoDriver()->getTexture( absolute.c_str() );
            Model->setMaterialTexture ( 0, texture );
            Device->getFileSystem()->changeWorkingDirectoryTo(workdir.c_str()); // Put back the filesystem where it was
        }
        return;
    }
    if (extension == "" && !fileRequest.loader)
    {
        filename+=".irr";    // user didnt typed a extension. I'm assuming its a scene.
        extension=".irr";
    }
    if (   extension == ".irr" && !fileRequest.loader)
    {
        // Remove the Editor elements from the scene
        vector3df cam0pos = viewp.Camera[0]->getPosition();
        vector3df cam0tar = viewp.Camera[0]->getTarget();
        vector3df cam2pos = viewp.Camera[2]->getPosition();
        vector3df cam2tar = viewp.Camera[2]->getTarget();
        vector3df cam3pos = viewp.Camera[3]->getPosition();
        vector3df cam3tar = viewp.Camera[3]->getTarget();
        vector3df cam4pos = viewp.Camera[4]->getPosition();
        vector3df cam4tar = viewp.Camera[4]->getTarget();
        viewp.grid->remove();
        viewp.ground->remove();
        viewp.SkyBox->remove();
        viewp.Camera[0]->remove();
        viewp.Camera[1]->remove();
        viewp.Camera[2]->remove();
        viewp.Camera[3]->remove();
        viewp.Camera[4]->remove();
        viewp.bill->remove();
        light1->remove();
        light2->remove();
        light3->remove();
        if (Model) Model->setDebugDataVisible(scene::EDS_OFF);
        Device->getFileSystem()->changeWorkingDirectoryTo(appPath.c_str()); // Put back the filesystem where it was
        Device->getSceneManager()->saveScene(filename.c_str());
        printf("Completed the scene saving\n");
        fileRequest.loader=true;
        // Re-create the Editor elements in the scene
        viewp.createCameras();
        viewp.Camera[0]->setPosition(cam0pos);
        viewp.Camera[0]->setTarget(cam0tar);
        viewp.Camera[2]->setPosition(cam2pos);
        viewp.Camera[2]->setTarget(cam2tar);
        viewp.Camera[3]->setPosition(cam3pos);
        viewp.Camera[3]->setTarget(cam3tar);
        viewp.Camera[4]->setPosition(cam4pos);
        viewp.Camera[4]->setTarget(cam4tar);
        viewp.createGrid();
        viewp.createGround();
        viewp.createSkyBox();
        viewp.createBillboard();
        createLights();
        if (Model) Model->setDebugDataVisible(scene::EDS_BBOX);

        return;
    }
    if (!fileRequest.loader)
    {
        Device->getFileSystem()->changeWorkingDirectoryTo(appPath.c_str()); // Put back the filesystem where it was
        return;
    } // Filename is not a .IRR
    // if a archive is loaded add it to the FileSystems..
    if (   extension == ".pk3" ||
            extension == ".zip"
       )
    {
        Device->getFileSystem()->addZipFileArchive( filename.c_str () );
        return;
    }

    // Load an IRRscene
    if (   extension == ".irr" && fileRequest.loader)
    {
        Device->getSceneManager()->loadScene(filename.c_str());
        selList->clear();
        lastNode=0;
        selNode=0;
        fakecounter=0;
        if (Model) Model->setDebugDataVisible(scene::EDS_OFF);
        SceneAnalys(Device->getSceneManager()->getRootSceneNode());
        printf("Completed the scene loading\n");
        Device->getFileSystem()->changeWorkingDirectoryTo(appPath.c_str()); // Put back the filesystem where it was
        return;
    }


    // load a model into the engine

    if (Model)
    {
        //Model->remove();
        //moveviews=false;  // hide the move button
        Model->setDebugDataVisible(scene::EDS_OFF);
        Model = 0; //lastNode+=1;
    }
    lastNode+=1;

    scene::IAnimatedMesh* m = Device->getSceneManager()->getMesh( filename.c_str() );
    ModelMs=m;
    if (!m)
    {
        // model could not be loaded
        if (StartUpModelFile != filename)
            Device->getGUIEnvironment()->addMessageBox(
                Caption.c_str(), L"The model could not be loaded. "\
                L"Maybe it is not a supported file format.");
     //           Device->getFileSystem()->changeWorkingDirectoryTo(appPath.c_str());
    //    Device->getFileSystem()->addFolderFileArchive ( "media/" );
        Device->getFileSystem()->changeWorkingDirectoryTo("media/");
        lastNode-=1;
        if (lastNode<0)
        {
            lastNode=0;
            selNode=0;
        }
        Device->getFileSystem()->changeWorkingDirectoryTo(appPath.c_str()); // Put back the filesystem where it was
        return;
    }
    // update the current scene with the new info
    // ModelMesh[lastNode-1]=m; // save the infor for this animated mesh
    selName[lastNode-1]=(fileNm.c_str()); // Put the name in the list
    selNode=lastNode-1; // Set the new selection on this object
    //selName[selNode][lastNode]=(L"Test string");
    scene::ISceneNode* sceneRoot = Device->getSceneManager()->getRootSceneNode();
    // set default material properties
    Model = Device->getSceneManager()->addAnimatedMeshSceneNode(m,sceneRoot);
    Model->setName(fileNm.c_str());
    Model->setMaterialFlag(video::EMF_LIGHTING, false);
    Model->setAnimationSpeed(30);
    Model->setFrameLoop(0,0);
    if (atcamtarget) Model->setPosition(viewp.Camera[0]->getTarget());
    //printf("LAst node, %i",lastNode);
    //createSelList(); printf("LastNode=%i\n",lastNode);
    if (lastNode>1 && Model) refreshAnimWin(); // refresh the anim windows with the new values//ThePoly=0;
    if (lastNode==2 && Model)
    {
        // remove tool box if already there
        IGUIEnvironment* env = Device->getGUIEnvironment();
        IGUIElement* root = env->getRootGUIElement();
        IGUIElement* e = root->getElementFromId(6002, true);
        if (e)
        {
            e->remove();
            createAnimationToolBox();
        }
    }

    // Regenerate the selection list
    selList->clear();
    selNode=0;
    lastNode=0;
    fakecounter=0;
    SceneAnalys(Device->getSceneManager()->getRootSceneNode());
    Device->getFileSystem()->changeWorkingDirectoryTo(workdir.c_str()); // Put back the filesystem where it was
}
