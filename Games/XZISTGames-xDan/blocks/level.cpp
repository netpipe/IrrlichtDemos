
#include "level.h"

void Level::Refresh(f32 delta_t)
{
    
    if (info->levelEditing)
    {
        if (playerEvent.keyCode[KEY_JUMP])
        {
            selectedBlock = NULL;
        }
        selectedBlocks.clear();
        selectedDepths.clear();
    }
    
    // physics stepping
    dSpaceCollide(dSpace, this, dNearCallback);
    //dWorldStep(dWorld, delta_t);
    dWorldQuickStep(dWorld, delta_t);
    dJointGroupEmpty(dJointGroup);
    
    if (info->levelEditing)
    {
        // find and select the closest block colliding with selection ray
        dReal lastDepth = 10000000000000.0;
        for (i = 0; i < selectedBlocks.size(); i ++)
        {
            if (selectedDepths[i] < lastDepth)
            {
                lastDepth = selectedDepths[i];
                selectedBlock = selectedBlocks[i];
            }
        }
    }
    
    for (i = 0; i < blocks.size(); i ++)
    {
        blocks[i]->Refresh();
    }
    
    vector3df cameraAngle = (camera->getTarget() - camera->getPosition()).getHorizontalAngle();
    
    f32 aRad = cameraAngle.Y*0.017453;
    
    // velocity of magnet/block moving in level editor
    const f32 tVel = 5;
    
    if (playerEvent.magnetMode)
    {
        if (playerEvent.keyCode[KEY_MAGRELEASE]) mag->release();
        
        // maybe don't set camera pos as it could be annoying...!
        //camera->setTarget(mag->getEndPos());
        
        if (playerEvent.keyCode[KEY_UP]) mag->translate(vector3df(0,tVel,0));
        if (playerEvent.keyCode[KEY_DOWN]) mag->translate(vector3df(0,-tVel,0));
        
        // strafe magnet relative to player angle
        if (playerEvent.keyCode[KEY_FORWARDS]) mag->translate(vector3df(tVel*sin(aRad),0,tVel*cos(aRad)));
        if (playerEvent.keyCode[KEY_BACKWARDS]) mag->translate(vector3df(-tVel*sin(aRad),0,-tVel*cos(aRad)));
        if (playerEvent.keyCode[KEY_LEFT]) mag->translate(vector3df(-tVel*cos(aRad),0,tVel*sin(aRad)));
        if (playerEvent.keyCode[KEY_RIGHT]) mag->translate(vector3df(tVel*cos(aRad),0,-tVel*sin(aRad)));
    }
    else
    {
        
        // if levelEditing AND a block is selected, block will be moved instead of player.
        // block moving uses same trig as magnet to move relative to camera.
        if (info->levelEditing && selectedBlock)
        {
            vector3df selectedPos = vector3df(0,0,0);
            vector3df selectedRot = vector3df(0,0,0);
            
            if (!playerEvent.mouseLeft)
            {
                if (playerEvent.mouseRight)
                {
                    // rotate if right mouse held
                    if (playerEvent.keyCode[KEY_FORWARDS]) selectedRot.X ++;
                    if (playerEvent.keyCode[KEY_BACKWARDS]) selectedRot.X --;
                    if (playerEvent.keyCode[KEY_LEFT]) selectedRot.Y ++;
                    if (playerEvent.keyCode[KEY_RIGHT]) selectedRot.Y --;
                    if (playerEvent.keyCode[KEY_UP]) selectedRot.Z ++;
                    if (playerEvent.keyCode[KEY_DOWN]) selectedRot.Z --;
                }
                else
                {
                if (playerEvent.keyCode[KEY_UP]) selectedPos += vector3df(0,tVel,0);
                if (playerEvent.keyCode[KEY_DOWN]) selectedPos += vector3df(0,-tVel,0);
        
                // strafe magnet relative to player angle
                if (playerEvent.keyCode[KEY_FORWARDS]) selectedPos += vector3df(tVel*sin(aRad),0,tVel*cos(aRad));
                if (playerEvent.keyCode[KEY_BACKWARDS]) selectedPos += vector3df(-tVel*sin(aRad),0,-tVel*cos(aRad));
                if (playerEvent.keyCode[KEY_LEFT]) selectedPos += vector3df(-tVel*cos(aRad),0,tVel*sin(aRad));
                if (playerEvent.keyCode[KEY_RIGHT]) selectedPos += vector3df(tVel*cos(aRad),0,-tVel*sin(aRad));
                }
            }
            
            //selectedBlock->setPosition(selectedPos);
            dBodyID b = selectedBlock->getBody();
            
            selectedPos *= 100;
            dBodySetLinearVel(b, selectedPos.X,selectedPos.Y,selectedPos.Z);
            
            dBodySetAngularVel(b, selectedRot.X,selectedRot.Y,selectedRot.Z);
        }
        
        // yay for IF statements.
        if (!selectedBlock || (selectedBlock && playerEvent.mouseLeft))
        {
            // ENABLE THIS FOR CORRECT FALLING!
            //if (info->levelEditing)
            {
                if (playerEvent.keyCode[KEY_UP]) player->goUp();
                if (playerEvent.keyCode[KEY_DOWN]) player->goDown();
            }
            
            // not in magnet mode so character is moved
            if (playerEvent.keyCode[KEY_FORWARDS]) player->goForward();
            if (playerEvent.keyCode[KEY_BACKWARDS]) player->goBackward();
            if (playerEvent.keyCode[KEY_LEFT]) player->goLeft();
            if (playerEvent.keyCode[KEY_RIGHT]) player->goRight();
            // ^ those four lines were previously below, in onGround() "if"
            //  but it's annoying not being able to steer in the air... must tweak this!?
        }
            
        if (player->onGround())
        {
            if (playerEvent.keyCode[KEY_JUMP])
            {
                if (!jumped) player->jump();
                jumped = true;
            }
            else
            {
                jumped = false;
            }
        }
    }
    
    mag->Refresh();
    
    player->setAngle(cameraAngle);
    player->Refresh();
    
    if (!info->levelEditing)
    { 
        vector3df pos = player->getPosition();
        pos.Y -= 10;
        camera->setPosition(pos);
        
        // so editing can be disabled when ingame. Possibly remove.
        info->cursor->setVisible(false);
        dBodySetGravityMode(player->getBody(), true);
    }
    else
    {
        dBodySetGravityMode(player->getBody(), false);
        info->cursor->setVisible(true);
        
        //player->setPosition(camera->getPosition());
        camera->setPosition(player->getPosition());
        
        vector3df camVec = camera->getTarget() - camera->getPosition();
        
        dGeomRaySet(dSelectRayGeom, camera->getPosition().X, camera->getPosition().Y, camera->getPosition().Z,
        camVec.X,camVec.Y,camVec.Z);
        selectRayCollision.reset();
        
        // saving and loading ?
        if (playerEvent.keyCode[KEY_SAVE])
        {
            save();
        }
    }
    
    // drawing
    info->driver->beginScene(true, true, SColor(0,200,200,200));
    info->smgr->drawAll();
    info->guienv->drawAll();
    info->driver->endScene();
}

