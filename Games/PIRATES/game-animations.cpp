/// This class is part of Game class.
/// So depends of objects and variables already created in the Game class.


/// King orders balloons: (Set global vars for camera target use too)
f32 kingOrderBalloonDuration = 0.0;
bool kingOrderWave2 = false;
stringw kingOrder = "";
/// Shark 2: (Set global vars for camera target use too)
bool cameraFollowShark2 = false;
/// Musicians animated:
bool musiciansAnimate = false;
/// GAME OVER (do only once loop, reseted on key press)
bool game_over_once = true;




class Animations : public Game {

    public:

    Animations();  // Constructor
    ~Animations(); // Destructor

    void Reset();
    void Initialize();
    void Loop();

    /// TOOL: Convert 3D positions to 2D positions:
    position2d<s32> Convert3DTo2DPositions(vector3df pos3D);

    /// void BlinkSoldierEyes(std::vector<IRigidBody*> bodySoldier, std::vector<IAnimatedMeshSceneNode*> nodeSoldier);
    void AudiosLoop();
    void SkyAnimatedLoop();
    void Shark1MoveLeftRightLoop();
    void Shark2MoveRightLoop();
    void BirdMoveLeftRightLoop();
    void WorkersLoop();
    void MusiciansLoop();
    void BullsLoop();
    void PriestsDrunkLoop();
    void KnightMoveLoop();

    void PrisonersArrestedLoop();
    void PrisonersArrestedBalloonLooping();
    void PrisonersEscapingLoop();
    void RemovePrisonersEscaping();
    void PrisonersEscapingBalloonLooping();

    void GameOverLoop();

    void ChickenMoveLeftRightLoop();
    void BlinkSoldierEyesLoop();
    void PlatformLoop();
    void PriestsLoop();
    void PriestsWalkingLoop();
    void PriestsScared();
    void BodyJumps(IRigidBody* body, f32 force);
    void KingLooping();
    void SoldiersLoop();
    void FlagsMovingLoop();
    void PirateShipBalancingLoop();

    /// A big wave:
    void HitWave2(vector3df pos);
    void HitWave2Loop();

    /// A very small wave:
    void HitWave3(vector3df pos);
    void HitWave3Loop();

    private:

    /// Ship:
    bool ship_balancing_right  = true;
    bool ship_balancing_up = true;
    /// Soldiers:
    /// bool soldiers_anim_forward = true;
    /// Shark 1:
    bool shark1_moving_right = true;
    bool shark1_moving_up = true;
    /// Shark 2:
    bool shark2_pursuit = false;
    f32 shark2_timer = 0.0;
    /// Priests:
    bool priests_scared = false;

    /// GAME OVER:
    /// bool game_over_once = true;
    f32 game_over_timer = 0.0;


    f32 blinkSoldierEyesDuration = 0.0;
    f32 blinkPrisonerEyesDuration = 0.0;
    f32 scaredPriestDuration = 1000.0;
    f32 platformStopped = 1000.0;
    f32 priestDrinkWait = 0.0;

    int seaWaveTextNum = 0;
    f32 seaWaveStandbyTimer = 0.0;
    f32 seaWaveTextureTimer = 0.0;


    /// King Orders Balloons:
    std::vector<ITexture*> kingOrdersBalloonTextures; /// Balloons for kings orders only


    /// King Random Balloons:
    std::vector<ITexture*> kingRandomBalloonTextures; /// Balloons for kings random only
    f32 kingRandomBalloonDuration = 0.0;
    int king_random_balloon = 0;      /// Random texture number
    int king_random_balloon_next = 0; /// Waiting time until next random balloon
    bool king_random_audio = true;    /// Play audio (talk) only once

    /// King GAME OVER Balloons:
    std::vector<ITexture*> kingGameOverBalloonTextures;  /// Balloons for kings GAME OVER


    /// Drunk Priest Random Balloons:
    std::vector<ITexture*> drunkPriestsRandomBalloonTextures; /// Balloons for drunk priests random only
    f32 drunkPriestsRandomBalloonDuration = 0.0;
    int drunkPriests_random_balloon = 0;      /// Random texture number
    int drunkPriests_random_balloon_next = 0; /// Waiting time until next random balloon
    bool drunkPriests_random_audio = true;    /// Play audio (talk) only once
    /// Drunk Priest animation:
    int drinkTime = 1000;


    /// Prisoners Arrested Balloons:
    IRigidBody* bodyPrisonerForBalloons = 0;
    std::vector<ITexture*> prisonersArrestedBalloonTextures; /// Balloons for arrested prisoners only
    vector3df balloonPosition; /// handles position, even if its body was removed
    bool doBalloon = false;
    f32 balloonDuration = 0.0;
    int randomPrisonerBalloon = 0;


    /// Prisoners Escaping Balloons:
    std::vector<ITexture*> prisonersEscapingBalloonTextures; /// Balloons for escaping prisoners only
    f32 escaping_balloon_timer = 0.0;
    int escaping_random_ballon = 0;


    /// Soldiers Clear Random Balloons:
    std::vector<ITexture*> soldiersClearRandomBalloonTextures; /// Balloons for drunk priests random only
    f32 soldiersClearRandomBalloonDuration = 0.0;
    int soldiersClear_random_balloon = 0;      /// Random texture number
    int soldiersClear_random_balloon_next = 0; /// Waiting time until next random balloon
    bool soldiersClear_random_audio = true;    /// Play audio (talk) only once
    /// Pick random soldiers body:
    int soldiersClear_random_balloon_body = 0;


    /// Hit Waves 2: (Big ones)
    core::array<video::ITexture*> hitWaves2Textures;
    vector3df hitWave2Position = vector3df(0.0,0.0,0.0);
    bool hitDoWave2Loop = false;
    unsigned int hitWave2TextNum = 0;
    f32 hitWave2TextureTimer = 0.0;

    /// Hit Waves 3: (Very small ones)
    core::array<video::ITexture*> hitWaves3Textures;
    vector3df hitWave3Position = vector3df(0.0,0.0,0.0);
    bool hitDoWave3Loop = false;
    unsigned int hitWave3TextNum = 0;
    f32 hitWave3TextureTimer = 0.0;


    /// Only a pointer to other classes already included:
    Audios* audios;
    Scene* scene;

};

Animations::Animations(){};  // Constructor (Can accept arguments, but this one does not)
Animations::~Animations(){}; // Destructor (do not accept arguments)




void Animations::Reset(){

    /// Reset:
    prisonersArrestedBalloonTextures.clear();
    /// Reset:
    prisonersEscapingBalloonTextures.clear();
    /// Reset:
    kingOrdersBalloonTextures.clear();
    /// Reset:
    kingRandomBalloonTextures.clear();
    /// Reset:
    kingGameOverBalloonTextures.clear();
    /// Reset:
    drunkPriestsRandomBalloonTextures.clear();
    /// Reset:
    soldiersClearRandomBalloonTextures.clear();
    /// Reset:
    hitWaves2Textures.clear();
    /// Reset:
    hitWaves3Textures.clear();
};




void Animations::Initialize(){

    /// Reset:
    Reset();  cout << "Animations->Reset() OK" << endl;

    /// Fill:
    prisonersArrestedBalloonTextures.push_back(driver->getTexture("balloon_prisoner_01.png"));
    prisonersArrestedBalloonTextures.push_back(driver->getTexture("balloon_prisoner_02.png"));
    prisonersArrestedBalloonTextures.push_back(driver->getTexture("balloon_prisoner_03.png"));
    prisonersArrestedBalloonTextures.push_back(driver->getTexture("balloon_prisoner_04.png"));
    prisonersArrestedBalloonTextures.push_back(driver->getTexture("balloon_prisoner_05.png"));
    prisonersArrestedBalloonTextures.push_back(driver->getTexture("balloon_prisoner_06.png"));
    prisonersArrestedBalloonTextures.push_back(driver->getTexture("balloon_prisoner_07.png"));
    prisonersArrestedBalloonTextures.push_back(driver->getTexture("balloon_prisoner_08.png"));
    prisonersArrestedBalloonTextures.push_back(driver->getTexture("balloon_prisoner_09.png"));
    prisonersArrestedBalloonTextures.push_back(driver->getTexture("balloon_prisoner_10.png"));

    /// Fill:
    prisonersEscapingBalloonTextures.push_back(driver->getTexture("balloon_prisoner_escaping_01.png"));
    prisonersEscapingBalloonTextures.push_back(driver->getTexture("balloon_prisoner_escaping_02.png"));
    prisonersEscapingBalloonTextures.push_back(driver->getTexture("balloon_prisoner_escaping_03.png"));
    prisonersEscapingBalloonTextures.push_back(driver->getTexture("balloon_prisoner_escaping_04.png"));
    prisonersEscapingBalloonTextures.push_back(driver->getTexture("balloon_prisoner_escaping_05.png"));
    prisonersEscapingBalloonTextures.push_back(driver->getTexture("balloon_prisoner_escaping_06.png"));

    /// Fill:
    kingOrdersBalloonTextures.push_back(driver->getTexture("balloon_king_01.png"));
    kingOrdersBalloonTextures.push_back(driver->getTexture("balloon_king_02.png"));
    kingOrdersBalloonTextures.push_back(driver->getTexture("balloon_king_03.png"));
    kingOrdersBalloonTextures.push_back(driver->getTexture("balloon_king_04.png"));

    /// Fill:
    kingRandomBalloonTextures.push_back(driver->getTexture("balloon_king_random_01.png"));
    kingRandomBalloonTextures.push_back(driver->getTexture("balloon_king_random_02.png"));
    kingRandomBalloonTextures.push_back(driver->getTexture("balloon_king_random_03.png"));
    kingRandomBalloonTextures.push_back(driver->getTexture("balloon_king_random_04.png"));

    /// Fill:
    kingGameOverBalloonTextures.push_back(driver->getTexture("balloon_king_game_over_01.png"));

    /// Fill:
    drunkPriestsRandomBalloonTextures.push_back(driver->getTexture("balloon_drunk_random_01.png"));
    drunkPriestsRandomBalloonTextures.push_back(driver->getTexture("balloon_drunk_random_02.png"));
    drunkPriestsRandomBalloonTextures.push_back(driver->getTexture("balloon_drunk_random_03.png"));
    drunkPriestsRandomBalloonTextures.push_back(driver->getTexture("balloon_drunk_random_04.png"));
    drunkPriestsRandomBalloonTextures.push_back(driver->getTexture("balloon_drunk_random_05.png"));
    drunkPriestsRandomBalloonTextures.push_back(driver->getTexture("balloon_drunk_random_06.png"));
    drunkPriestsRandomBalloonTextures.push_back(driver->getTexture("balloon_drunk_random_07.png"));

    /// Fill:
    soldiersClearRandomBalloonTextures.push_back(driver->getTexture("balloon_soldier_random_01.png"));
    soldiersClearRandomBalloonTextures.push_back(driver->getTexture("balloon_soldier_random_02.png"));
    soldiersClearRandomBalloonTextures.push_back(driver->getTexture("balloon_soldier_random_03.png"));

    /// Fill:
    hitWaves2Textures.push_back(driver->getTexture("hit_waves_type2_01.png"));
    hitWaves2Textures.push_back(driver->getTexture("hit_waves_type2_02.png"));
    hitWaves2Textures.push_back(driver->getTexture("hit_waves_type2_03.png"));
    hitWaves2Textures.push_back(driver->getTexture("hit_waves_type2_04.png"));
    hitWaves2Textures.push_back(driver->getTexture("hit_waves_type2_05.png"));
    hitWaves2Textures.push_back(driver->getTexture("hit_waves_type2_06.png"));
    hitWaves2Textures.push_back(driver->getTexture("hit_waves_type2_07.png"));

    /// Fill:
    hitWaves3Textures.push_back(driver->getTexture("hit_waves_type3_01.png"));
    hitWaves3Textures.push_back(driver->getTexture("hit_waves_type3_02.png"));
    hitWaves3Textures.push_back(driver->getTexture("hit_waves_type3_03.png"));
    hitWaves3Textures.push_back(driver->getTexture("hit_waves_type3_04.png"));

};




