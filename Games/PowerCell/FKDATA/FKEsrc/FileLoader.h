class files
{
public :

    static IrrlichtDevice *device;
    static gui::IGUIListBox * filelist;
    static gui::IGUIComboBox * filetype;
    static io::IFileList * filelist1;
    static gui::IGUIStaticText * FileNameText;
    static gui::IGUIEditBox * name;
    static gui::IGUISpriteBank* sprites;
    static gui::IGUIButton * ok;
    static gui::IGUIButton * cancel;
    static gui::IGUIButton * close;
    static stringc exts[1024];
    static bool loader;

private:
    IGUIWindow* wnd;

public:
    bool image,irr,meshes,compress,initfolder, customfolder;
    int items, single, startitem;

////////////////////////////////////////////////////////////////////////////////
// This add a single image to the sprite bank
// Code based on Bitplane's example
////////////////////////////////////////////////////////////////////////////////
    void addIcon(IGUISpriteBank* sb, c8* fn, int sizex, int sizey)
    {
        // load and add the texture to the bank
        device->getVideoDriver()->makeColorKeyTexture(Device->getVideoDriver()->getTexture(fn), position2di(0,0));

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
////////////////////////////////////////////////////////////////////////////////
// Function to replace the default folder image
//
////////////////////////////////////////////////////////////////////////////////
    void changeFolderIcon(irr::c8 * fn)
    {
        if (initfolder)
        {
            device->getVideoDriver()->makeColorKeyTexture(Device->getVideoDriver()->getTexture(fn), position2di(0,0));
            sprites->setTexture (0, Device->getVideoDriver()->getTexture(fn));
        }
        else
        {
            addIcon(sprites, fn,16,16);
            customfolder=true;
        }
    }
////////////////////////////////////////////////////////////////////////////////
// Create a filetype. Can be used to create additional filetypes
// ---hide flag in the description will hide the description in the combo box
////////////////////////////////////////////////////////////////////////////////
    void addFileType(stringw description, stringc ext, irr::c8 * fn)
    {
        if ((stringc)description.c_str()!="---hide") filetype->addItem(description.c_str());
        exts[items++]=ext.c_str();
        addIcon(sprites, fn,16,16);
    }

////////////////////////////////////////////////////////////////////////////////
// Create Filetypes for IRRlicht supported images formats
//
////////////////////////////////////////////////////////////////////////////////
    void addImageCombo()
    {
        addFileType(L"    Bitmap image (*.bmp)",".bmp","./media/filetypes/jpgtype.png");
        addFileType(L"    Jpeg images (*.jpg)",".jpg","./media/filetypes/jpgtype.png");
        addFileType(L"    PCX images (*.pcx)",".pcx","./media/filetypes/pngtype.png");
        addFileType(L"    PNG images (*.png)",".png","./media/filetypes/pngtype.png");
        addFileType(L"    PPM images (*.ppm)",".ppm","./media/filetypes/pngtype.png");
        addFileType(L"    PGM images (*.pgm)",".pgm","./media/filetypes/pngtype.png");
        addFileType(L"    Photoshop image (*.psd)",".psd","./media/filetypes/psdtype.png");
        addFileType(L"    Targa images (*.tga)",".tga","./media/filetypes/pngtype.png");

    }
////////////////////////////////////////////////////////////////////////////////
// Create Filestypes for IRRlicht supported meshes formats
//
////////////////////////////////////////////////////////////////////////////////
    void addMeshesCombo()
    {
        addFileType(L"    BSP (*.bsp)",".bsp","./media/filetypes/3dstype.png"); //
        addFileType(L"    3D Studio (*.3ds)",".3ds","./media/filetypes/3dstype.png");
        addFileType(L"    Blitz 3D (*.b3d)",".b3d","./media/filetypes/b3dtype.png");
        addFileType(L"    Collada (*.dae)",".dae","./media/filetypes/b3dtype.png");
        addFileType(L"    Collada (*.xml)",".xml","./media/filetypes/b3dtype.png");
        addFileType(L"    Cartography shop 4 (*.csm)",".csm","./media/filetypes/b3dtype.png");
        addFileType(L"    DeleD (*.dmf)",".dmf","./media/filetypes/b3dtype.png");
        addFileType(L"    DirectX (*.x)",".x","./media/filetypes/xtype.png");
        addFileType(L"    Lightwave (*.lwo)",".lwo","./media/filetypes/lwotype.png");
        addFileType(L"    Maya (*.obj)",".obj","./media/filetypes/objtype.png");
        addFileType(L"    Milkshape 3D (*.ms3d)",".ms3d","./media/filetypes/b3dtype.png");
        addFileType(L"    My3D (*.my3d)",".my3d","./media/filetypes/my3dtype.png");
        addFileType(L"    OCT (*.oct)",".oct","./media/filetypes/b3dtype.png");
        addFileType(L"    Pulsar LM Tools (*.lmts)",".lmts","./media/filetypes/b3dtype.png");
        addFileType(L"    Quake II (*.md2)",".md2","./media/filetypes/md2type.png");
        addFileType(L"    Quake III (*.md3)",".md3","./media/filetypes/md3type.png");
                addFileType(L"    IRRMESH (*.irrmesh)",".irrmesh","./media/filetypes/md3type.png");
    }
////////////////////////////////////////////////////////////////////////////////
// Create the filelist using the defined filestypes
//
////////////////////////////////////////////////////////////////////////////////
    void addfilelist(gui::IGUIListBox * list)
    {
        IGUISkin *skin = device->getGUIEnvironment()->getSkin();

        filelist1=device->getFileSystem()->createFileList();
        list->clear();
        for (int a=0; a!=filelist1->getFileCount(); a++)
        {
            stringw value = filelist1->getFileName(a);
            stringc extension="";
            stringc source=value.c_str();
            core::getFileNameExtension ( extension, source);
            extension.make_lower();
            if (single==0)
            {
                for (int b=startitem; b!=items; b++)
                {
                    if (extension==exts[b]) list->addItem(value.c_str(),b);
                }
            }
            if (single>0)
            {
                if (extension==exts[single]) list->addItem(value.c_str(),single);
            }
            if (filelist1->isDirectory(a))
            {
                list->addItem(value.c_str(), 0);
            }
        }
        stringw s = device->getFileSystem()->getWorkingDirectory();
        s+="\\ ";
        FileNameText->setText(s.c_str());
    }
////////////////////////////////////////////////////////////////////////////////
// Draw the file selector
//
////////////////////////////////////////////////////////////////////////////////
    void draw(stringw title, bool img, bool level, bool mesh, bool archive)
    {
        IGUIEnvironment* env = device->getGUIEnvironment();
        IGUIElement* root = env->getRootGUIElement();
        IGUIElement* e = root->getElementFromId(7000, true);
        video::IVideoDriver* driver = Device->getVideoDriver();
        f32 screenx = driver->getScreenSize().Width;
        f32 screeny = driver->getScreenSize().Height;
        FileNameText=0;
        items=0;
        image=img;
        irr=level;
        meshes=mesh;
        compress=archive;
        if (e)   // Need to clear and reload all
        {
            e->remove();
            printf("The file selector is already present!!!");
        }

        wnd = env->addWindow(core::rect<s32>((s32)(screenx/2)-195,(s32)(screeny/2)-180,(s32)(screenx/2)+195,(s32)(screeny/2+180)),
                             true, title.c_str(), 0, 7000);
        close = wnd->getCloseButton();
        close->setID(1202);
        stringw s = device->getFileSystem()->getWorkingDirectory();
        s+="\\ ";
        stringc s1 = device->getFileSystem()->getWorkingDirectory();
        Device->getFileSystem()->changeWorkingDirectoryTo(appPath.c_str());
        FileNameText=env->addStaticText(s.c_str(),core::rect<s32>(10,35,380,55),true,false,wnd,true);
        FileNameText->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER);
        filelist=env->addListBox(core::rect<s32>(10,60,380,300),wnd,890,true);
        filelist->clear();
        filelist1=0;
        if (sprites) filelist->setSpriteBank(sprites);
        items=sprites->getTextureCount();
        startitem=items;
        if (!customfolder) addIcon(sprites, "./media/filetypes/folder.png",16,16);
        initfolder=true; // add the default folder icon
        filetype=env->addComboBox(core::rect<s32>(10,305,300,325),wnd,900);
        filetype->addItem(L"All supported files");
        exts[items++]="---All";
        if (image) addImageCombo();
        if (meshes) addMeshesCombo();
        if (irr)
        {
            addFileType(L"    IRR Scenes (*.irr)",".irr","./media/filetypes/irrtype.png");
        }
        if (compress)
        {
            addFileType(L"---hide",".zip","./media/filetypes/compressedtype.png");
            addFileType(L"---hide",".pk3","./media/filetypes/compressedtype.png");
        }
        name=env->addEditBox(L"",core::rect<s32>(10,330,300,350),true,wnd,901);
        if (loader) ok =  env->addButton(core::rect< s32 >(310,305,380,325),wnd,1200,L"Load",L"Load selected item");
        else ok =  env->addButton(core::rect< s32 >(310,305,380,325),wnd,1200,L"Save",L"Save the item");
        cancel =  env->addButton(core::rect< s32 >(310,330,380,350),wnd,1201,L"Cancel",L"Return to editor");
        Device->getFileSystem()->changeWorkingDirectoryTo(s1.c_str());
        addfilelist(filelist);

    }
////////////////////////////////////////////////////////////////////////////////
// Single click event in the filelist
// Get the name and update the name in the filename text input
////////////////////////////////////////////////////////////////////////////////
    void singleClick()
    {
        const s32 selected = filelist->getSelected();
        if (!filelist1->isDirectory(selected))
            name->setText(filelist->getListItem(selected));
    }
////////////////////////////////////////////////////////////////////////////////
// Double click event in the filelist
// navigate thru the folders, Compressed folders not yet complete.
// Return true if a file as been doubleclicked, and false if it's a folder
////////////////////////////////////////////////////////////////////////////////
    bool doubleClick()
    {
        const s32 selected = filelist->getSelected();
        bool file = false;
        stringc name = filelist1->getFileName(selected);
        stringc ext = "";
        core::getFileNameExtension ( ext, name);
        ext.make_lower();
        file=true;
        if (filelist1)
        {
            if (filelist1->isDirectory(selected))
            {
                device->getFileSystem()->changeWorkingDirectoryTo(name.c_str());
                addfilelist(filelist);
                file = false;
            }
            else
                if (ext==".zip")
                {
                    printf("Zip file"); // Not functionnal
                    Device->getFileSystem()->addZipFileArchive(name.c_str(),false,false);
                    Device->getFileSystem()->changeWorkingDirectoryTo(name.c_str());
                    addfilelist(filelist);
                }
        }
        return file;
    }
////////////////////////////////////////////////////////////////////////////////
// Combo Action
//
////////////////////////////////////////////////////////////////////////////////
    void comboAction(s32 pos)
    {
        if (exts[pos]==".irr")
        {
            single=pos;
            image=false;
            irr=true;
            meshes=false;
            compress=false;
        }
        else
            if (exts[pos]=="---All")
            {
                single=0;
                image=true;
                irr=true;
                meshes=true;
                compress=true;
            }
            else
                if (exts[pos]=="---Compress")
                {
                    single=0;
                    image=false;
                    irr=false;
                    meshes=false;
                    compress=true;
                }
                else
                    single=pos;
        addfilelist(filelist);
    }
////////////////////////////////////////////////////////////////////////////////
// Close Loader
// Remove the file selector from the display
////////////////////////////////////////////////////////////////////////////////
    void closeLoader()
    {
        IGUIEnvironment* env = device->getGUIEnvironment();
        IGUIElement* root = env->getRootGUIElement();
        IGUIElement* e = root->getElementFromId(7000, true);
        //filelist->clear();
        if (e) e->remove();
    }
////////////////////////////////////////////////////////////////////////////////
// Get Filename
// Return the selected filename from the list and close the fileselector
// Would need to get it from the text field for save
////////////////////////////////////////////////////////////////////////////////
    stringc getFilename()
    {
        IGUIEnvironment* env = device->getGUIEnvironment();
        IGUIElement* root = env->getRootGUIElement();
        IGUIElement* e = root->getElementFromId(7000, true);
        const s32 selected = filelist->getSelected();
        //stringc name = filelist1->getFileName(selected);
        stringc name = filelist->getListItem(selected);
        name = root->getElementFromId(901, true)->getText();
        printf ("Here is the filename selected %s\n",name.c_str());
        if (e) e->remove();
        return name.c_str();
    }
////////////////////////////////////////////////////////////////////////////////
// Initialize the Filetypes icon bank, define witch default filestypes will be
// used.
////////////////////////////////////////////////////////////////////////////////
    void init (IrrlichtDevice *dev)
    {
        device=dev;
        initfolder=false;
        sprites = device->getGUIEnvironment()->addEmptySpriteBank("fileicons");
        items=0;
        single=0;
    }

};
IrrlichtDevice * files::device = 0;
gui::IGUIListBox * files::filelist = 0;
gui::IGUIComboBox * files::filetype = 0;
io::IFileList * files::filelist1 = 0;
gui::IGUIStaticText * files::FileNameText=0;
gui::IGUIEditBox * files::name;
gui::IGUISpriteBank* files::sprites=0;
gui::IGUIButton * files::ok;
gui::IGUIButton * files::cancel;
gui::IGUIButton * files::close;
stringc files::exts[1024]={"",""};
bool files::loader=true;