void Level::Start(Info *info, int id)
{
    this->info = info;
    this->id = id;
    
    levelString = "level/lev_";
    levelString += id;
    levelString += ".data";
    
    info->cursor->setVisible(false);
    camera = info->smgr->addCameraSceneNodeFPS();
    //info->smgr->setShadowColor(SColor(150,0,0,0));
	
	camera->setFarValue(8200);
	camera->setFOV(PI/2.5f);
	
	// light
	ILightSceneNode *light = info->smgr->addLightSceneNode(camera, vector3df(0,1000,0), SColorf(0.6f, 0.6f, 0.6f, 0.6f), 2000.0f);
	SLight &slight = light->getLightData();
	slight.AmbientColor = SColorf(0.5,0.5,0.5,0.5);
    
	// ODE physics
	dWorld = dWorldCreate();
	dWorldSetERP(dWorld, 0.2);
	dWorldSetContactSurfaceLayer(dWorld, 0.001);
	dWorldSetCFM(dWorld, 0.00001);
	dSpace = dSimpleSpaceCreate(0);
	dJointGroup = dJointGroupCreate(0);
	dWorldSetGravity(dWorld, 0, -500, 0);
	
	// some temporary pointers for scenery
	IAnimatedMesh *mesh;
	IAnimatedMeshSceneNode *node;
	vector3df pos;
	vector3df scale;
	
	// create walls
	mesh = info->smgr->getMesh("mesh.x");
    node = info->smgr->addAnimatedMeshSceneNode(mesh);
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(EMF_LIGHTING, true);
    // position it
    pos = vector3df(0, 0, 0);
    scale = vector3df(10, 10, 10);
    node->setPosition(pos);
    node->setScale(scale);
    node->updateAbsolutePosition();
    // create ODE mesh for world
    meshes.push_back(new odeMesh(dSpace, mesh, scale, pos));
    
    // floor
	mesh = info->smgr->getMesh("tiled_floor.3ds");
	IMesh *tangentMesh = info->smgr->getMeshManipulator()->createMeshWithTangents(mesh->getMesh(0));
	
    node = info->smgr->addAnimatedMeshSceneNode(mesh);
    pos = vector3df(0,-100,0);
    scale = vector3df(1,1,1);
    node->setPosition(pos);
    meshes.push_back(new odeMesh(dSpace, mesh, scale, pos));
    
    node->setMaterialType(EMT_NORMAL_MAP_SOLID);
    //info->driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
    //node->setMaterialType(EMT_PARALLAX_MAP_SOLID);
    //node->getMaterial(0).MaterialTypeParam = 0.35f; // adjust height for parallax effect
    
    ITexture *normalMap = info->driver->getTexture("tiles_bump.bmp");
    info->driver->makeNormalMapTexture(normalMap, 9.0f);
    node->setMaterialTexture(1,	normalMap);
    
    // use following to check if renderer is possible!
    //IMaterialRenderer *renderer = info->river->getMaterialRenderer(EMT_NORMAL_MAP_SOLID);
    
    //save();
    load();
    
    // add player
    player = new Player(info, dWorld, dSpace);
    player->setPosition(vector3df(0, 10, 0));
    
    mag = new Magnet(info, dWorld, dSpace, vector3df(0, 2000, 0));
    
    // set player event
    playerEvent.setInfo(info);
    playerEvent.Reset();
    info->device->setEventReceiver(&playerEvent);
    
    // selection ray for level editing
    selectRayCollision.type = COLLISION_SELECTRAY;
    selectRayCollision.data = (void *)this;
    dSelectRayGeom = dCreateRay(dSpace, 100000);
    dGeomSetData(dSelectRayGeom, (void *)&selectRayCollision);
    
    // misc
    info->levelEditing = false;
    selectedBlock = NULL;
}