/// Constant and Random audios:
void Animations::AudiosLoop(){

    /// Random:
    if(audios){
    int r1 = rand()%2000;
    if(r1==200){ audios->PlayFx(21, cameras[0]->getPosition()); }; /// Horse audio
    int r2 = rand()%5000;
    if(r2==200){ audios->PlayFx(22, cameras[0]->getPosition()); }; /// Cock audio
    if(r2==210){ audios->PlayFx(24, cameras[0]->getPosition()); }; /// Yell audio
    if(r2==220){ audios->PlayFx(27, cameras[0]->getPosition()); }; /// Bull audio
    if(r2==230){ audios->PlayFx(28, cameras[0]->getPosition()); }; /// Hammer audio
    if(r2==240){ audios->PlayFx(29, cameras[0]->getPosition()); }; /// Door audio

    /// Constant:
    audios->PlayFx(36, vector3df(4700,0,0)); /// Fountain
    };

};


void Animations::SkyAnimatedLoop(){
    if(nodeSky){
    static f32 uu = 0.0;
    static f32 vv = 0.0;
    /// Animate textures --> http://www.irrlicht3d.org/wiki/index.php?n=Main.HowToUseTexturesAndMaterials
    nodeSky->getMaterial(0).getTextureMatrix(0).setTextureTranslate(uu,vv); /// Placard ---> Range is clipped between 0 and 1 with the default initial value of 0.5.
    uu += 0.0000050*frameDeltaTime; // vv += 0.0005;
    if(uu>1.0 || vv>1.0){ uu = 0.0; vv = 0.0; };
    };
};


/// ANIMATION: Shark 1
void Animations::Shark1MoveLeftRightLoop(){
    if(nodeShark1!=0){
    /// X Positions:
    vector3df pos = nodeShark1->getPosition();
    vector3df rot = nodeShark1->getRotation();
    if(shark1_moving_right == true){
       rot.Y = -90; /// Face right
       pos.X += 0.160*frameDeltaTime;
       if(pos.X>=1500){ pos.X = 1500; shark1_moving_right = false; };
    };
    if(shark1_moving_right == false){
       rot.Y = 90; /// Face left
       pos.X -= 0.160*frameDeltaTime;
       if(pos.X<=-1500){ pos.X = -1500; shark1_moving_right = true; };
    };
    /// Y Positions:
    if(shark1_moving_up == true){
       pos.Y += 0.010*frameDeltaTime;
       if(pos.Y>=-50){ pos.Y = -50; shark1_moving_up = false; };
    };
    if(shark1_moving_up == false){
       pos.Y -= 0.010*frameDeltaTime;
       if(pos.Y<=-70){ pos.Y = -70; shark1_moving_up = true; };
    };
    /// Apply:
    nodeShark1->setPosition(pos);
    nodeShark1->setRotation(rot);
    };
};



/// ANIMATION: Shark 2
void Animations::Shark2MoveRightLoop(){

    /// Calling this loop is: shark2_pursuit = true;

    if(nodeShark2 && shark2_timer>=1000 && shark2_pursuit == true){
    /// Positions:
    vector3df pos = nodeShark2->getPosition();
    vector3df rot = nodeShark2->getRotation();
    rot.Y = -90; /// Face right
    pos.X += 1.250*frameDeltaTime;
    if(pos.X>=7400){ pos.X = 7400; shark2_pursuit = false; audios->PlayFx(24, cameras[0]->getPosition()); };
    /// Apply:
    nodeShark2->setPosition(pos);
    nodeShark2->setRotation(rot);
    nodeShark2->setVisible(true);
    cameraFollowShark2 = true;
    /// Audio:
    audios->PlayFx(30, pos);
    };

    if(nodeShark2 && shark2_pursuit == false){
    /// Apply:
    cameraFollowShark2 = false;
    nodeShark2->setPosition(vector3df(-500,-68,-750));
    nodeShark2->setVisible(false);
    shark2_timer = 0.0;
    };

    shark2_timer+=0.50*frameDeltaTime;
};




/// ANIMATION: Chicken
void Animations::ChickenMoveLeftRightLoop(){

    for(int i=0; i<chickens.size(); i++) {
    if(chickens[i].Node && chickens[i].Body && chickens[i].Body->isInWorld()){

    /// 1) Animate embed animation:
    f32 fstart = chickens[i].Node->getStartFrame();
    f32 fend   = chickens[i].Node->getEndFrame();
    f32 frame  = chickens[i].Node->getFrameNr();
    /// Next frame calculations:
    frame += chickens[i].VelX*frameDeltaTime;
    if(frame<=fstart){ frame = fstart; };
    if(frame>=fend  ){ frame = fstart; };
    /// Apply frame:
    chickens[i].Node->setCurrentFrame((f32)frame);



    /// Reset all forces:
    chickens[i].Body->clearForces();
    /// Transform:
    btTransform trans = chickens[i].Body->getPointer()->getCenterOfMassTransform();
    chickens[i].PosY = trans.getOrigin().getY();
    chickens[i].VelY = chickens[i].Body->getLinearVelocity().Y; /// Get any force already applied on Y (jumping force)
    if(chickens[i].VelY>60){ chickens[i].VelY=60; }; /// Max jump height allowed

    /// Randomly change direction:
    int randChangeDirection = rand()%500;
    if(randChangeDirection==100){ if(chickens[i].MoveRight==true){ chickens[i].MoveRight=false; } else { chickens[i].MoveRight=true; }; };

    /// Moving chicken to one direction, according animation...
    if(chickens[i].Colliding==false){
    /// Get Positions:
    f32 moveX = trans.getOrigin().getX();
    if(chickens[i].MoveRight==true ){ moveX += (chickens[i].VelX*frameDeltaTime)/100; };
    if(chickens[i].MoveRight==false){ moveX -= (chickens[i].VelX*frameDeltaTime)/100; };
    if(moveX >= chickens[i].MaxPosX){ moveX = chickens[i].MaxPosX; chickens[i].MoveRight = false; };
    if(moveX <= chickens[i].MinPosX){ moveX = chickens[i].MinPosX; chickens[i].MoveRight = true;  };
    /// Set Positions:
    chickens[i].Body->setLinearFactor(vector3df(1,1,0)); // Allowed positions axis
    if(chickens[i].MoveRight==true ){ chickens[i].Body->setLinearVelocity(vector3df( chickens[i].VelX, chickens[i].VelY, 0.0f), ERBTransformSpace::ERBTS_WORLD); };
    if(chickens[i].MoveRight==false){ chickens[i].Body->setLinearVelocity(vector3df(-chickens[i].VelX, chickens[i].VelY, 0.0f), ERBTransformSpace::ERBTS_WORLD); };
    trans.setOrigin(btVector3(moveX, chickens[i].PosY, trans.getOrigin().getZ()));
    /// Set Rotations:
    chickens[i].Body->setAngularFactor(vector3df(0,0,0));
    chickens[i].Body->setAngularVelocity(vector3df(0,0,0), ERBTransformSpace::ERBTS_WORLD);
    if(chickens[i].MoveRight==true ){ chickens[i].Rotation -= 0.010*frameDeltaTime; if(chickens[i].Rotation<-1.5){ chickens[i].Rotation = -1.5; }; };
    if(chickens[i].MoveRight==false){ chickens[i].Rotation += 0.010*frameDeltaTime; if(chickens[i].Rotation> 1.5){ chickens[i].Rotation =  1.5; }; };
    trans.setRotation(btQuaternion(chickens[i].Rotation,0,0)); // Reset rotation (Bullet order is: Y,X,Z!)
    };

    /// Moving chicken to one direction, according collision...
    if(chickens[i].Colliding==true){
    /// Set Positions:
    if(chickens[i].MoveRight==true ){ chickens[i].MoveRight = false; /* f32 newX = trans.getOrigin().getX(); newX-=5.0; trans.setOrigin(btVector3(newX, chickens[i].PosY, trans.getOrigin().getZ())); */ };
    if(chickens[i].MoveRight==false){ chickens[i].MoveRight = true;  /* f32 newX = trans.getOrigin().getX(); newX+=5.0; trans.setOrigin(btVector3(newX, chickens[i].PosY, trans.getOrigin().getZ())); */ };
    chickens[i].Colliding = false;
    };

    /// Apply:
    chickens[i].Body->getPointer()->getMotionState()->setWorldTransform(trans); /// Update physics #1
    chickens[i].Body->getPointer()->setCenterOfMassTransform(trans); /// Update physics #2

    };
    };
};




