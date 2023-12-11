///
/// FORMATION DEMO
/// kudos to Rogerborg
///

struct WrapperClass
{
 struct Unit;

 struct Formation
 {
    f32 Rotation;
    vector3df DesiredPosition;
    ISceneNode * Visual;
    f32 Speed;
    array<Unit *> Units;
    u32 MaxFiles; // Add files to each rank until full
    f32 FileWidth;
    f32 RankDepth;
    ITerrainSceneNode * Terrain;

    Formation(const vector3df & position,
              f32 speed,
              u32 maxFiles,
              int initialUnits,
              f32 fileWidth,
              f32 rankDepth,
    //          ITerrainSceneNode * terrain,
              ISceneManager * smgr)
    {
        Rotation = 0.f;
        Speed = speed;
        MaxFiles = maxFiles;
        FileWidth = fileWidth;
        RankDepth = rankDepth;
     //   Terrain = terrain;
        DesiredPosition = position;

        Visual = smgr->addAnimatedMeshSceneNode(smgr->addArrowMesh("formation arrow", SColor(255*.4, 0, 255, 0), SColor(255, 0, 255, 0), 4, 8,
                                                100.f, 60.f, 5.f, 30.f));
        Visual->setMaterialFlag(video::EMF_LIGHTING, false);
        Visual->setPosition(DesiredPosition);
        Visual->setRotation(vector3df(90, 0, 0));
        Visual->updateAbsolutePosition();

        for(int unit = 0; unit < initialUnits; ++unit)
        {
            position2di rankAndFile(unit / maxFiles, unit % maxFiles);

            Units.push_back(new Unit(this, rankAndFile, smgr));
        }
    }

    ~Formation()
    {
        for(u32 unit = 0; unit < Units.size(); ++unit)
            delete Units[unit];
    }

    void process(f32 deltaTime)
    {
        vector3df currentPosition = calculateMovePosition(Visual->getAbsolutePosition(),
                                                    DesiredPosition,
                                                    Speed,
                                                    deltaTime,
                                                    Rotation);
        currentPosition.Y += Visual->getBoundingBox().getExtent().Y;

        Visual->setPosition(currentPosition);
        Visual->setRotation(vector3df(90, Rotation, 0));
        Visual->updateAbsolutePosition();

        for(u32 unit = 0; unit < Units.size(); ++unit)
            if(Units[unit])
                Units[unit]->process(deltaTime);
    }

    vector3df getDesiredUnitPosition(const position2di & rankAndFile)
    {
        vector3df position(FileWidth * (rankAndFile.X - (.5f * MaxFiles)),
                           0.f,
                           -RankDepth * rankAndFile.Y);

        position.rotateXZBy(-Rotation); // This appears to be bass ackwards compared to getHorizontalAngle().Y

        position += Visual->getAbsolutePosition();

        return position;
    }

    vector3df calculateMovePosition(vector3df currentPosition,
                              const vector3df & desiredPosition,
                              f32 speed,
                              f32 deltaTime,
                              f32 & rotation)
    {
        const f32 availableMovement = speed * deltaTime;
        vector3df desiredMovement = desiredPosition - currentPosition;
        desiredMovement.Y = 0.f;
        const f32 desiredMovementMagnitudeSq = desiredMovement.getLengthSQ();

        if(desiredMovementMagnitudeSq > 0.f)
        {
            if(desiredMovementMagnitudeSq <= availableMovement * availableMovement)
            {
                currentPosition = desiredPosition;
            }
            else
            {
                currentPosition += desiredMovement.normalize() * availableMovement;
            }

            rotation = desiredMovement.getHorizontalAngle().Y;
        }

      //  currentPosition.Y = Terrain->getHeight(currentPosition.X, currentPosition.Z);

        return currentPosition;
    }
 };

 struct Unit
 {
    Formation * ParentFormation;
    ISceneNode * Visual;
    position2di RankAndFile;
    f32 Speed;
    static const f32 SpeedMultiplier=1.1;

    Unit(Formation * formation, const position2di & rankAndFile, ISceneManager * smgr)
    {
        ParentFormation = formation;
        RankAndFile = rankAndFile;
        Visual = smgr->addAnimatedMeshSceneNode(smgr->addArrowMesh("pawn", SColor(255, 255, 0, 0), SColor(255, 0, 255, 255), 4, 8,
                                                100.f, 60.f, 5.f, 30.f));
        Visual->setMaterialFlag(video::EMF_LIGHTING, false);
        Visual->setPosition(ParentFormation->getDesiredUnitPosition(RankAndFile));
        Visual->updateAbsolutePosition();

        Speed = ParentFormation->Speed * SpeedMultiplier;
        // Randomise this a little
        Speed *= .9f + 2.f * ((f32)rand() / (f32)RAND_MAX);
    }



    virtual void process(f32 deltaTime)
    {
        f32 fakeRotation;
        vector3df currentPosition = ParentFormation->calculateMovePosition(Visual->getAbsolutePosition(),
                                                                           ParentFormation->getDesiredUnitPosition(RankAndFile),
                                                                           Speed,
                                                                           deltaTime,
                                                                           fakeRotation);

        currentPosition.Y += Visual->getBoundingBox().getExtent().Y;

        Visual->setPosition(currentPosition);
    }
//const f32 WrapperClass::Unit::SpeedMultiplier = 1.1f;

 }; // Unit


}; // WrapperClass