void Level::Finish()
{
    // delete meshes first as the deconstructor destroys ODE bodies.
    // otherwise dWorldDestroy(?) would remove them leaving invalid pointers.
    for (i = 0; i < meshes.size(); i ++)
    {
        delete meshes[i];
    }
    
    delete player;
    
    dSpaceDestroy(dSpace);
    dWorldDestroy(dWorld);
    dJointGroupDestroy(dJointGroup);
}

void Level::dNearCallback(void *data, dGeomID o1, dGeomID o2)
{
    // maybe have MAX_CONTACTS and/or actual contacts in collision stuct
    Level *me = (Level *)data;
    
    if (dGeomIsSpace (o1) || dGeomIsSpace (o2)) return;
    
    const int MAX_CONTACTS = 12; // per body
    
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    
    // exit without doing anything if the two bodies are connected by a joint    
    //if (b1 && b2 && dAreConnected (b1, b2))return;
    if(b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact)) return;
    
    Collision *col1 = (Collision *)dGeomGetData(o1);
    Collision *col2 = (Collision *)dGeomGetData(o2);
    
    dContact contact[MAX_CONTACTS];
    for (int i = 0; i < MAX_CONTACTS; i++)
    {
        // if collision structure has a material set, use that.
        if (col1)
        {
            if (col1->surfaceSet)
            {
                contact[i].surface = col1->surface;
                continue;
            }
        }
        
        // default material
        contact[i].surface.mode = dContactBounce | dContactSoftCFM;
        contact[i].surface.mu = 0;
        contact[i].surface.bounce = 0;
        contact[i].surface.bounce_vel = 0;
        contact[i].surface.soft_cfm = 0;
    }
    
    int numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));
    
    if (numc == 0) return;
    
    if (col1 && col2)
    {
        // if magnet colliding with block
        if (col1->type == COLLISION_BLOCK && col2->type == COLLISION_MAGNET)
        {
            if (!me->playerEvent.keyCode[KEY_MAGRELEASE]) ((Magnet *)col2->data) -> attachBlock((Block *)col1->data, contact[0].geom);
        }
        // if level editing, blocks can be selected
        if (col1->type == COLLISION_SELECTRAY && col2->type == COLLISION_BLOCK && me->info->levelEditing)
        {
            // press jump key to select blocks
            if (me->playerEvent.keyCode[KEY_JUMP])
            {
                // add block and its depth to lists
                // actual selected block will be the one with closest depth
                me->selectedBlocks.push_back((Block *)col2->data);
                me->selectedDepths.push_back(contact[0].geom.depth);
            }
        }
    }
    
    // set number of collisions for geom. if used, refresh must collision.reset() every frame.
    for (int i = 0; i < 2; i ++)
    {
        dGeomID geom;
        Collision *col;
        if (i == 0) geom = o1;
        if (i == 1) geom = o2;
        col = (Collision *)dGeomGetData(geom);
        if (col)
        {
            col->add();
            if (dGeomGetClass(geom) == dRayClass) col->setRayContact(contact[0].geom);
        }
    }
    
    // if either are rays, we don't need contact joints
    if ( (dGeomGetClass(o1) == dRayClass) || (dGeomGetClass(o2) == dRayClass))
    {
        return;
    }
    
    for (int i = 0; i < numc; i++)
    {
        dJointID c = dJointCreateContact(me->dWorld, me->dJointGroup, contact+i);
        dJointAttach (c, b1, b2);
    }
}