/// ANIMATION: Bird
void Animations::BirdMoveLeftRightLoop(){

    for(int i=0; i<birds.size(); i++) {
    if(birds[i].Node && birds[i].Body && birds[i].Body->isInWorld()){

    /// Get speed for both animations:
    f32 speed = birds[i].Speed;

    /// Animate embed animation:
    /// Set animation frames limits:
    f32 fstart = birds[i].Node->getStartFrame();
    f32 fend   = birds[i].Node->getEndFrame();
    f32 frame  = birds[i].Node->getFrameNr();
    /// Next frame calculations:
    frame += speed*frameDeltaTime;
    if(frame<=fstart){ frame = fstart; };
    if(frame>=fend  ){ frame = fstart; };
    /// Apply frame:
    birds[i].Node->setCurrentFrame((f32)frame);


    /// 1st) Get body transform:
    btTransform trans = birds[i].Body->getPointer()->getCenterOfMassTransform();
    f32 PosX = trans.getOrigin().getX();
    f32 PosY = trans.getOrigin().getY();

    /// a) Calculations: X Positions and rotation facing the direction...
    if(birds[i].MovingRight == true){
       PosX += (speed/20)*frameDeltaTime;
       if(PosX>=8000){ PosX = 8000; birds[i].MovingRight = false; };
       trans.setRotation(btQuaternion(55,0,0)); /// Rotation (Bullet order is: Y,X,Z!)
    };
    if(birds[i].MovingRight == false){
       PosX -= (speed/20)*frameDeltaTime;
       if(PosX<=-2000){ PosX = -2000; birds[i].MovingRight = true; };
       trans.setRotation(btQuaternion(-55,0,0)); /// Rotation (Bullet order is: Y,X,Z!)
    };

    /// b) Calculations: Y Positions...
    if(birds[i].MovingUp == true){
       PosY += (speed/200)*frameDeltaTime;
       if(PosY>=1450){ PosY = 1450; birds[i].MovingUp = false; };
    };
    if(birds[i].MovingUp == false){
       PosY -= (speed/200)*frameDeltaTime;
       if(PosY<=1200){ PosY = 1200; birds[i].MovingUp = true; };
    };


    /// 2nd) Apply new positions to body:
    trans.setOrigin(btVector3(PosX, PosY, trans.getOrigin().getZ())); /// Apply new position - btVector3(X,Y,Z)
    birds[i].Body->getPointer()->getMotionState()->setWorldTransform(trans); /// Update physics #1
    birds[i].Body->getPointer()->setCenterOfMassTransform(trans); /// Update physics #2

    };
    };
};



/// ANIMATION: Platform
void Animations::PlatformLoop(){

    if(platformStopped>=1000.0){
    if(chains[0].Body && chains[0].Body->isInWorld()){

    /// 1st) Get body transform:
    btTransform trans = chains[0].Body->getPointer()->getCenterOfMassTransform();
    f32 PosX = trans.getOrigin().getX();
    f32 PosY = trans.getOrigin().getY();

    /// frameDeltaTime could be too much (ex: when user is dragging the window)
    f32 deltatime = frameDeltaTime; if(deltatime>50.0){ deltatime=50.0; };

    /// a) Calculations: X Positions and rotation facing the direction...
    if(chains[0].MovingRight == true){
    PosX += 0.12*deltatime;
    PosY += 0.01*deltatime;
    audios->PlayFx(19, vector3df(5600,0,0)); /// Worker winch audio
    if(PosY>=1020){ PosY = 1020; }; /// Must match -->Scene::CreateChains() !!!
    if(PosX>=5400){
    PosY = 1020; /// Must match -->Scene::CreateChains() !!!
    PosX = 5400;
    chains[0].MovingRight = false;
    platformStopped = 0.0;
    /// HitWave 2:
    for(int c=0; c<cargos.size(); c++) {
    if(cargos[c].Body && cargos[c].Body->isInWorld()){
    vector3df pos = cargos[c].Body->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)
    HitWave2(pos);
    };
    };
    /// Audio:
    audios->PlayFx(34, vector3df(5300,0,0)); /// Laser audio
    /// REMOVE Cargos:
    scene->RemoveCargos();
    };
    // cout << "PosY: " << PosY << endl;
    };
    if(chains[0].MovingRight == false){
    PosX -= 0.12*deltatime;
    PosY -= 0.01*deltatime;
    audios->PlayFx(19, vector3df(5600,0,0)); /// Worker winch audio
    if(PosY<=890){ PosY = 890; }; /// Must match -->Scene::CreateChains() !!!
    if(PosX<=3850){
    PosY = 890; /// Must match -->Scene::CreateChains() !!!
    PosX = 3850;
    chains[0].MovingRight = true;
    platformStopped = 0.0;
    audios->PlayFx(34, vector3df(3850,0,0)); /// Laser audio
    audios->PlayFx(13, vector3df(3850,0,0)); /// Barrels bump audio
    /// CREATE Cargos:
    scene->CreateCargos();
    /// HitWave 2:
    for(int c=0; c<cargos.size(); c++) {
    if(cargos[c].Body && cargos[c].Body->isInWorld()){
    vector3df pos = cargos[c].Body->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)
    HitWave2(pos);
    };
    };
    };
    // cout << "PosY: " << PosY << endl;
    };

    /// 2nd) Apply new positions to body:
    trans.setOrigin(btVector3(PosX, PosY, trans.getOrigin().getZ())); /// Apply new position - btVector3(X,Y,Z)
    chains[0].Body->getPointer()->getMotionState()->setWorldTransform(trans); /// Update physics #1
    chains[0].Body->getPointer()->setCenterOfMassTransform(trans); /// Update physics #2

    };
    };

    platformStopped += 0.35*frameDeltaTime;

};


/*
/// ANIMATION: Platform
void Animations::PlatformLoop(){

    if(platformStopped>=1000.0){
    if(chains[0].Body){
    if(chains[0].Body->isInWorld()){

    /// 1st) Get body transform:
    btTransform trans = chains[0].Body->getPointer()->getCenterOfMassTransform();
    f32 PosX = trans.getOrigin().getX();

    /// a) Calculations: X Positions and rotation facing the direction...
    if(chains[0].MovingRight == true){
    PosX += 0.12*frameDeltaTime;
    audios->PlayFx(19, vector3df(5600,0,0)); /// Worker winch audio
    if(PosX>=5200){
    PosX = 5200; chains[0].MovingRight = false; platformStopped = 0.0;
    audios->PlayFx(13, vector3df(5200,0,0)); /// Barrels bump audio
    scene->RemoveCargos();
    };
    };
    if(chains[0].MovingRight == false){
    PosX -= 0.12*frameDeltaTime;
    audios->PlayFx(19, vector3df(5600,0,0)); /// Worker winch audio
    if(PosX<=3850){
    PosX = 3850; chains[0].MovingRight = true;  platformStopped = 0.0;
    audios->PlayFx(13, vector3df(3850,0,0)); /// Barrels bump audio
    scene->CreateCargos();
    };
    };

    /// 2nd) Apply new positions to body:
    trans.setOrigin(btVector3(PosX, trans.getOrigin().getY(), trans.getOrigin().getZ())); /// Apply new position - btVector3(X,Y,Z)
    chains[0].Body->getPointer()->getMotionState()->setWorldTransform(trans); /// Update physics #1
    chains[0].Body->getPointer()->setCenterOfMassTransform(trans); /// Update physics #2

    };
    };
    };

    platformStopped += 0.35*frameDeltaTime;

};
*/

/// ANIMATION: Workers
void Animations::WorkersLoop(){

    if(platformStopped>=1000.0){
    for(int i=0; i<workers.size(); i++) {
    if(workers[i].Node){
    /// Get speed:
    f32 speed = workers[i].Speed;
    /// Animate embed animation:
    f32 fstart = workers[i].Node->getStartFrame();
    f32 fend   = workers[i].Node->getEndFrame();
    f32 frame  = workers[i].Node->getFrameNr();
    /// Next frame calculations:
    frame += speed*frameDeltaTime;
    if(frame<=fstart){ frame = fstart; };
    if(frame>=fend  ){ frame = fstart; };
    /// Apply frame:
    workers[i].Node->setCurrentFrame((f32)frame);
    };
    };
    };
};



/// ANIMATION: Musicians
void Animations::MusiciansLoop(){

    if(musiciansAnimate == true){
    for(int i=0; i<musicians.size(); i++) {
    if(musicians[i].Node){
    /// Get speed:
    f32 speed = musicians[i].Speed;
    /// Animate embed animation:
    f32 fstart = musicians[i].Node->getStartFrame();
    f32 fend   = musicians[i].Node->getEndFrame();
    f32 frame  = musicians[i].Node->getFrameNr();
    /// Next frame calculations:
    frame += speed*frameDeltaTime;
    if(frame<=fstart){ frame = fstart; };
    if(frame>=fend  ){ frame = fstart; };
    /// Apply frame:
    musicians[i].Node->setCurrentFrame((f32)frame);
    /// Apply texture to my specified material
    for(int m=0; m<musicians[i].Node->getMaterialCount(); m++){
	musicians[i].Node->getMaterial(m).setTexture(0, driver->getTexture("musicians_01A.jpg"));
	};
    };
    };
    };

    if(musiciansAnimate == false){
    for(int i=0; i<musicians.size(); i++) {
    if(musicians[i].Node){
    /// Animate embed animation:
    f32 fstart = musicians[i].Node->getStartFrame();
    /// Apply frame:
    musicians[i].Node->setCurrentFrame((f32)fstart);
    /// Apply texture to my specified material
    for(int m=0; m<musicians[i].Node->getMaterialCount(); m++){
	musicians[i].Node->getMaterial(m).setTexture(0, driver->getTexture("musicians_01B.jpg"));
	};
    };
    };
    };
};





/// ANIMATION: Bulls
void Animations::BullsLoop(){

    for(int i=0; i<bulls.size(); i++) {
    if(bulls[i].Node){
    /// Get speed:
    f32 speed = bulls[i].Speed;
    /// Animate embed animation:
    f32 fstart = bulls[i].Node->getStartFrame();
    f32 fend   = bulls[i].Node->getEndFrame();
    f32 frame  = bulls[i].Node->getFrameNr();
    /// Next frame calculations:
    frame += speed*frameDeltaTime;
    if(frame<=fstart){ frame = fstart; };
    if(frame>=fend  ){ frame = fstart; };
    /// Apply frame:
    bulls[i].Node->setCurrentFrame((f32)frame);
    /// Apply texture to my specified material
    // for(int m=0; m<bulls[i].Node->getMaterialCount(); m++){
	// bulls[i].Node->getMaterial(m).setTexture(0, driver->getTexture("bulls_01A.jpg"));
	// };
    };
    };
};