void Level::load()
{
    blocks.clear();
    
    const c8 *fname = levelString.c_str();
    
    if (!info->fs->existFile(fname))
    {
        save();
    }
    
    FILE *f = fopen(fname, "r");
    
    u32 size;
    fread(&size, sizeof(u32), 1, f);
    
    for (i = 0; i < size; i ++)
    {
        // type
        int type;
        fread(&type, sizeof(int), 1, f);
        
        // position
        vector3df pos;
        fread(&pos.X, sizeof(f32), 1, f);
        fread(&pos.Y, sizeof(f32), 1, f);
        fread(&pos.Z, sizeof(f32), 1, f);
        
        // rotation
        dQuaternion quat;
        fread(quat, sizeof(dReal), 4, f);
        
        // create the block
        Block *block = new Block(info, dWorld, dSpace, type);
        block->setPosition(pos);
        
        dBodyID b = block->getBody();
        dBodySetQuaternion(b, quat);
        
        //block->setRotation(rot);
        blocks.push_back(block);
    }
    
    fclose(f);
}

void Level::save()
{
    const c8 *fname = levelString.c_str();
    
    FILE *f = fopen(fname, "w");
    
    u32 size;
    
    // number of blocks
    size = blocks.size();
    fwrite(&size, sizeof(u32), 1, f);
    
    for (i = 0; i < blocks.size(); i ++)
    {
        // type
        fwrite(&blocks[i]->type, sizeof(int), 1, f);
        // position
        vector3df pos = blocks[i]->getPosition();
        fwrite(&pos.X, sizeof(f32), 1, f);
        fwrite(&pos.Y, sizeof(f32), 1, f);
        fwrite(&pos.Z, sizeof(f32), 1, f);
        // rotation
        dBodyID b = blocks[i]->getBody();
        dReal *quat = (dReal *)dBodyGetQuaternion(b);
        fwrite(quat, sizeof(dReal), 4, f);
    }
    
    fclose(f);
}