/// ANIMATION: Priest Drinking
void Animations::PriestsDrunkLoop(){

    /// 1) Animation...
    if(priestDrinkWait>drinkTime){
    IAnimatedMeshSceneNode* node = ((IAnimatedMeshSceneNode*)smgr->getSceneNodeFromName("priest_drunk"));
    if(node){ /// Node exists?...
    /// Animate embed animation:
    f32 fstart = node->getStartFrame();
    f32 fend = node->getEndFrame();
    f32 frame = node->getFrameNr();
    /// Next frame calculations:
    frame += 3.5*frameDeltaTime;
    if(frame<=fstart){ frame = fstart; };
    if(frame>=fend  ){ frame = fstart; priestDrinkWait = 0.0; drinkTime = rand()%8000+1000; };
    /// Apply frame:
    node->setCurrentFrame(frame);

    if(frame>2000 && frame<3000){
    if(platforms[0].Node && !drunkPriestsRandomBalloonTextures.empty()){
        if(drunkPriests_random_audio){
            /// Audio drinking:
            audios->PlayFx(25, platforms[0].Node->getPosition());
            /// Audio talking:
            audios->PlayTalk(5, platforms[0].Node->getPosition());
        drunkPriests_random_audio = false;
        };
        /// Do balloon. Needed to enter next loop:
        drunkPriestsRandomBalloonDuration = drunkPriests_random_balloon_next+1;
        drunkPriests_random_balloon = 0; /// Balloon texture 0 == "cheers"
    };
    };
    // cout << "current frame: " << frame << endl;
    };
    };
    priestDrinkWait += 0.20*frameDeltaTime;


    /// 2) Balloon...
    /// Random:
    if(drunkPriestsRandomBalloonDuration==0.0){
    drunkPriests_random_balloon = rand()%(drunkPriestsRandomBalloonTextures.size());
    drunkPriests_random_balloon_next = rand()%25000+5000;
    drunkPriests_random_audio = true;
    };
    /// Show:
    if(drunkPriestsRandomBalloonDuration>drunkPriests_random_balloon_next){
    IAnimatedMeshSceneNode* node = ((IAnimatedMeshSceneNode*)smgr->getSceneNodeFromName("priest_drunk"));
    if(node){ /// Node exists?...
    /// a) Convert 3D to 2D positions:
    vector3df posvector = node->getAbsolutePosition(); /// Get position (in this case, must be the Node Absolute Position!)
    position2d<s32> pos = Convert3DTo2DPositions(posvector);
    pos.X -=  32; // half the image length
    pos.Y -= 110; // above the character
    /// b) DRAW balloon:
    if(drunkPriestsRandomBalloonTextures[drunkPriests_random_balloon]){ driver->draw2DImage(drunkPriestsRandomBalloonTextures[drunkPriests_random_balloon], pos, core::rect<s32>(0,0,64,64), 0, video::SColor(255,255,255,255), true); };
    if(drunkPriests_random_audio){ audios->PlayTalk(5, posvector); HitWave2(posvector); drunkPriests_random_audio = false; };
    };
    };
    /// Timer:
    drunkPriestsRandomBalloonDuration += 1.0*frameDeltaTime;
    /// Hide:
    if(drunkPriestsRandomBalloonDuration>=(drunkPriests_random_balloon_next+2000)){
    drunkPriestsRandomBalloonDuration = 0.0;
    };



};




/// ANIMATION: Knight
void Animations::KnightMoveLoop(){

    for(int i=0; i<knights.size(); i++) {
    if(knights[i].Node && knights[i].Body && knights[i].Body->isInWorld()){

    /// 1) Animate embed animation:
    f32 fstart = knights[i].Node->getStartFrame();
    f32 fend   = knights[i].Node->getEndFrame();
    f32 frame  = knights[i].Node->getFrameNr();
    /// Next frame calculations:
    frame += knights[i].VelX*frameDeltaTime;
    if(frame<=fstart){ frame = fstart; };
    if(frame>=fend  ){ frame = fstart; };
    /// Apply frame:
    knights[i].Node->setCurrentFrame((f32)frame);



    /// Reset all forces:
    knights[i].Body->clearForces();
    /// Transform:
    btTransform trans = knights[i].Body->getPointer()->getCenterOfMassTransform();
    knights[i].PosY = trans.getOrigin().getY();
    knights[i].VelY = knights[i].Body->getLinearVelocity().Y; /// Get any force already applied on Y (jumping force)
    if(knights[i].VelY>60){ knights[i].VelY=60; }; /// Max jump height allowed

    /// Randomly change direction:
    int randChangeDirection = rand()%500;
    if(randChangeDirection==100){ if(knights[i].MoveRight==true){ knights[i].MoveRight=false; } else { knights[i].MoveRight=true; }; };

    /// Moving knights to one direction, according animation...
    /// Get Positions:
    f32 moveX = trans.getOrigin().getX();
    if(knights[i].MoveRight==true ){ moveX += (knights[i].VelX*frameDeltaTime)/100; };
    if(knights[i].MoveRight==false){ moveX -= (knights[i].VelX*frameDeltaTime)/100; };
    if(moveX >= knights[i].MaxPosX){ moveX = knights[i].MaxPosX; knights[i].MoveRight = false; };
    if(moveX <= knights[i].MinPosX){ moveX = knights[i].MinPosX; knights[i].MoveRight = true;  };
    /// Set Positions:
    knights[i].Body->setLinearFactor(vector3df(1,1,0)); // Allowed positions axis
    if(knights[i].MoveRight==true ){ knights[i].Body->setLinearVelocity(vector3df( knights[i].VelX, knights[i].VelY, 0.0f), ERBTransformSpace::ERBTS_WORLD); };
    if(knights[i].MoveRight==false){ knights[i].Body->setLinearVelocity(vector3df(-knights[i].VelX, knights[i].VelY, 0.0f), ERBTransformSpace::ERBTS_WORLD); };
    trans.setOrigin(btVector3(moveX, knights[i].PosY, trans.getOrigin().getZ()));
    /// Set Rotations:
    knights[i].Body->setAngularFactor(vector3df(0,0,0));
    knights[i].Body->setAngularVelocity(vector3df(0,0,0), ERBTransformSpace::ERBTS_WORLD);
    if(knights[i].MoveRight==true ){ knights[i].Rotation -= 0.0045*frameDeltaTime; if(knights[i].Rotation<-1.5){ knights[i].Rotation = -1.5; }; };
    if(knights[i].MoveRight==false){ knights[i].Rotation += 0.0045*frameDeltaTime; if(knights[i].Rotation> 1.5){ knights[i].Rotation =  1.5; }; };
    trans.setRotation(btQuaternion(knights[i].Rotation,0,0)); // Reset rotation (Bullet order is: Y,X,Z!)

    /// Apply:
    knights[i].Body->getPointer()->getMotionState()->setWorldTransform(trans); /// Update physics #1
    knights[i].Body->getPointer()->setCenterOfMassTransform(trans); /// Update physics #2

    };
    };
};




/// ANIMATION: Flags movements:
void Animations::FlagsMovingLoop() {

    for(int i=0; i<flags.size(); i++) {
    if(flags[i].Node){
    /// Get speed:
    f32 speed = flags[i].Speed;
    /// Animate embed animation:
    f32 fstart = flags[i].Node->getStartFrame();
    f32 fend   = flags[i].Node->getEndFrame();
    f32 frame  = flags[i].Node->getFrameNr();
    /// Next frame calculations:
    frame += speed*frameDeltaTime;
    if(frame<=fstart){ frame = fstart; };
    if(frame>=fend  ){ frame = fstart; };
    /// Apply frame:
    flags[i].Node->setCurrentFrame((f32)frame);
    };
    };
};



/// ANIMATION: Pirate ship balancing
void Animations::PirateShipBalancingLoop() {

    if(nodePirateShip){
    /// Rotations:
    vector3df rot = nodePirateShip->getRotation();
    if(ship_balancing_right == true){
       rot.Z += 0.0015*frameDeltaTime;
       if(rot.Z>=1.0){ rot.Z = 1.0; ship_balancing_right = false; };
    };
    if(ship_balancing_right == false){
       rot.Z -= 0.0015*frameDeltaTime;
       if(rot.Z<=-1.0){ rot.Z = -1.0; ship_balancing_right = true; };
    };
    nodePirateShip->setRotation(rot);
    /// Positions:
    vector3df pos = nodePirateShip->getPosition();
    if(ship_balancing_up == true){
       pos.Y += 0.0045*frameDeltaTime;
       if(pos.Y>=0.0){ pos.Y = 0.0; ship_balancing_up = false; };
    };
    if(ship_balancing_up == false){
       pos.Y -= 0.0045*frameDeltaTime;
       if(pos.Y<=-8.0){ pos.Y = -8.0; ship_balancing_up = true; };
    };
    nodePirateShip->setPosition(pos);
    };
};








void Animations::BlinkSoldierEyesLoop(){

    int doNow = rand()%20;
    if(doNow==10 && !soldiers.empty()){

    /// Reset:
    blinkSoldierEyesDuration = 0.0f;
    /// Random to get one soldier body:
    int selected = rand()%(soldiers.size());

    if(soldiers[selected].Body && soldiers[selected].Body->isInWorld()){
    /// Search node by its body ID:
    int id = soldiers[selected].Body->getUniqueID();
    for(int i=0; i<soldiers.size(); i++) {
    if(id == soldiers[i].Body->getUniqueID()) {
    if(soldiers[i].Node && i<soldiers.size()){
    soldiers[i].Node->setName("nodeSoldier_BLINK");
    for(int m=0; m<soldiers[i].Node->getMaterialCount(); m++){
    /// Set soldier face texture to: Blink eyes
	soldiers[i].Node->getMaterial(m).setTexture(0, driver->getTexture("soldier_01B.jpg"));
    };

    };
    };
    };
    };

    };


    blinkSoldierEyesDuration += 1*frameDeltaTime;

    if(blinkSoldierEyesDuration>100){
    scene::ISceneNode* node = smgr->getSceneNodeFromName("nodeSoldier_BLINK");
    if(node){
    node->setName("");
    /// Apply texture to my specified material
    for(int m=0; m<node->getMaterialCount(); m++){
	/// Set soldier face texture to: Normal
	node->getMaterial(m).setTexture(0, driver->getTexture("soldier_01A.jpg"));
	};
    };
    };
};



void Animations::PrisonersArrestedLoop(){


    int doNow = rand()%300;

    /// 1) EYES ANIMATION:
    if(doNow==10 && !prisoners.empty()){
    /// Reset:
    blinkPrisonerEyesDuration = 0.0f;
    /// Random to get one body:
    int selected = rand()%(prisoners.size());
    if(prisoners[selected].Body && prisoners[selected].Body->isInWorld() && prisoners[selected].Free == false){
    /// Search node by its body ID:
    int id = prisoners[selected].Body->getUniqueID();
    for(int i=0; i<prisoners.size(); i++) {
    if(id == prisoners[i].Body->getUniqueID()) {
    prisoners[i].Node->setName("nodePrisoner_BLINK");
    for(int m=0; m<prisoners[i].Node->getMaterialCount(); m++){
    /// Change texture to: Blink eyes
	prisoners[i].Node->getMaterial(m).setTexture(0, driver->getTexture("prisoner_01B.jpg"));
	};
    };
    };
    };
    };

    blinkPrisonerEyesDuration += 1*frameDeltaTime;
    if(blinkPrisonerEyesDuration>100){
    scene::ISceneNode* node = smgr->getSceneNodeFromName("nodePrisoner_BLINK");
    if(node){
    node->setName("");
    /// Apply texture to my specified material
    for(int m=0; m<node->getMaterialCount(); m++){
	/// Set face texture:
	node->getMaterial(m).setTexture(0, driver->getTexture("prisoner_01A.jpg"));
	};
    };
    };

    /// 2) BODY JUMP:
    if(doNow==20 && !prisoners.empty()){
    /// Random to get one body:
    int selected = rand()%(prisoners.size());
    if(prisoners[selected].Body && prisoners[selected].Body->isInWorld() && prisoners[selected].Free == false){
    BodyJumps(prisoners[selected].Body, 3200);
    };
    };


    /// 3) BALLOON:
    /// if((doTextBalloon==true && !bodyPrisoner.empty()) || (doNow==300 && !bodyPrisoner.empty())){
    if(doNow==30 && !prisoners.empty() && doBalloon == false){
    /// Random to get one body:
    int selected = rand()%(prisoners.size());
    if(prisoners[selected].Body && prisoners[selected].Body->isInWorld() && prisoners[selected].Free == false){
    /// Start text balloon:
    bodyPrisonerForBalloons = prisoners[selected].Body;
    balloonDuration = 0.0;
    doBalloon = true;
    randomPrisonerBalloon = rand()%prisonersArrestedBalloonTextures.size();
	/// Audios:
	vector3df pos = ((IRigidBody*)prisoners[selected].Body)->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)
    audios->PlayTalk(1, pos);
    HitWave2(pos);
    };
    };


    /// 4) NODES (PRISONERS) ALWAYS FACING THE CAMERA: (THE USER)
    for(int i=0; i<prisoners.size(); i++) {
    if(prisoners[i].Node && prisoners[i].Free == false){
    irr::core::vector3df vec = (prisoners[i].Node->getAbsolutePosition() - cameras[0]->getAbsolutePosition()).normalize();
    irr::core::vector3df dir(0,0,1);
    irr::f32 r = acos(dir.dotProduct(vec))*180/3.14;
    if(cameras[0]->getAbsolutePosition().X < prisoners[i].Node->getAbsolutePosition().X){
    prisoners[i].Node->setRotation(irr::core::vector3df(0,+r,0));
    } else {
    prisoners[i].Node->setRotation(irr::core::vector3df(0,-r,0));
    };
    };
    };


};




void Animations::PrisonersEscapingLoop(){

    if(!prisoners.empty()){
    for(int i=0; i<prisoners.size(); i++) {
    if(prisoners[i].Node && prisoners[i].Body && prisoners[i].Body->isInWorld() && prisoners[i].Free == true){


    /// Audio: (Escaping audio loop)
    vector3df pos = prisoners[i].Node->getPosition();
    audios->PlayFx(31, pos);



    /// Audio and attach dust particles:
    if(prisoners[i].AttachParticles==false){
    prisoners[i].AttachParticles = true;

    /// Audio: (Tcham!):
    audios->PlayFx(5, pos);

    /// Create particles - PRISONER - RUNNING DUST:
    scene::IParticleSystemSceneNode* ps;
    scene::IParticleEmitter* em;
    /// Create particle system: (CYLINDER)
    ps = smgr->addParticleSystemSceneNode(true);
    em = ps->createCylinderEmitter(
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &center
    15,   // radius
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &direction
    15,   // length
    true, // outline only
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &normal
    50,   // min
    100,  // max
    SColor(255,255,255,255), // video::SColor &minStartColor
    SColor(255,255,255,255), // video::SColor &maxStartColor
    500,  // lifeTimeMin
    1000, // lifeTimeMax
    0,    // Angle
    core::dimension2df(20.0f, 20.0f), // core::dimension2df &minStartSize
    core::dimension2df(40.0f, 40.0f)  // core::dimension2df &maxStartSize
    );

   	ps->setEmitter(em); // Grabs the emitter
	em->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(SColor(0,150,150,150), 150); // Fade out particles
	ps->addAffector(paf); // Same goes for the affector
	paf->drop();

	ps->setParent(prisoners[i].Node); // Attach to the node
	ps->setPosition(vector3df(-100,-60,0));
	ps->setRotation(core::vector3df(0,0,0));
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("smoke.png"));
	ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	ps->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    };



    /// 1) Animate embed animation:
    f32 fstart = prisoners[i].Node->getStartFrame();
    f32 fend   = prisoners[i].Node->getEndFrame();
    f32 frame  = prisoners[i].Node->getFrameNr();
    /// Next frame calculations:
    frame += 20.0*frameDeltaTime;
    if(frame<=fstart){ frame = fstart; };
    if(frame>=fend  ){ frame = fstart; };
    /// Apply frame:
    prisoners[i].Node->setCurrentFrame((f32)frame);



    /// 2) NODES (SOLDIERS) ALWAYS FACING THE ESCAPING PRISONER:
    /*
    for(int o=0; o<soldiers.size(); o++){
    irr::core::vector3df vec = (soldiers[o].Node->getAbsolutePosition() - prisoners[i].Node->getAbsolutePosition()).normalize();
    irr::core::vector3df dir(0,0,1);
    irr::f32 r = acos(dir.dotProduct(vec))*180/3.14;
    if(prisoners[i].Node->getAbsolutePosition().X < soldiers[o].Node->getAbsolutePosition().X){
    soldiers[o].Node->setRotation(irr::core::vector3df(0,+r,0));
    } else {
    soldiers[o].Node->setRotation(irr::core::vector3df(0,-r,0));
    };
    };
    */



    /// 3) Moving prisoner to front: (Near camera)
    if(prisoners[i].MoveFront==true){

    /// Trans:
    btTransform trans = prisoners[i].Body->getPointer()->getCenterOfMassTransform();
    f32 moveZ = trans.getOrigin().getZ();

    /// Reset all forces:
    prisoners[i].Body->clearForces();

    /// Get Y:
    prisoners[i].VelY = prisoners[i].Body->getLinearVelocity().Y; /// Get any force already applied on Y (jumping force)
    if(prisoners[i].VelY>60){ prisoners[i].VelY=60; }; /// Max jump height allowed

    /// Move Z:
    prisoners[i].VelZ = prisoners[i].Body->getLinearVelocity().Z;
    moveZ -= (prisoners[i].VelZ*frameDeltaTime)/100; /// - (minus) Z == is to go to near camera

    if(moveZ >= -350){
        /// On goal, lets go to next goal...
        moveZ = -350;
        prisoners[i].Body->clearForces();
        // prisoners[i].VelX = 0.0;
        prisoners[i].VelY = 0.0;
        prisoners[i].VelZ = 0.0;
        prisoners[i].MoveFront = false;
        prisoners[i].MoveRight = true;
        // prisoners[i].Body->setLinearFactor(vector3df(0,0,0)); // Allowed positions axis
        // prisoners[i].Body->setAngularFactor(vector3df(0,0,0)); // Allowed positions axis
        /// Wave - Type 2:
        HitWave2(prisoners[i].Node->getPosition());
    };

    // if(moveZ < -350){
    /// Set Positions:
    prisoners[i].Body->setLinearFactor(vector3df(0,1,1)); // Allowed positions axis
    prisoners[i].Body->setLinearVelocity(vector3df(0, prisoners[i].VelY, -prisoners[i].VelZ), ERBTransformSpace::ERBTS_WORLD);
    trans.setOrigin(btVector3(trans.getOrigin().getX(), trans.getOrigin().getY(), moveZ));

    /// Apply:
    prisoners[i].Body->getPointer()->getMotionState()->setWorldTransform(trans); /// Update physics #1
    prisoners[i].Body->getPointer()->setCenterOfMassTransform(trans); /// Update physics #2
    // };

    };



    /// 4) Moving prisoner to right: (to out of screen)
    if(prisoners[i].MoveRight==true){

    /// Trans:
    btTransform trans = prisoners[i].Body->getPointer()->getCenterOfMassTransform();
    f32 moveX = trans.getOrigin().getX();

    /// Reset all forces:
    prisoners[i].Body->clearForces();

    /// Get Y:
    prisoners[i].VelY = prisoners[i].Body->getLinearVelocity().Y; /// Get any force already applied on Y (jumping force)
    if(prisoners[i].VelY>60){ prisoners[i].VelY=60; }; /// Max jump height allowed

    /// Move X;
    moveX += (prisoners[i].VelX*frameDeltaTime)/100;

    if(moveX >= 6150){

        /// On goal, lets go to next goal...
        moveX = 6150;
        prisoners[i].Body->clearForces();
        prisoners[i].VelX = 0.0;
        prisoners[i].VelY = 0.0;
        prisoners[i].VelZ = 0.0;
        /// Prisoner jumps to water:
        prisoners[i].Body->setLinearFactor(vector3df(1,1,1)); // Allowed positions axis
        prisoners[i].Body->setAngularFactor(vector3df(1,1,1)); // Allowed positions axis
        prisoners[i].Body->applyImpulse(core::vector3df(1500,2000,0), core::vector3df(1,1,0));
        prisoners[i].MoveFront = false;
        prisoners[i].MoveRight = false;
        /// Audio:
        audios->PlayFx(33, prisoners[i].Node->getPosition());
        /// Wave - Type 2:
        HitWave2(prisoners[i].Node->getPosition());
        /// Run shark 2:
        shark2_pursuit = true;
        /// Start game over timer:
        game_over_timer = 0.0;

        /// REMOVE:
        /// ALLWAYS search and remove bodies from its own vector array:
        world->removeCollisionObject(world->getCollisionObjectByID(prisoners[i].BodyID)); /// Remove from bullet
        world->removeCollisionObject(prisoners[i].Body); /// Remove from bullet
        prisoners.erase(prisoners.begin()+i);

    };


    if(moveX < 6150){
    /// Set Positions:
    prisoners[i].Body->setLinearFactor(vector3df(1,1,0)); // Allowed positions axis
    prisoners[i].Body->setLinearVelocity(vector3df(prisoners[i].VelX, prisoners[i].VelY, 0), ERBTransformSpace::ERBTS_WORLD);
    trans.setOrigin(btVector3(moveX, trans.getOrigin().getY(), trans.getOrigin().getZ()));

    /// Rotations:
    prisoners[i].Body->setAngularFactor(vector3df(0,0,0));
    prisoners[i].Body->setAngularVelocity(vector3df(0.0,0.0,0.0), ERBTransformSpace::ERBTS_WORLD);
    prisoners[i].Rotation -= 0.0025*frameDeltaTime; if(prisoners[i].Rotation<-1.5){ prisoners[i].Rotation = -1.5; };
    trans.setRotation(btQuaternion(prisoners[i].Rotation,0,0)); // Reset rotation (Bullet order is: Y,X,Z!)

    /// Apply:
    prisoners[i].Body->getPointer()->getMotionState()->setWorldTransform(trans); /// Update physics #1
    prisoners[i].Body->getPointer()->setCenterOfMassTransform(trans); /// Update physics #2
    };

    };


    };
    };
    };


    /// GAME IS OVER:
    game_over_timer += 0.5*frameDeltaTime;
    if(game_over_timer>=3000){ game_over_timer = 3000; };
    if(game_over_timer>=2000 && prisoners.empty() && shark2_pursuit == false){
    GAME_OVER = true;
    };



};









/// Code snippets: http://irrlicht.sourceforge.net/forum/viewtopic.php?t=34385
void Animations::PrisonersArrestedBalloonLooping(){

    /// Text Ballon duration:
    balloonDuration += 1*frameDeltaTime;

    if(balloonDuration>1500){
    doBalloon = false;
    };

    if(doBalloon == true){
    if(bodyPrisonerForBalloons && bodyPrisonerForBalloons->isInWorld()){ /// Body exists? (Could have been removed, if not exists it crashes!)
    /// Convert 3D to 2D positions:
    balloonPosition = bodyPrisonerForBalloons->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)
    position2d<s32> pos = Convert3DTo2DPositions(balloonPosition);
    pos.X -= 32; // half the image length
    pos.Y -= 96; // above the enemy
    /// DRAW "energy bar" image background:
    if(prisonersArrestedBalloonTextures[randomPrisonerBalloon]){ driver->draw2DImage(prisonersArrestedBalloonTextures[randomPrisonerBalloon], pos, core::rect<s32>(0,0,64,64), 0, video::SColor(255,255,255,255), true); };
    };
    };
};




/// Code snippets: http://irrlicht.sourceforge.net/forum/viewtopic.php?t=34385
void Animations::PrisonersEscapingBalloonLooping(){

    if(escaping_balloon_timer>=1000){
    escaping_random_ballon = rand()%(prisonersEscapingBalloonTextures.size());
    escaping_balloon_timer = 0.0;
    };

    // if(escaping_balloon_timer==0){ audios->PlayTalk(2, cameras[0]->getPosition()); };

    for(int i=0; i<prisoners.size() && escaping_balloon_timer>=500; i++){
    if(prisoners[i].Body && prisoners[i].Body->isInWorld() && prisoners[i].Free == true){ /// Body exists? (Could have been removed, if not exists it crashes!)
    /// Convert 3D to 2D positions:
    balloonPosition = prisoners[i].Body->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)
    position2d<s32> pos = Convert3DTo2DPositions(balloonPosition);
    pos.X -= 32; // half the image length
    pos.Y -= 96; // above the enemy
    /// DRAW balloon image background:
    if(prisonersEscapingBalloonTextures[escaping_random_ballon]){ driver->draw2DImage(prisonersEscapingBalloonTextures[escaping_random_ballon], pos, core::rect<s32>(0,0,64,64), 0, video::SColor(255,255,255,255), true); };
    // audios->PlayTalk(2, balloonPosition);
    };
    };

    escaping_balloon_timer+=0.25*frameDeltaTime;

};




void Animations::GameOverLoop(){

    if(GAME_OVER==true && LEVEL_READY==true) {

    /// GAME OVER events:
    if(game_over_once==true){
    game_over_once = false;
    /// Stop Music:
    musiciansAnimate = false;
    audios->StopMusic(1); /// Music: Main Theme
    audios->StopFx(30); /// Fx: Shark Suspense
    /// King yell:
    audios->PlayTalk(3, cameras[0]->getPosition());
    /// Play music:
    audios->PlayFx(40, cameras[0]->getPosition());
    /// Everyone Jumps:
    // level->EveryoneJumps();
    /// Stop play!...
    textInfos1->setVisible(false);
    /// textTotalScore->setVisible(false);
    textAngle->setVisible(false);
    if(NAMES_TEXTS==true){ for(int i=0; i<personageName.size(); i++){ personageName[i]->setVisible(false); }; };
    };

    /// King GAME OVER balloons:
    if(kings[0].Body && kings[0].Body->isInWorld()){ /// Body exists? (Could have been removed, if not exists it crashes!)
    /// Convert 3D to 2D positions:
    vector3df posvector = kings[0].Body->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)
    position2d<s32> pos = Convert3DTo2DPositions(posvector);
    pos.X -= 256; // half the image length
    pos.Y -= 240; // above the character
    /// DRAW balloon:
    if(kingGameOverBalloonTextures[0]){ driver->draw2DImage(kingGameOverBalloonTextures[0], pos, core::rect<s32>(0,0,512,210), 0, video::SColor(255,255,255,255), true); };
    };

    };

};




/// Code snippets: http://irrlicht.sourceforge.net/forum/viewtopic.php?t=34385
void Animations::KingLooping(){


    /// 1) Node animation:
    for(int i=0; i<kings.size(); i++) {
    if(kings[i].Node){
    /// 1) Animate embed animation:
    f32 fstart = kings[i].Node->getStartFrame();
    f32 fend   = kings[i].Node->getEndFrame();
    f32 frame  = kings[i].Node->getFrameNr();
    /// Next frame calculations:
    frame += kings[i].VelX*frameDeltaTime;
    if(frame<=fstart){ frame = fstart; };
    if(frame>=fend  ){ frame = fstart; };
    /// Apply frame:
    kings[i].Node->setCurrentFrame((f32)frame);
    };
    };

    if(GAME_OVER==false){
    /// 2) King ORDER Balloon duration:
    /// Timer:
    kingOrderBalloonDuration += 1.0*frameDeltaTime;
    /// Hide:
    if(kingOrderBalloonDuration>2500){ kingOrder = ""; };
    /// Show:
    if(kingOrder.size()>0){
    if(kings[0].Body && kings[0].Body->isInWorld()){ /// Body exists? (Could have been removed, if not exists it crashes!)
    /// Convert 3D to 2D positions:
    vector3df posvector = kings[0].Body->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)
    position2d<s32> pos = Convert3DTo2DPositions(posvector);
    pos.X -=  32; // half the image length
    pos.Y -= 110; // above the character
    /// Wave - Type 2:
    if(kingOrderWave2){ HitWave2(posvector); kingOrderWave2 = false; };
    /// DRAW balloon:
    if(kingOrder=="silence"){ if(kingOrdersBalloonTextures[0]){ driver->draw2DImage(kingOrdersBalloonTextures[0], pos, core::rect<s32>(0,0,64,64), 0, video::SColor(255,255,255,255), true); }; };
    if(kingOrder=="music"  ){ if(kingOrdersBalloonTextures[1]){ driver->draw2DImage(kingOrdersBalloonTextures[1], pos, core::rect<s32>(0,0,64,64), 0, video::SColor(255,255,255,255), true); }; };
    if(kingOrder=="show"   ){ if(kingOrdersBalloonTextures[2]){ driver->draw2DImage(kingOrdersBalloonTextures[2], pos, core::rect<s32>(0,0,64,64), 0, video::SColor(255,255,255,255), true); }; };
    if(kingOrder=="hide"   ){ if(kingOrdersBalloonTextures[3]){ driver->draw2DImage(kingOrdersBalloonTextures[3], pos, core::rect<s32>(0,0,64,64), 0, video::SColor(255,255,255,255), true); }; };
    };
    };
    };


    if(GAME_OVER==false){
    /// 3) King RANDOM Balloon duration:
    /// Random:
    if(kingRandomBalloonDuration==0.0){
    king_random_balloon = rand()%(kingRandomBalloonTextures.size());
    king_random_balloon_next = rand()%50000+10000;
    king_random_audio = true;
    };
    /// Show:
    if(kingOrder.size()<=0 && kingRandomBalloonDuration>king_random_balloon_next){
    if(kings[0].Body && kings[0].Body->isInWorld()){ /// Body exists? (Could have been removed, if not exists it crashes!)
    /// a) Convert 3D to 2D positions:
    vector3df posvector = kings[0].Body->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)
    position2d<s32> pos = Convert3DTo2DPositions(posvector);
    pos.X -=  32; // half the image length
    pos.Y -= 110; // above the character
    /// b) DRAW balloon:
    if(kingRandomBalloonTextures[king_random_balloon]){ driver->draw2DImage(kingRandomBalloonTextures[king_random_balloon], pos, core::rect<s32>(0,0,64,64), 0, video::SColor(255,255,255,255), true); };
    if(king_random_audio){ audios->PlayTalk(4, posvector); HitWave2(posvector); king_random_audio = false;};
    };
    };
    /// Timer:
    kingRandomBalloonDuration += 1.0*frameDeltaTime;
    /// Hide:
    if(kingRandomBalloonDuration>=(king_random_balloon_next+2000)){
    kingRandomBalloonDuration = 0.0;
    };
    };


    /// NODES (KING) ALWAYS FACING THE CAMERA: (THE USER)
    for(int i=0; i<kings.size(); i++) {
    if(kings[i].Node){
    irr::core::vector3df vec = (kings[i].Node->getAbsolutePosition() - cameras[0]->getAbsolutePosition()).normalize();
    irr::core::vector3df dir(0,0,1);
    irr::f32 r = acos(dir.dotProduct(vec))*180/3.14;
    if(cameras[0]->getAbsolutePosition().X < kings[i].Node->getAbsolutePosition().X){
    kings[i].Node->setRotation(irr::core::vector3df(0,+r,0));
    } else {
    kings[i].Node->setRotation(irr::core::vector3df(0,-r,0));
    };
    };
    };
};




void Animations::PriestsLoop(){

    for(int i=0; i<priests.size(); i++) {
    if(priests[i].Node && priests[i].Body && priests[i].Body->isInWorld()){

    /// Get speed for both animations:
    f32 speed = priests[i].Speed;

    /// Animate embed animation:
    /// Set animation frames limits:
    f32 fstart = priests[i].Node->getStartFrame();
    f32 fend   = priests[i].Node->getEndFrame();
    f32 frame  = priests[i].Node->getFrameNr();
    /// Next frame calculations:
    frame += speed*frameDeltaTime;
    if(frame<=fstart){ frame = fstart; };
    if(frame>=fend  ){ frame = fstart; };
    /// Apply frame:
    priests[i].Node->setCurrentFrame((f32)frame);

    /// Change texture to normal:
    scaredPriestDuration += 0.5*frameDeltaTime;
    if(scaredPriestDuration>=13000.0){
    for(int m=0; m<priests[i].Node->getMaterialCount(); m++){
	priests[i].Node->getMaterial(m).setTexture(0, driver->getTexture("priest_01A.jpg"));
	};
    };

    };
    };
};




void Animations::PriestsWalkingLoop(){

    for(int i=0; i<priestswalking.size(); i++) {
    if(priestswalking[i].Node && priestswalking[i].Body && priestswalking[i].Body->isInWorld()){

    /// 1) Animate embed animation:
    f32 fstart = priestswalking[i].Node->getStartFrame();
    f32 fend   = priestswalking[i].Node->getEndFrame();
    f32 frame  = priestswalking[i].Node->getFrameNr();
    /// Next frame calculations:
    frame += priestswalking[i].VelX*frameDeltaTime;
    if(frame<=fstart){ frame = fstart; };
    if(frame>=fend  ){ frame = fstart; };
    /// Apply frame:
    priestswalking[i].Node->setCurrentFrame((f32)frame);



    /// Reset all forces:
    priestswalking[i].Body->clearForces();
    /// Transform:
    btTransform trans = priestswalking[i].Body->getPointer()->getCenterOfMassTransform();
    priestswalking[i].PosY = trans.getOrigin().getY();
    priestswalking[i].VelY = priestswalking[i].Body->getLinearVelocity().Y; /// Get any force already applied on Y (jumping force)
    if(priestswalking[i].VelY>60){ priestswalking[i].VelY=60; }; /// Max jump height allowed


    /// Randomly change direction:
    int randChangeDirection = rand()%500;
    if(randChangeDirection==100){ if(priestswalking[i].MoveRight==true){ priestswalking[i].MoveRight=false; } else { priestswalking[i].MoveRight=true; }; };

    /// Moving priestswalking to one direction, according animation...
    /// Get Positions:
    f32 moveX = trans.getOrigin().getX();
    if(priestswalking[i].MoveRight==true ){ moveX += (priestswalking[i].VelX*frameDeltaTime)/100; };
    if(priestswalking[i].MoveRight==false){ moveX -= (priestswalking[i].VelX*frameDeltaTime)/100; };
    if(moveX >= priestswalking[i].MaxPosX){ moveX = priestswalking[i].MaxPosX; priestswalking[i].MoveRight = false; };
    if(moveX <= priestswalking[i].MinPosX){ moveX = priestswalking[i].MinPosX; priestswalking[i].MoveRight = true;  };
    /// Set Positions:
    priestswalking[i].Body->setLinearFactor(vector3df(1,1,0)); // Allowed positions axis
    if(priestswalking[i].MoveRight==true ){ priestswalking[i].Body->setLinearVelocity(vector3df( priestswalking[i].VelX, priestswalking[i].VelY, 0.0f), ERBTransformSpace::ERBTS_WORLD); };
    if(priestswalking[i].MoveRight==false){ priestswalking[i].Body->setLinearVelocity(vector3df(-priestswalking[i].VelX, priestswalking[i].VelY, 0.0f), ERBTransformSpace::ERBTS_WORLD); };
    trans.setOrigin(btVector3(moveX, priestswalking[i].PosY, trans.getOrigin().getZ()));
    /// Set Rotations:
    priestswalking[i].Body->setAngularFactor(vector3df(0,0,0));
    priestswalking[i].Body->setAngularVelocity(vector3df(0,0,0), ERBTransformSpace::ERBTS_WORLD);
    if(priestswalking[i].MoveRight==true ){ priestswalking[i].Rotation -= 0.0045*frameDeltaTime; if(priestswalking[i].Rotation<-1.5){ priestswalking[i].Rotation = -1.5; }; };
    if(priestswalking[i].MoveRight==false){ priestswalking[i].Rotation += 0.0045*frameDeltaTime; if(priestswalking[i].Rotation> 1.5){ priestswalking[i].Rotation =  1.5; }; };
    trans.setRotation(btQuaternion(priestswalking[i].Rotation,0,0)); // Reset rotation (Bullet order is: Y,X,Z!)

    /// Apply:
    priestswalking[i].Body->getPointer()->getMotionState()->setWorldTransform(trans); /// Update physics #1
    priestswalking[i].Body->getPointer()->setCenterOfMassTransform(trans); /// Update physics #2


    };
    };
};




void Animations::PriestsScared(){

    if(scaredPriestDuration>=1000.0){ /// Just to avoid too short repetitions
    scaredPriestDuration = 0.0;

    for(int i=0; i<priests.size(); i++) {
    if(priests[i].Node){
    /// Change texture to scared:
    for(int m=0; m<priests[i].Node->getMaterialCount(); m++){
	priests[i].Node->getMaterial(m).setTexture(0, driver->getTexture("priest_01B.jpg"));
	};
    };
    };

    };
};




void Animations::BodyJumps(IRigidBody* body, f32 force){

    /// Body exists?
    if(body && body->isInWorld()){
    /// Reset all forces:
    body->clearForces();
    /// Apply jumping force:
    body->applyImpulse(core::vector3df(0,force,0), core::vector3df(0,1,0));
    };
};



/// ANIMATION: Soldiers walking + random balloons:
void Animations::SoldiersLoop() {




    /// Animate its node:
    for(int i=0; i<soldiers.size(); i++){
    if(soldiers[i].Node){
    /// Set animation frames limits:
    f32 fstart = soldiers[i].Node->getStartFrame();
    f32 fend   = soldiers[i].Node->getEndFrame();
    f32 frame  = soldiers[i].Node->getFrameNr();
    f32 speed  = soldiers[i].VelX/1.5;
    frame += speed*frameDeltaTime;
    /// Next frame calculations:
    if(frame<=fstart){ frame = fstart; };
    if(frame>=fend  ){ frame = fstart; };
    /// Apply frame:
    soldiers[i].Node->setCurrentFrame((f32)frame);

    };
    };



    /// Soldiers CLEAR RANDOM Balloon duration:
    /// Random:
    if(soldiersClearRandomBalloonDuration==0.0){
    soldiersClear_random_balloon = rand()%(soldiersClearRandomBalloonTextures.size());
    soldiersClear_random_balloon_next = rand()%20000+2000; // rand()%25000+5000;
    soldiersClear_random_balloon_body = rand()%(soldiers.size());
    soldiersClear_random_audio = true;
    };
    /// Show:
    if(soldiersClearRandomBalloonDuration>soldiersClear_random_balloon_next){
    if(soldiers[soldiersClear_random_balloon_body].Body && soldiers[soldiersClear_random_balloon_body].Body->isInWorld()){ /// Body exists? (Could have been removed, if not exists it crashes!)
    /// a) Convert 3D to 2D positions:
    vector3df posvector = soldiers[soldiersClear_random_balloon_body].Body->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)
    position2d<s32> pos = Convert3DTo2DPositions(posvector);
    pos.X -=  32; // half the image length
    pos.Y -= 110; // above the character
    /// b) DRAW balloon:
    if(soldiersClearRandomBalloonTextures[soldiersClear_random_balloon]){ driver->draw2DImage(soldiersClearRandomBalloonTextures[soldiersClear_random_balloon], pos, core::rect<s32>(0,0,64,64), 0, video::SColor(255,255,255,255), true); };
    if(soldiersClear_random_audio){ audios->PlayTalk(6, posvector); HitWave2(posvector); soldiersClear_random_audio = false;};
    };
    };
    /// Timer:
    soldiersClearRandomBalloonDuration += 1.0*frameDeltaTime;
    /// Hide:
    if(soldiersClearRandomBalloonDuration>=(soldiersClear_random_balloon_next+2000)){
    soldiersClearRandomBalloonDuration = 0.0;
    };





    /// Walking Limits:
    bool addStairsValue = false;

    for(int i=0; i<soldiers.size(); i++){
    if(soldiers[i].Body && soldiers[i].Body->isInWorld()){ /// Body exists? (Could have been removed, if not exists it crashes!)
    /// Reset all forces:
    soldiers[i].Body->clearForces();
    /// Transform:
    btTransform trans = soldiers[i].Body->getPointer()->getCenterOfMassTransform();
    soldiers[i].PosX = trans.getOrigin().getX();
    soldiers[i].PosY = trans.getOrigin().getY();
    soldiers[i].RotY = trans.getRotation().getY();
    soldiers[i].VelY = soldiers[i].Body->getLinearVelocity().Y; /// Get any force already applied on Y (jumping force)
    if(soldiers[i].VelY>60){soldiers[i].VelY=60;}; /// Max jump height allowed
    };
    };


    /// Randomly change direction:
    for(int i=0; i<soldiers.size(); i++){ /// i = One soldier
    int randChangeDirection = rand()%500;
    if(randChangeDirection==100){ if(soldiers[i].MoveRight==true){ soldiers[i].MoveRight=false; } else { soldiers[i].MoveRight=true; }; };
    };

    /// Distance between: Soldier - Vs - Soldier:
    for(int i=0; i<soldiers.size(); i++){ /// i = One soldier
    for(int o=0; o<soldiers.size(); o++){ /// o = Other soldier
    if(o!=i){ /// If its not the same soldier... Then compare...
        int distanceX = abs(soldiers[i].PosX-soldiers[o].PosX);
        int distanceY = abs(soldiers[i].PosY-soldiers[o].PosY);
        if(distanceX<100 && distanceY<100){ /// Warning: Collision between these 2 bodies (soldiers) is disabled in its shape (else they could be stuck together)
        if(soldiers[i].MoveRight==true ){ soldiers[i].MoveRight = false; soldiers[o].MoveRight = true;  }; /// Invert booleans for both
        if(soldiers[i].MoveRight==false){ soldiers[i].MoveRight = true;  soldiers[o].MoveRight = false; }; /// Invert booleans for both
        };
        if(soldiers[i].PosX<soldiers[i].MinPosX){ soldiers[i].PosX = soldiers[i].MinPosX; soldiers[i].MoveRight = true;  addStairsValue = true; };
        if(soldiers[i].PosX>soldiers[i].MaxPosX){ soldiers[i].PosX = soldiers[i].MaxPosX; soldiers[i].MoveRight = false; };
    };
    };
    };




    /*
    /// Distance between: Soldier - Vs - Boxes:
    int bigger1 = bodyBox.size();
    int bigger2 = bodySoldier.size();
    if(bigger1>bigger2){ max1 = bigger1; max2 = bigger2; };
    if(bigger1<bigger2){ max1 = bigger2; max2 = bigger1; };

    for(int i1=0; i1<bigger1;  i1++){ /// o = Box
    for(int i2=0; i2<bigger2); i2++){ /// i = Soldier
        if(bodyBox[i1]){              /// Body exists? (Could have been removed, if not exists it crashes!)
        if(bodyBox[i1]->isInWorld()){ /// Body exists? (Could have been removed, if not exists it crashes!)
        btTransform trans = bodyBox[i1]->getPointer()->getCenterOfMassTransform();
        f32 posBX = trans.getOrigin().getX();
        f32 posBY = trans.getOrigin().getY();
        int distanceX = abs(posX[i2]-posBX);
        int distanceY = abs(posY[i2]-posBY);
        if(distanceX<200 && distanceY<30){
        if(move_to_right[i2]==true ){ move_to_right[i2] = false; }; /// Invert booleans
        if(move_to_right[i2]==false){ move_to_right[i2] = true;  }; /// Invert booleans
        };
        if(posX[i2]<minPosX[i2]){ posX[i2] = minPosX[i2]; move_to_right[i2] = true;  };
        if(posX[i2]>maxPosX[i2]){ posX[i2] = maxPosX[i2]; move_to_right[i2] = false; };
        };
        };
    };
    };
    */


    /*
    int m =0;

    if(addStairsValue){
    int m = move_to_stairs[0];
    m+=1;
    move_to_stairs[0] = m;
    cout <<m << endl;
    };

    if(m>50){
    /// Reset all forces:
    bodySoldier[0]->clearForces();
    bodySoldier[0]->getPointer()->setGravity(btVector3(0,0,0));
    nodeSoldier[0]->setPosition(vector3df(0,-300,0));
    btTransform trans = bodySoldier[0]->getPointer()->getCenterOfMassTransform();
    /// Positions:
    velY[0] = -1;
    bodySoldier[0]->setLinearFactor(vector3df(0,0,0)); // Allowed positions axis
    bodySoldier[0]->setLinearVelocity(vector3df(0, 0, 0), ERBTransformSpace::ERBTS_LOCAL);
    trans.setOrigin(btVector3(0, -300, 0));
   /// Rotations:
    bodySoldier[0]->setAngularFactor(vector3df(0,0,0));
    bodySoldier[0]->setAngularVelocity(vector3df(0.0,0.0,0.0), ERBTransformSpace::ERBTS_LOCAL);
    trans.setRotation(btQuaternion(0,0,0)); // Reset rotation (Bullet order is: Y,X,Z!)
    /// Apply:
    bodySoldier[0]->getMotionState()->setWorldTransform(trans);


    move_to_stairs[0] = 0;
    };
    */


    for(int i=0; i<soldiers.size(); i++){
    if(soldiers[i].Body && soldiers[i].Body->isInWorld()){ /// Body exists? (Could have been removed, if not exists it crashes!)

    /// Moving soldier to right:
    if(soldiers[i].MoveRight==true){
    btTransform trans = soldiers[i].Body->getPointer()->getCenterOfMassTransform();
    /// Positions:
    soldiers[i].Body->setLinearFactor(vector3df(1,1,0)); // Allowed positions axis
    soldiers[i].Body->setLinearVelocity(vector3df(soldiers[i].VelX, soldiers[i].VelY, 0.0f), ERBTransformSpace::ERBTS_WORLD);
    trans.setOrigin(btVector3(soldiers[i].PosX, soldiers[i].PosY, trans.getOrigin().getZ()));
    /// Rotations:
    soldiers[i].Body->setAngularFactor(vector3df(0,0,0));
    soldiers[i].Body->setAngularVelocity(vector3df(0.0,0.0,0.0), ERBTransformSpace::ERBTS_WORLD);
    soldiers[i].Rotation -= 0.006*frameDeltaTime; if(soldiers[i].Rotation<-1.1){ soldiers[i].Rotation = -1.1; };
    trans.setRotation(btQuaternion(soldiers[i].Rotation,0,0)); // Reset rotation (Bullet order is: Y,X,Z!)
    /// Apply:
    soldiers[i].Body->getPointer()->getMotionState()->setWorldTransform(trans); /// Update physics #1
    soldiers[i].Body->getPointer()->setCenterOfMassTransform(trans); /// Update physics #2
    };

    /// Moving soldier to left:
    if(soldiers[i].MoveRight==false){
    btTransform trans = soldiers[i].Body->getPointer()->getCenterOfMassTransform();
    /// Positions:
    soldiers[i].Body->setLinearFactor(vector3df(1,1,0));  // Allowed positions axis
    soldiers[i].Body->setLinearVelocity(vector3df(-soldiers[i].VelX, soldiers[i].VelY, 0.0f), ERBTransformSpace::ERBTS_WORLD);
    trans.setOrigin(btVector3(soldiers[i].PosX, soldiers[i].PosY, trans.getOrigin().getZ()));
    /// Rotations:
    soldiers[i].Body->setAngularFactor(vector3df(0,0,0));
    soldiers[i].Body->setAngularVelocity(vector3df(0.0,0.0,0.0), ERBTransformSpace::ERBTS_WORLD);
    soldiers[i].Rotation += 0.006*frameDeltaTime; if(soldiers[i].Rotation>1.1){ soldiers[i].Rotation = 1.1; };
    trans.setRotation(btQuaternion(soldiers[i].Rotation,0,0)); // Reset rotation (Bullet order is: Y,X,Z!)
    /// Apply:
    soldiers[i].Body->getPointer()->getMotionState()->setWorldTransform(trans); /// Update physics #1
    soldiers[i].Body->getPointer()->setCenterOfMassTransform(trans); /// Update physics #2
    };



    /// UPDATES all previous transforms (by nodes animations, etc) and updates coordinates relative to the physic world:
    // btVector3 aabbmin0, aabbmax0;
    // bodySoldier[i]->getPointer()->getCollisionShape()->getAabb(bodySoldier[i]->getPointer()->getWorldTransform(), aabbmin0, aabbmax0);  // extracts the object's min/max axis aligned bounding box world coordinates
    // world->getPointer()->getBroadphase()->setAabb(bodySoldier[i]->getPointer()->getBroadphaseHandle(), aabbmin0, aabbmax0, 0); // sets the proxy's aabb and updates the tree

    };
    };


};



/// Hit Waves - Type 2: (Big ones)
void Animations::HitWave2(vector3df pos){

    hitDoWave2Loop = true;
    hitWave2TextureTimer = 0.0;
    hitWave2TextNum = 0;
    hitWave2Position = pos; /// Get body position (in irrBullet)

};

void Animations::HitWave2Loop(){
    if(hitDoWave2Loop){
    hitWave2TextureTimer+=1.50*frameDeltaTime;
    if(hitWave2TextureTimer>=20){ /// Duration between textures (20 is perfect)
    /// Change texture;
    if(hitWave2TextNum<hitWaves2Textures.size()){
    /// Convert 3D to 2D positions:
    position2d<s32> pos = Convert3DTo2DPositions(hitWave2Position);
    pos.X -= 128/2; // half the image width
    pos.Y -= 128/2; // half the image height
    /// DRAW "energy bar" image background:
    if(hitWaves2Textures[hitWave2TextNum]){ driver->draw2DImage(hitWaves2Textures[hitWave2TextNum], pos, core::rect<s32>(0,0,128,128), 0, video::SColor(255,255,255,255), true); };
    };
    hitWave2TextureTimer = 0.0;
    hitWave2TextNum+=1;
    };
    if(hitWave2TextNum>=hitWaves2Textures.size()){
    hitWave2TextNum = 0;
    hitDoWave2Loop = false;
    };
    };
};



/// Hit Waves - Type 3: (Very small ones)
void Animations::HitWave3(vector3df pos){

    hitDoWave3Loop = true;
    hitWave3TextureTimer = 0.0;
    hitWave3TextNum = 0;
    hitWave3Position = pos; /// Get body position (in irrBullet)

};

void Animations::HitWave3Loop(){
    if(hitDoWave3Loop){
    hitWave3TextureTimer+=1.50*frameDeltaTime;
    if(hitWave3TextureTimer>=20){ /// Duration between textures (20 is perfect)
    /// Change texture;
    if(hitWave3TextNum<hitWaves3Textures.size()){
    /// Convert 3D to 2D positions:
    position2d<s32> pos = Convert3DTo2DPositions(hitWave3Position);
    pos.X -= 128/2; // half the image width
    pos.Y -= 128/2; // half the image height
    /// DRAW "energy bar" image background:
    if(hitWaves3Textures[hitWave3TextNum]){ driver->draw2DImage(hitWaves3Textures[hitWave3TextNum], pos, core::rect<s32>(0,0,128,128), 0, video::SColor(255,255,255,255), true); };
    };
    hitWave3TextureTimer = 0.0;
    hitWave3TextNum+=1;
    };
    if(hitWave3TextNum>=hitWaves3Textures.size()){
    hitWave3TextNum = 0;
    hitDoWave3Loop = false;
    };
    };
};



/// TOOL - Convert 3D to 2D positions:
/// http://irrlicht.sourceforge.net/forum/viewtopic.php?t=93
position2d<s32> Animations::Convert3DTo2DPositions(vector3df pos3D) {

    s32 sw = driver->getScreenSize().Width;
    s32 sh = driver->getScreenSize().Height;
    sw/=2;
    sh/=2;

    f32 transformedPos[4];
    core::matrix4 trans;
    trans*=smgr->getActiveCamera()->getProjectionMatrix();
    trans*=smgr->getActiveCamera()->getViewMatrix();
    transformedPos[0]=pos3D.X;
    transformedPos[1]=pos3D.Y;
    transformedPos[2]=pos3D.Z;
    transformedPos[3]=1.0f;
    trans.multiplyWith1x4Matrix(transformedPos);
    f32 zDiv = transformedPos[3] == 0.0f ? 1.0f : (1.0f/transformedPos[3]);

    s32 screenx = (s32)(sw*transformedPos[0]*zDiv)+sw;
    s32 screeny = ((s32)(sh-(sh*(transformedPos[1]*zDiv))));

    return position2d<s32>(screenx, screeny);
};


void Animations::Loop(){

    /// Constant and Random audios:
    AudiosLoop();

    /// Sky:
    SkyAnimatedLoop();

    /// Shark 1:
    Shark1MoveLeftRightLoop();

    /// Shark 2 - Pursuit Prisoners:
    Shark2MoveRightLoop();

    /// Platform:
    PlatformLoop();

    /// Workers:
    WorkersLoop();

    /// Musicians:
    MusiciansLoop();

    /// Bulls:
    BullsLoop();

    /// Priest Drinking and Random Balloons:
    /// PriestsDrunkLoop(); // <-- Loop called in "game-level.cpp" ( 2D draws must be after smgr->drawAll() )

    /// Knights:
    KnightMoveLoop();

    /// Bird:
    BirdMoveLeftRightLoop();

    /// Chicken:
    ChickenMoveLeftRightLoop();

    /// Flags:
    FlagsMovingLoop();

    /// Pirate Ship:
    PirateShipBalancingLoop();

    /// Soldiers Blink Eyes:
    BlinkSoldierEyesLoop();

    /// Priests:
    PriestsLoop();

    /// Priests Walking Loop:
    PriestsWalkingLoop();

    /// Prisoners Blink Eyes:
    PrisonersArrestedLoop();

    /// Prisoners Free:
    PrisonersEscapingLoop();

    /// Hit wave 2: (Big ones)
    /// HitWave2Loop(); // <-- Loop called in "game-level.cpp" ( 2D draws must be after smgr->drawAll() )

    /// Hit wave 3: (Very small ones)
    /// HitWave3Loop(); // <-- Loop called in "game-level.cpp" ( 2D draws must be after smgr->drawAll() )

    /// Prisoners Arrested Balloons:
    /// PrisonersArrestedBalloonLooping(); // <-- Loop called in "game-level.cpp" ( 2D draws must be after smgr->drawAll() )

    /// Prisoners Escaping Balloons:
    /// PrisonersEscapingBalloonLooping(); // <-- Loop called in "game-level.cpp" ( 2D draws must be after smgr->drawAll() )

    /// King Balloons:
    /// KingLooping(); // <-- Loop called in "game-level.cpp" ( 2D draws must be after smgr->drawAll() )

    /// GAME OVER:
    /// GameOverLoop(); // <-- Loop called in "game-level.cpp" ( 2D draws must be after smgr->drawAll() )

    /// Soldiers Walking and Random Balloons:
    /// SoldiersLoop(); // <-- Loop called in "game-level.cpp" ( 2D draws must be after smgr->drawAll() )

};


