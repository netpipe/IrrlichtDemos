
/// All the other classes:
#include "game-scene.cpp"
#include "game-effects.cpp"
#include "game-audios.cpp"
#include "game-animations.cpp"



/// STRUCT object and Vars for BALLS: (Here, since balls are created in this class)
struct Ball {
IAnimatedMeshSceneNode* Node = 0;
IRigidBody* Body = 0;
int BodyID = 0;
f32 LifeTime = 0.0;
};
std::vector<Ball> balls;
int ballToFollowID = 0; /// The ball that camera will follow
bool cameraGoToOrigin = true;


/// Globals:
f32 cannon_power = 0;
bool audios_already_initialized = false;


/// LEVEL:
class Level : public Game {

    public:

    Level();  // Constructor
    ~Level(); // Destructor

    void ResetAll(); /// TO-DO (Not functional yet!)
    void ResetMap();
    void Initialize();
    void Loop();

    void RemoveBalls();
    void CreateBall(const f32 cannon_power);
    void CheckCollisions();

    void HitBodyJump(IRigidBody* body, f32 force);
    void EveryoneJumps();
    void BodyRemove(IRigidBody* body, std::string why);
    void ReverseImpulse(IRigidBody* body);

    // void walkMovement(IRigidBody* body0, IRigidBody* body1);
    void LightSetPosition();
    void CameraSetTargetAndPosition();
    void CameraShake();

    void Draw2DIntro();
    void Draw2DView();
    void Draw2DOver();
    void Draw2DPowerBar();
    void Draw2DPrisonersBar();
    void DrawLines();
    void DrawTrajectoryLines();

    /// Declaring all mine other classes:
    Scene* scene = new Scene();
    Effects* effects = new Effects();
    Animations* animations = new Animations();
    Audios* audios = new Audios();

    private:

    /// Space key is pressed:
    bool canDoAudio = true;
    bool cameraIsNearShip = true;
    bool userIsMovingCamera = false;
    f32 userCameraSpeedX = 0.0;
    f32 userCameraSpeedY = 0.0;
    bool userGoingLeft = false;
    bool userGoingUp   = false;

    /// Camera Shake:
    f32 shakeCamera = 1000.0;
    f32 cameraShakingTime = 0.0;

    /// Power Bar:
    ITexture* powerBar_Frame;
    ITexture* powerBar_Force;
    ITexture* powerBar_Arrow;
    ITexture* powerBar_Ready;
    ITexture* powerBar_Reloading;
    ITexture* powerBar_Rotation;
    int old_bar_percentage = 0; // (100*cannon_power)/(600-200)-50;
    int new_bar_percentage = 0; // (100*cannon_power)/(600-200)-50;
    f32 arrow2DTimer = 0.0;

    /// Prisoners Bar:
    ITexture* prisonersBar_Back;
    ITexture* prisonersBar_Free;

    /// Game Intro:
    ITexture* game_Intro;
    /// Game View:
    ITexture* game_View;
    /// Game Over:
    ITexture* game_Over;

    /// Ball:
    f32 timer_ballInScene = 0.0;

    /// Just to void too many and continuous hits on bell:
    f32 canHitBellAgainTimer = 1000;
    /// Just to void too many and continuous hits on targets:
    f32 canHitTargetAgainTimer = 1000;

};

Level::Level(){};  // Constructor (Can accept arguments, but this one does not)
Level::~Level(){}; // Destructor (do not accept arguments)


/// TO-DO (Not functional yet!)
void Level::ResetAll(){

    /// Reset 2D textures:
    powerBar_Frame     = 0;
    powerBar_Force     = 0;
    powerBar_Arrow     = 0;
    powerBar_Ready     = 0;
    powerBar_Reloading = 0;
    powerBar_Rotation  = 0;

    /// Reset 2D textures:
    prisonersBar_Back = 0;
    prisonersBar_Free = 0;

    /// Reset 2D textures:
    game_Intro = 0;
    game_View  = 0;
    game_Over  = 0;

    /// Reset:
    PLAYER_TOTAL_SCORE = 0;
    PLAYER_LEVEL       = 0;
    PLAYER_NAME        = L"";

    old_bar_percentage = 0; // (100*cannon_power)/(600-200)-50;
    new_bar_percentage = 0; // (100*cannon_power)/(600-200)-50;

    /// Balls:
    cameraGoToOrigin = true;
    ballToFollowID = 0;

};



void Level::ResetMap(){

    /// Reset:
    PLAYER_TOTAL_SCORE = 0;
    PLAYER_LEVEL       = 0;
    PLAYER_NAME        = L"";

    old_bar_percentage = 0; // (100*cannon_power)/(600-200)-50;
    new_bar_percentage = 0; // (100*cannon_power)/(600-200)-50;

    /// To update the total score texts:
    effects->TotalScoreAdd(0);

    /// Balls:
    cameraGoToOrigin = true;
    ballToFollowID = 0;

    /// RESET Cannon Rotation to default:
    vector3df cannonRot = nodeCannon->getRotation(); cannonRot.Z = 0;
    nodeCannon->setRotation(vector3df(0,0,cannonRot.Z));

    /// Set to their defaults:
    textInfos1->setVisible(false);
    textTotalScore->setVisible(false);
    textAngle->setVisible(false);


    /// REMOVE these from physics:
    RemoveBalls();
    scene->RemoveBoxes();
    scene->RemoveCargos();
    scene->RemoveJails();
    scene->RemovePrisoners();

    /// CREATE these for physics:
    scene->CreateBoxes();
    scene->CreateJails();
    scene->CreatePrisoners();

};



void Level::Initialize(){

    /// Reset:
    ResetAll();

    /// Fill 2D textures:
    powerBar_Frame     = driver->getTexture("gui_power_frame.png");
    powerBar_Force     = driver->getTexture("gui_power_force.png");
    powerBar_Arrow     = driver->getTexture("gui_arrow.png");
    powerBar_Ready     = driver->getTexture("gui_ready.png");
    powerBar_Reloading = driver->getTexture("gui_reloading.png");
    powerBar_Rotation  = driver->getTexture("gui_rotation.png");

    /// Fill 2D textures:
    prisonersBar_Back = driver->getTexture("gui_prisoners_back.png");
    prisonersBar_Free = driver->getTexture("gui_prisoners_free.png");

    /// Fill 2D textures:
    game_Intro = driver->getTexture("game_intro.png");

    /// Fill 2D textures:
    game_View = driver->getTexture("game_view.png");

    /// Fill 2D textures:
    game_Over = driver->getTexture("game_over.png");



    /// Loading Bar: (Create)
    /// ex: driver->removeTexture(backgroundImageTexture); - OR - backgroundImageTexture = driver->getTexture("courtain.jpg"); - OR - NULL
    /// Loading Bar:
    CProgressBar* loadingBar = new CProgressBar(device, driver, guienv, NULL, rect<s32>(300, SCREEN_HEIGHT/2, SCREEN_WIDTH-300, SCREEN_HEIGHT/2+15), true, 7);
    loadingBar->setPercent(10); // 0 to 100
    loadingBar->draw();
    loadingBar->setVisible(true);


    /// Initialize all other game classes:
    cout << "level->Initialize() ... " << endl;
    scene->Initialize();        cout << "scene->Initialize() OK"      << endl;
    audios->Initialize();       cout << "audios->Initialize() OK"     << endl;
    animations->Initialize();   cout << "animations->Initialize() OK" << endl;
    effects->Initialize();      cout << "effects->Initialize() OK"    << endl;


    /// Loading Bar:
    loadingBar->setPercent(100);   // Last One: Must be 100
    loadingBar->draw();
    delete loadingBar; /// Remove now
    loadingBar = NULL; /// Remove now


    if(audios){
    audios->PlayMusic(2); /// Music: Ocean
    audios->PlayMusic(3); /// Music: People Talking
    audios->PlayFx(6,  cameras[0]->getPosition()); /// Sound: zoom
    /// audios->PlayFx(37, cameras[0]->getPosition()); /// Game end
    };

    std::cout << "Level created... Ok" << std::endl;
};





void Level::Loop(){

    driver->beginScene(true, true, video::SColor(255,255,255,255));

    // Work out a frame delta time.
    const u32 now = device->getTimer()->getTime();
    /// frameDeltaTime = (f32)(now - then)/1000.f; // Time in seconds
    frameDeltaTime = (f32)(now - then);
    then = now;


    u32 fps = driver->getFPS();


    if(lastFPS != fps){
    core::stringw tmp(L"Video Driver: "); tmp += driver->getName(); tmp += L" - FPS: "; tmp += fps;
    device->setWindowCaption(tmp.c_str());
    lastFPS = fps;
    };



    /// PHYSICS: UPDATING:
    /// irr::u32 stepSimulation(irr::f32 timeStep, irr::u32 maxSubSteps = 1, irr::f32 fixedTimeStep = irr::f32(1.) / irr::f64(60.));
    /// world->stepSimulation(frameDeltaTime*0.0100f, 60, f32(1.)/f64(60.));
    /// world->stepSimulation(frameDeltaTime*0.0100f, 60, f32(4.0)/f64(60.0)); /// Default was 1.0 BUT divided by 4.0 == MUCH MORE accurate physics and display movements !!!
    world->stepSimulation(frameDeltaTime*0.0100f, 60, f32(4.0)/f64(60.0)); /// Default was 1.0 BUT divided by 4.0 == MUCH MORE accurate physics and display movements !!!
    // world->stepSimulation(frameDeltaTime*0.010f, 60);
    /// world->stepSimulation(frameDeltaTime);
    // world->updateCollisionObjects(); <- Internal use only!
    // UpdatePhysics(frameDeltaTime);


    /// EVENTS: ( From: "libraries\MainLibs\myEvents.h" )



    if(receiver.IsKeyDown(irr::KEY_KEY_P)){
    /// Pause Game:
    // world->pauseSimulation(!world->simulationPaused());
    };

    /// IF GAME IS NOT PAUSED:
    // if(!world->simulationPaused()){




    if(USER_PRESSED_START==false && LEVEL_READY==false && CAN_EXIT==true){

    /// 1st) ESC == EXITS the game:
    static bool canPressEscape = true;
    if(receiver.IsKeyDown(irr::KEY_ESCAPE) && canPressEscape){
    canPressEscape = false;
    EXIT_GAME = true;
    };
    if(receiver.IsKeyUp(irr::KEY_ESCAPE)){
    canPressEscape = true;
    };

    };




    if(USER_PRESSED_START==false && LEVEL_READY==false && CAN_EXIT==true){

    /// Space key = STARTS the game:
    static bool canPressStart = true;
    if(receiver.IsKeyDown(irr::KEY_SPACE) && canPressStart){
    canPressStart = false;
    audios->PlayButton(1);
    audios->PlayFx(39, cameras[0]->getPosition()); /// Game end
    USER_PRESSED_START = true;
    GAME_OVER          = false;
    game_over_once     = true;
    fadeInMaterials    = 0;
    fadeOutMaterials   = 255;
    fadein_materials   = true;
    fadeout_materials  = false;
    /// Everything Jumps:
    // EveryoneJumps();
    /// Reset:
    ResetMap();
    };
    if(receiver.IsKeyUp(irr::KEY_SPACE)){
    canPressStart = true;
    };

    };





    if(USER_PRESSED_START==true && LEVEL_READY==true){

    /// 2nd) ESC == Goes to main menu the game:
    static bool canPressEscapeToMenu = true;
    if(receiver.IsKeyDown(irr::KEY_ESCAPE) && canPressEscapeToMenu){
    canPressEscapeToMenu = false;
    EXIT_GAME            = false;
    USER_PRESSED_START   = false;
    LEVEL_READY          = false;
    GAME_OVER            = false;
    game_over_once       = true;
    fadeInMaterials      = 0;
    fadeOutMaterials     = 255;
    fadein_materials     = false;
    fadeout_materials    = true;
    /// audios->PlayFx(38, cameras[0]->getPosition()); /// Game intro
    audios->PlayFx(6, cameras[0]->getPosition());
    audios->PlayButton(1);
    /// Everything Jumps:
    // EveryoneJumps();
    /// Reset:
    // ResetMap();
    /// Reset: (Removes all balls)
    RemoveBalls();
    };
    if(receiver.IsKeyUp(irr::KEY_ESCAPE)){
    canPressEscapeToMenu = true;
    };


    if(GAME_OVER == false){
    /// Space key = go to ship - or - fire
    if(receiver.IsKeyDown(irr::KEY_SPACE) && effects->CannonCanFireAgain()){
    /// Clear the ball that we have being following:
    cameraGoToOrigin = true;
    userIsMovingCamera = false;
    if(cameraIsNearShip == true){
    cannon_power += 0.55*frameDeltaTime;
    if(cannon_power>1200){ cannon_power = 1200; };
    if(cannon_power<200){ cannon_power = 200; };
    /// cout << "Force: " << cannon_power << endl;
    };
    if(cameraIsNearShip == false && canDoAudio == true){
    /// Audio:
    audios->PlayFx(6, vector3df(0,0,0));
    canDoAudio = false;
    };
    };
    if(receiver.IsKeyUp(irr::KEY_SPACE) && effects->CannonCanFireAgain() && cannon_power>200){
    if(cameraIsNearShip == true){
    CreateBall(cannon_power);
    old_bar_percentage = (100*cannon_power)/(1200-200)-20;
    Draw2DPowerBar(); /// Percentage between 2 numbers: (100*current)/total;
    cannon_power = 200;
    /// Cannon Particles:
    effects->CannonFiring(nodeCannon);
    /// Wave 2:
    animations->HitWave2(nodeCannon->getPosition());
    /// Audio:
    audios->PlayFx(3, vector3df(0,0,0));
    canDoAudio = true;
    /// Shake Camera:
    shakeCamera = 0.0;
    };
    };
    };


    if(GAME_OVER == false){
    /// Get Cannon Rotation:
    vector3df cannonRot = nodeCannon->getRotation();

    if(receiver.IsKeyDown(irr::KEY_KEY_Q)){
    /// Cannon Rotation:
    cannonRot.Z += 0.030*frameDeltaTime;
    if(cannonRot.Z>90){ cannonRot.Z = 90; };
    if(cannonRot.Z<90){ audios->PlayFx(26, nodeCannon->getPosition()); }; /// Do sound
    };
    if(receiver.IsKeyDown(irr::KEY_KEY_A)){
    /// Cannon Rotation:
    cannonRot.Z -= 0.030*frameDeltaTime;
    if(cannonRot.Z<0){ cannonRot.Z = 0; };
    if(cannonRot.Z>0){ audios->PlayFx(26, nodeCannon->getPosition()); }; /// Do sound
    };

    /// Apply New Cannon Rotation:
    nodeCannon->setRotation(vector3df(0,0,cannonRot.Z));
    };



    /// Reload Level:
    if(RELOAD_LEVEL_KEY){
    static bool canPressR = true;
    if(receiver.IsKeyDown(irr::KEY_KEY_R) && canPressR){
    canPressR = false;
    audios->PlayButton(1);
    this->Initialize();
    };
    if(receiver.IsKeyUp(irr::KEY_KEY_R)){
    canPressR = true;
    };
    // if(event.KeyInput.Key == KEY_KEY_P && event.KeyInput.PressedDown == false)
    };


    /// Show/Hide texts:
    if(GAME_OVER == false){
    static bool canPressT = true;
    if(receiver.IsKeyDown(irr::KEY_KEY_T) && canPressT){
    canPressT = false;
    audios->PlayButton(1);
    if(textInfos1){
    if(textInfos1->isVisible()){
        textInfos1->setVisible(false);
        kingOrderBalloonDuration = 0.0;
        kingOrderWave2 = true;
        kingOrder = "hide";
        audios->PlayTalk(3, cameras[0]->getPosition()); /// King order
    } else {
        textInfos1->setVisible(true);
        kingOrderBalloonDuration = 0.0;
        kingOrderWave2 = true;
        kingOrder = "show";
        audios->PlayTalk(3, cameras[0]->getPosition()); /// King order
    };
    };

    };
    if(receiver.IsKeyUp(irr::KEY_KEY_T)){
    canPressT = true;
    };
    };



    /// Music On/Off:
    if(GAME_OVER == false){
    static bool canPressM = true;
    if(receiver.IsKeyDown(irr::KEY_KEY_M) && canPressM){
    canPressM = false;
    audios->PlayButton(1);
    if(music_01){
    if(music_01->isPlaying()){
        music_01->stop();
        musiciansAnimate = false;
        kingOrderBalloonDuration = 0.0;
        kingOrderWave2 = true;
        kingOrder = "silence";
        audios->PlayTalk(3, cameras[0]->getPosition()); /// King order
    } else {
        music_01->play();
        musiciansAnimate = true;
        kingOrderBalloonDuration = 0.0;
        kingOrderWave2 = true;
        kingOrder = "music";
        audios->PlayTalk(3, cameras[0]->getPosition()); /// King order
    };
    };
    };
    if(receiver.IsKeyUp(irr::KEY_KEY_M)){
    canPressM = true;
    };
    };


    }; /// End: USER_INPUT



    /// DEBUG ONLY - Test stuff F key:
    if(DEBUG_FUNCTIONS_KEY){
    static bool canPressF = true;
    if(receiver.IsKeyDown(irr::KEY_KEY_F) && canPressF){
    canPressF = false;
    audios->PlayButton(1);
    /// Everything Jumps:
    EveryoneJumps();
    /// Test bell timmer:
    canHitBellAgainTimer = 0.0;
    /// Shake Camera:
    shakeCamera = 0.0;
    };
    if(receiver.IsKeyUp(irr::KEY_KEY_F)){
    canPressF = true;
    };
    };


    /// DEBUG ONLY - Select Camera:
    if(DEBUG_CAMERA_KEY){
    static bool canPressC = true;
    static int selectedCam = 0;
    if(receiver.IsKeyDown(irr::KEY_KEY_C) && canPressC){
    canPressC = false;
    audios->PlayButton(1);
    if(selectedCam<cameras.size()-1){ selectedCam+=1; } else { selectedCam=0; };
    smgr->setActiveCamera(cameras[selectedCam]);
    cout << "selectedCam: " << selectedCam << endl;
    };
    if(receiver.IsKeyUp(irr::KEY_KEY_C)){
    canPressC = true;
    };
    };







    /// Process All Collisions:
    CheckCollisions();

    /// Scene loop:
    scene->Loop();

    /// Effects loop:
    effects->Loop();

    /// Effects loop:
    animations->Loop();

    /// Audios loop:
    audios->Loop();

    /// Set light new position:
    LightSetPosition();

    /// Set camera new position and target:
    CameraSetTargetAndPosition();

    /// Shake Camera?
    CameraShake();

    /// Fade in/out materials?
    scene->FadeMaterials();




    /// OnResize window, do...
    int sw = driver->getScreenSize().Width;
    int sh = driver->getScreenSize().Height;
    driver->OnResize(core::dimension2d<irr::u32>(sw,sh));
    /// Update globals:
    SCREEN_WIDTH  = sw;
    SCREEN_HEIGHT = sh;
    /// OnResize main widow, update camera ratio:
    // cameras[0]->setAspectRatio((float)sw/(float)sh);
    /// OnResize main widow, update listbox position: (Songs list)
    // musicListBox->setRelativePosition(core::rect<s32>(10,360,165,sh-20));
    /// Keep constant focus REMOVE on Listbox: (So its own ListBox keys events dont mess with other IMP keys!)
    // guienv->removeFocus(musicListBox); // Opposite is: guienv->setFocus(musicListBox);
    /// Update IGUI texts positions:
    // textInfos1->setRelativePosition(core::rect<s32>(20,SCREEN_HEIGHT-25,SCREEN_WIDTH,SCREEN_HEIGHT));


    /// OnResize main widow, update text position: (Infos texts)
    int th = textInfos1->getTextHeight(); /// Get current lyrics font size (height)
    // int tw = textInfos1->getTextWidth(); /// Get current lyrics font size (height)
    textInfos1->setRelativePosition(core::rect<s32>(0,(sh-th)-15,sw,sh));


    /*
    if(bodySoldier.size()>0){
    if(((IRigidBody*)bodySoldier[0])->isInWorld()){ /// Body exists? (Could have been removed, if not exists it crashes!)
    u32 id = ((IRigidBody*)bodySoldier[0])->getUniqueID();
    if(world->getCollisionObjectByID(id)){ world->removeCollisionObject(world->getCollisionObjectByID(id)); };
    };
    };
    */

    /*
    btTransform trans = bodySoldier[0]->getPointer()->getCenterOfMassTransform();

    irr::core::matrix4 mat;
    mat.setTranslation(vector3df(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
    mat.setRotationDegrees(vector3df(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ()));
    nodeSoldier[0]->setPosition(mat.getTranslation());
    nodeSoldier[0]->setRotation(mat.getRotationDegrees());
    */




    // DEBUG_BULLET = true;

    // Draw physics debug:
    if(DEBUG_BULLET){

    core::array<scene::ISceneNode*> allNodes;
    smgr->getSceneNodesFromType(scene::ESNT_ANY, allNodes); // Find all nodes
    for(unsigned int i=0; i<allNodes.size(); ++i){
    scene::ISceneNode* node = allNodes[i];  // Create node
    node->setMaterialFlag(video::EMF_WIREFRAME, true);
    // node->setMaterialFlag(video::EMF_POINTCLOUD, true);
    };

    /// world->setDebugMode(true);
    if(world){
    world->debugDrawWorld(true);
    world->debugDrawProperties(true);
    };
    };



    // }; // if (!world->simulationPaused()){};

    smgr->drawAll(); /// Draw scene (all nodes)

    /// -------------------------------------------------------------------------------------------
    /// Everything is 2D must be drawn AFTER smgr->drawAll():
    /// NOTE: The 2D order here will be layers order too...
    /// -------------------------------------------------------------------------------------------

    // drawLines(); /// Render lines, only after the rest is rendered

    /// In main game (Game View):
    Draw2DView();

    /// Over (GAME OVER):
    Draw2DOver();

    /// Jails Energy Bars:
    effects->HitJail2DEnergyBarLoop();

    /// Soldiers Balloons:
    effects->SoldiersBalloonLooping();

    /// King Animations + Balloons:
    animations->KingLooping();

    /// Prisoners Arrested Balloons:
    animations->PrisonersArrestedBalloonLooping();

    /// Prisoners Escaping Balloons:
    animations->PrisonersEscapingBalloonLooping();

    /// Priest Drinking + Balloons:
    animations->PriestsDrunkLoop();

    /// Soldiers Walking and Random Balloons:
    animations->SoldiersLoop();

    /// Cannon Power Bar:
    new_bar_percentage = ((100*cannon_power)/(1200-200))-20; /// Percentage between 2 numbers: (100*current)/total;
    Draw2DPowerBar(); /// Render power bar. 2D draws, must be after drawAll()

    /// Prisoners Bar:
    Draw2DPrisonersBar();

    /// Hit Waves 1: (Normal ones)
    effects->HitWave1Loop();

    /// Hit Waves 2: (Big ones)
    animations->HitWave2Loop();

    /// Hit Waves 3: (Very small ones)
    animations->HitWave3Loop();

    /// GAME OVER:
    animations->GameOverLoop();

    /// Intro (Game Splash Screen):
    Draw2DIntro();


    /// Ball trajectory preview with line, draw only after the rest is rendered
    // drawTrajectoryLines();

    /// -------------------------------------------------------------------------------------------

    /// driver->setMaterial(irr::video::SMaterial()); // Update all materials (else some will be inverted/distorted)
    /// driver->setMaterial(driver->getMaterial2D()); // Updates all 2D materials (else some will be inverted/distorted when using effects)
    guienv->drawAll();  /// Draw GUI, only after the rest is rendered
    driver->endScene(); /// ENDS RENDER

};






void Level::Draw2DPowerBar(){

    if(LEVEL_READY == true && GAME_OVER == false){

    /// Note: The SColors(a,r,g,b) here, paints the incoming textures

    /// Alphas values:
    f32 alpha1 = 40, alpha2 = 15;
    if(effects->CannonCanFireAgain() == true && cameraIsNearShip == true){
    alpha1 = 255; alpha2 = 55;
    };

    int PosX = 65; /// Default X position for all drawn elements
    int PosY = (SCREEN_HEIGHT/2); /// Default Y start (draws always centered on Y)


    /// Angles Texts:
    int anglez = nodeCannon->getRotation().Z;
    /// Character codes (for the angle symbol) --> http://www.utf8-chartable.de/unicode-utf8-table.pl?start=128&number=128&utf8=string-literal&unicodeinhtml=hex
    stringw an = L""; an += anglez; an += L"\xba";
    textAngle->setText(an.c_str());
    textAngle->setRelativePosition(core::rect<s32>(PosX+35,PosY-95,SCREEN_WIDTH,SCREEN_HEIGHT));
    textAngle->setOverrideColor(video::SColor(alpha1,255,255,255)); // opacity, red, green, blue


    /// "rotation" background image:
    if(powerBar_Rotation){ driver->draw2DImage(powerBar_Rotation, position2d<s32>(PosX+25, PosY-60), core::rect<s32>(0,0,50,50), 0, video::SColor(alpha1,255,255,255), true); };

    /// calculate the "rotation" line:
    f32 angle  = -(nodeCannon->getRotation().Z); // f32 angle = 45;
    f32 posX1  = PosX+25;
    f32 posY1  = PosY-11;
    f32 length = 48;
    f32 posX2  = (int)round(posX1+length*cos(angle*3.14/180.0));
    f32 posY2  = (int)round(posY1+length*sin(angle*3.14/180.0));
    /// Draw the line:
    driver->draw2DLine(core::position2d<s32>(posX1, posY1), core::position2d<s32>(posX2, posY2), video::SColor(alpha1,255,255,255));

    /// "power bar" image:
    if(powerBar_Force){ driver->draw2DImage(powerBar_Force, position2d<s32>(PosX, PosY+0), core::rect<s32>(0,0,old_bar_percentage,10), 0, video::SColor(alpha2,255,255,255), true); };
    if(powerBar_Force){ driver->draw2DImage(powerBar_Force, position2d<s32>(PosX, PosY+0), core::rect<s32>(0,0,new_bar_percentage,10), 0, video::SColor(alpha1,255,255,255), true); };
    /// "power bar" frame image:
    if(powerBar_Frame){ driver->draw2DImage(powerBar_Frame, position2d<s32>(PosX, PosY+0), core::rect<s32>(0,0,100,10), 0, video::SColor(alpha1,255,255,255), true); };

    /// "ready" image:
    if(cameraIsNearShip==true && effects->CannonCanFireAgain() == true){
    if(powerBar_Ready){ driver->draw2DImage(powerBar_Ready, position2d<s32>(PosX, PosY+20), core::rect<s32>(0,0,100,40), 0, video::SColor(255,255,255,255), true); };
    };

    /// "reloading" image:
    if(cameraIsNearShip==true && effects->CannonCanFireAgain() == false){
    if(powerBar_Reloading){ driver->draw2DImage(powerBar_Reloading, position2d<s32>(PosX, PosY+20), core::rect<s32>(0,0,100,40), 0, video::SColor(255,255,255,255), true); };
    };

    /// "arrow" image, blinking:
    if(cameraIsNearShip==false){
    if(arrow2DTimer< 150){ if(powerBar_Arrow){ driver->draw2DImage(powerBar_Arrow, position2d<s32>(PosX, PosY+20), core::rect<s32>(0,0,100,40), 0, video::SColor(255,255,255,255), true); }; };
    if(arrow2DTimer>=150){ if(powerBar_Arrow){ driver->draw2DImage(powerBar_Arrow, position2d<s32>(PosX, PosY+20), core::rect<s32>(0,0,100,40), 0, video::SColor( 80,255,255,255), true); }; };
    arrow2DTimer+=0.5*frameDeltaTime; if(arrow2DTimer>=300){ arrow2DTimer = 0.0; };
    } else { arrow2DTimer = 0.0; };

    };
};





void Level::Draw2DPrisonersBar(){

    if(LEVEL_READY == true){
    int free = 4-prisoners.size();
    free = abs(free);
    /// cout << "Free: " << free << endl;
    int cut = 16;
    if(free<=0){ cut = 0;     };
    if(free==1){ cut = cut*1; };
    if(free==2){ cut = cut*2; };
    if(free==3){ cut = cut*3; };
    if(free==4){ cut = cut*4; };
    /// "prisoners bar" image:
    if(prisonersBar_Back){ driver->draw2DImage(prisonersBar_Back, position2d<s32>(20+cut,60), core::rect<s32>(0,0,66-cut,20), 0, video::SColor(255,255,255,255), true); };
    if(prisonersBar_Free){ driver->draw2DImage(prisonersBar_Free, position2d<s32>(20,60), core::rect<s32>(0,0,cut,20), 0, video::SColor(255,255,255,255), true); };
    };

};


void Level::Draw2DIntro(){

    if(LEVEL_READY == false){
    /// Game "intro" image:
    if(game_Intro){
    /// driver->draw2DImage(texture, destRect, textureFrame, clipRect, arColors, true);
    driver->draw2DImage(game_Intro, core::rect<s32>(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), core::rect<s32>(0, 0, game_Intro->getSize().Width, game_Intro->getSize().Height), 0, 0, true);
    };
    };

};

void Level::Draw2DView(){

    if(LEVEL_READY == true && GAME_OVER == false){
    /// Game "view" image:
    if(game_View){
    /// driver->draw2DImage(texture, destRect, textureFrame, clipRect, arColors, true);
    driver->draw2DImage(game_View, core::rect<s32>(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), core::rect<s32>(0, 0, game_View->getSize().Width, game_View->getSize().Height), 0, 0, true);
    };
    };

};


void Level::Draw2DOver(){

    if(LEVEL_READY == true && GAME_OVER == true){
    /// Game "over" image:
    if(game_Over){
    /// driver->draw2DImage(texture, destRect, textureFrame, clipRect, arColors, true);
    driver->draw2DImage(game_Over, core::rect<s32>(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), core::rect<s32>(0, 0, game_Over->getSize().Width, game_Over->getSize().Height), 0, 0, true);
    };
    };

};




/// IMPORTANT: "irrBulletCollisionCallBackInformation.cpp" has a BUG! Correct by me at line 21 (where is body1, should be body0) !!!
/// irrBullet: collisionexample.cpp --> https://github.com/danyalzia/irrBullet/blob/master/examples/Collision/collisionexample.cpp
/// myRigidBodyPtr->setUserPointer((void*)mySceneNodePtr);

/// CHECK COLLISIONS:
void Level::CheckCollisions(){



    for(unsigned int i=0; i<world->getNumManifolds(); i++){

    ICollisionCallbackInformation* info = world->getCollisionCallback(i);

    /// Must be set as constants! Else they will mess on remove body:
    const ICollisionObject* obj0 = info->getBody0();
    const ICollisionObject* obj1 = info->getBody1();


    /// Objects have this attribute?...
    bool scene0          = obj0->getAttributes()->existsAttribute("scene");
    bool scene1          = obj1->getAttributes()->existsAttribute("scene");
    bool ball0           = obj0->getAttributes()->existsAttribute("ball");
    bool ball1           = obj1->getAttributes()->existsAttribute("ball");
    bool soldier0        = obj0->getAttributes()->existsAttribute("soldier");
    bool soldier1        = obj1->getAttributes()->existsAttribute("soldier");
    bool box0            = obj0->getAttributes()->existsAttribute("box");
    bool box1            = obj1->getAttributes()->existsAttribute("box");
    bool target0         = obj0->getAttributes()->existsAttribute("target");
    bool target1         = obj1->getAttributes()->existsAttribute("target");
    bool jail0           = obj0->getAttributes()->existsAttribute("jail");
    bool jail1           = obj1->getAttributes()->existsAttribute("jail");
    bool prisoner0       = obj0->getAttributes()->existsAttribute("prisoner");
    bool prisoner1       = obj1->getAttributes()->existsAttribute("prisoner");
    bool bird0           = obj0->getAttributes()->existsAttribute("bird");
    bool bird1           = obj1->getAttributes()->existsAttribute("bird");
    bool cargo0          = obj0->getAttributes()->existsAttribute("cargo");
    bool cargo1          = obj1->getAttributes()->existsAttribute("cargo");
    bool chicken0        = obj0->getAttributes()->existsAttribute("chicken");
    bool chicken1        = obj1->getAttributes()->existsAttribute("chicken");
    bool bell0           = obj0->getAttributes()->existsAttribute("bell");
    bool bell1           = obj1->getAttributes()->existsAttribute("bell");
    bool priest0         = obj0->getAttributes()->existsAttribute("priest");
    bool priest1         = obj1->getAttributes()->existsAttribute("priest");
    bool chain0          = obj0->getAttributes()->existsAttribute("chain");
    bool chain1          = obj1->getAttributes()->existsAttribute("chain");
    bool platform0       = obj0->getAttributes()->existsAttribute("platform");
    bool platform1       = obj1->getAttributes()->existsAttribute("platform");
    bool worker0         = obj0->getAttributes()->existsAttribute("worker");
    bool worker1         = obj1->getAttributes()->existsAttribute("worker");


    // obj0->getPointer()->getMotionState()->updateObject();
    // obj1->getPointer()->updateObject();

    /// Example Code for irrBullet == info->getContactPoint(c);
    /// Example Code for btBullet  == info->getPointer()->getContactPoint(c);

    /// Check trough all the contact points:
    /// u32 totalContacts = info->getPointer()->getNumContacts();
    /// for(unsigned int c=0; c<totalContacts; c++){

    /// btManifoldPoint& point = info->getPointer()->getContactPoint(0);
    /// f32 distance = point.getDistance();
    /// f32 impulse  = point.getAppliedImpulse();
    /// f32 lifetime = point.getLifeTime(); /// lifetime >10 == Probably objects are stuck (We must remove one)




    bool removeobj0 = false;
    bool removeobj1 = false;

    /// NORMAL COLLISIONS:
    /// if(distance<=1.0f){
    if(obj0 && obj1){ /// Objects exists?
    if(((IRigidBody*)obj0)->isInWorld() && ((IRigidBody*)obj1)->isInWorld()){ /// Body exists? (Could have been removed, if not exists it crashes!)

    // float impulse = point.m_appliedImpulse; // get impules
    // const btVector3& ptA = point.getPositionWorldOnA(); // global contact position
    // const btVector3& ptB = point.getPositionWorldOnB(); // global contact position
    // const btVector3& normalOnB = point.m_normalWorldOnB; // global contact vector

    /// ---------------------------------------------------------------------------------------------------------------

    /// Get current applied forces (linear velocity) on obj0:
    btVector3 f0 = ((IRigidBody*)obj0)->getPointer()->getLinearVelocity();
    vector3df forces0 = vector3df(abs(f0.getX()), abs(f0.getY()), abs(f0.getZ()));

    /// Get current applied forces (linear velocity) on obj1:
    btVector3 f1 = ((IRigidBody*)obj1)->getPointer()->getLinearVelocity();
    vector3df forces1 = vector3df(abs(f1.getX()), abs(f1.getY()), abs(f1.getZ()));

    /// Get current position on obj0:
    vector3df pos0 = ((IRigidBody*)obj0)->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)

    /// Get current position on obj1:
    vector3df pos1 = ((IRigidBody*)obj1)->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)

    /// ---------------------------------------------------------------------------------------------------------------





    /// COLLISIONS: Soldiers - VS - Scene Obstacles
    if(soldier0 && scene1){
    /// Reverse one soldier only:
    /// reverseImpulse(((IRigidBody*)obj0));
    // cout << "lifetime: " << lifetime << " impulse: " << impulse << endl;
    /// if(lifetime>=10){ reverseImpulse(((IRigidBody*)obj0)); };
    };
    if(soldier1 && scene0){
    /// Reverse one soldier only:
    /// reverseImpulse(((IRigidBody*)obj1));
    // cout << "lifetime: " << lifetime << " impulse: " << impulse << endl;
    /// if(lifetime>=10){ reverseImpulse(((IRigidBody*)obj1)); };
    };


   /// ---------------------------------------------------------------------------------------------------------------



    /// COLLISIONS: Cargo - VS - Scene
    if(cargo0 && scene1){
    audios->PlayFx(35, pos0); /// Bass
    audios->PlayFx(13, pos0); /// Barrels bump audio
    effects->HitScore(pos0, 100, "yellow");
    animations->HitWave2(pos0);
    removeobj0 = true;
    };
    if(cargo1 && scene0){
    audios->PlayFx(35, pos1); /// Bass
    audios->PlayFx(13, pos1); /// Barrels bump audio
    effects->HitScore(pos1, 100, "yellow");
    animations->HitWave2(pos1);
    removeobj1 = true;
    };


    /// ---------------------------------------------------------------------------------------------------------------


    /// COLLISIONS: Balls - VS - Workers
    if(ball0 && worker1){
    audios->PlayFx(9, pos1);
    effects->HitScore(pos1, 20, "yellow");
    animations->HitWave2(pos1);
    /// Jump Worker:
    HitBodyJump(((IRigidBody*)obj1),1000);
    /// Jump Platform:
    HitBodyJump(((IRigidBody*)platforms[0].Body),2000);
    };
    if(ball1 && worker0){
    audios->PlayFx(9, pos0);
    effects->HitScore(pos0, 20, "yellow");
    animations->HitWave2(pos0);
    /// Jump Worker:
    HitBodyJump(((IRigidBody*)obj0),1000);
    /// Jump Platform:
    HitBodyJump(((IRigidBody*)platforms[0].Body),2000);
    };


    /// ---------------------------------------------------------------------------------------------------------------


    /// COLLISIONS: Balls - VS - Birds
    if(ball0 && bird1){
    effects->HitScore(pos1, rand()%9+1, "yellow");
    audios->PlayFx(11, pos1);
    animations->HitWave2(pos1);
    /// Remove ball:
    // removeobj0 = true;
    };
    if(ball1 && bird0){
    effects->HitScore(pos0, rand()%9+1, "yellow");
    audios->PlayFx(11, pos0);
    animations->HitWave2(pos0);
    /// Remove ball:
    // removeobj1 = true;
    };


    /// ---------------------------------------------------------------------------------------------------------------


    /// COLLISIONS: Chickens - VS - Balls:
    if(chicken0 && ball1){
    if(forces1>=vector3df(20,20,0)){
    /// Audio:
    effects->HitScore(pos0, rand()%4+1, "yellow");
    audios->PlayFx(9, pos0);   /// Sound: jump
    audios->PlayFx(14, pos0);  /// Sound: coook!
    animations->HitWave2(pos0);
    };
    /// Chicken Jumps:
    HitBodyJump(((IRigidBody*)obj0),5000);
    };
    if(chicken1 && ball0){
    if(forces0 >= vector3df(20,20,0)){
    /// Audio:
    effects->HitScore(pos1, rand()%4+1, "yellow");
    audios->PlayFx(9, pos1);   /// Sound: jump
    audios->PlayFx(14, pos1);  /// Sound: coook!
    animations->HitWave2(pos1);
    };
    /// Chicken Jumps:
    HitBodyJump(((IRigidBody*)obj1),5000);
    };

    /// COLLISIONS: Chickens - VS - Many other objects:
    if(chicken0 && (cargo1 || box1)){
    /// Chicken Jumps:
    HitBodyJump(((IRigidBody*)obj0),500);
    for(unsigned int i=0; i<chickens.size(); i++) { if(((IRigidBody*)obj0)->getUniqueID() == chickens[i].BodyID){ chickens[i].Colliding = true; }; };
    };
    if(chicken1 && (cargo0 || box0)){
    /// Chicken Jumps:
    HitBodyJump(((IRigidBody*)obj1),500);
    // if(lifetime>=10){ BodyRemove(((IRigidBody*)obj0), "hit"); /// An IF with BodyRemove() MUST be always in the end !!!
    /// Set colliding flag to true (need for animations)
    for(unsigned int i=0; i<chickens.size(); i++) { if(((IRigidBody*)obj1)->getUniqueID() == chickens[i].BodyID){ chickens[i].Colliding = true; }; };
    };



    /// ---------------------------------------------------------------------------------------------------------------



    /// COLLISIONS: Balls - VS - Bell
    if(ball0 && bell1){
    if(forces0>=vector3df(10,10,0)){
    /// Void too many and continuous hits on bell:
    if(canHitBellAgainTimer>=1000){
    effects->HitScore(pos1, 50, "yellow");
    animations->HitWave2(pos1);
    audios->PlayFx(18, cameras[0]->getPosition()); /// Bell
    audios->PlayFx(24, cameras[0]->getPosition()); /// Yell
    /// Reset:
    canHitBellAgainTimer = 0.0;
    /// Priests:
    animations->PriestsScared();
    /// Shake Camera:
    shakeCamera = 0.0;
    /// Everything Jumps:
    EveryoneJumps();
    };
    };
    };
    if(ball1 && bell0){
    if(forces1>=vector3df(10,10,0)){
    /// Void too many and continuous hits on bell:
    if(canHitBellAgainTimer>=1000){
    effects->HitScore(pos0, 50, "yellow");
    animations->HitWave2(pos0);
    audios->PlayFx(18, cameras[0]->getPosition()); /// Bell
    audios->PlayFx(24, cameras[0]->getPosition()); /// Yell
    /// Reset:
    canHitBellAgainTimer = 0.0;
    /// Priests:
    animations->PriestsScared();
    /// Shake Camera:
    shakeCamera = 0.0;
    /// Everything Jumps:
    EveryoneJumps();
    };
    };
    };

    /// Hit Bell Timer:
    canHitBellAgainTimer += 0.005*frameDeltaTime;
    if(canHitBellAgainTimer>1000){ canHitBellAgainTimer = 1000; };
    /// std::cout << "canHitBellAgainTimmer: " << canHitBellAgainTimmer << std::endl;



    /// ---------------------------------------------------------------------------------------------------------------



    /// COLLISIONS: Soldiers - VS - Boxes
    if(soldier0 && box1){
    /// Soldier Jumps:
    HitBodyJump(((IRigidBody*)obj0),4000);
    if(forces0>=vector3df(5,5,0) || forces1>=vector3df(5,5,0)){
    /// Particles:
    effects->HitBox(pos1);
    /// Remove box:
    removeobj1 = true;
    };
    };
    if(soldier1 && box0){
    /// Soldier Jumps:
    HitBodyJump(((IRigidBody*)obj1),4000);
    if(forces0>=vector3df(5,5,0) || forces1>=vector3df(5,5,0)){
    /// Particles:
    effects->HitBox(pos0);
    /// Remove box:
    removeobj0 = true;
    };
    };

    /// ---------------------------------------------------------------------------------------------------------------



    /// COLLISIONS: Soldiers - VS - Platform
    /*
    if(soldier0 && platform1){
    /// Soldier Jumps:
    HitBodyJump(((IRigidBody*)obj0),4000);
    };
    if(soldier1 && platform0){
    /// Soldier Jumps:
    HitBodyJump(((IRigidBody*)obj1),4000);
    };
    */


    /// ---------------------------------------------------------------------------------------------------------------



    /// COLLISIONS: There is a ball? Start countdown to remove it...
    /*
    if(ball0){
    /// Start countdown to remove the ball:
    timer_ballInScene+=(0.5*frameDeltaTime);
    /// Remove ball - too much time:
    if(timer_ballInScene>10000){ timer_ballInScene = 0.0; BodyRemove(((IRigidBody*)obj0), "hit"); };
    /// Remove ball - too much hitting:
    if(lifetime>=11 && impulse<=0){ BodyRemove(((IRigidBody*)obj0), "hit"); };
    };
    if(ball1){
    /// Start countdown to remove the ball:
    timer_ballInScene+=(0.5*frameDeltaTime);
    /// Remove ball - too much time:
    if(timer_ballInScene>10000){ timer_ballInScene = 0.0; BodyRemove(((IRigidBody*)obj1), "hit"); };
    /// Remove ball - too much hitting:
    if(lifetime>=11 && impulse<=0){ BodyRemove(((IRigidBody*)obj1), "hit"); };
    };
    */

    /// COLLISIONS: Balls - VS - Everything...
    if(ball0){
    if(ball0 && (!scene1 && !platform1 && !jail1 && !box1 && !cargo1)){
    /// Small ball audio:
    audios->PlayFx(20,pos0);
    /// Very small wave:
    animations->HitWave3(pos0);
    };
    if(forces0.Y>=40){ /// Audio:
    effects->HitWave1((IRigidBody*)obj0);
    audios->PlayFx(15,pos0);
    };
    // cout << "Forces on ball0: " << forces0 << endl;
    if(forces0.X<=1 && forces0.Y<=1){
    audios->PlayFx(17,pos0);
    animations->HitWave2(pos0);
    removeobj0 = true;
    };
    };
    if(ball1){
    if(ball1 && (!scene0 && !platform0 && !jail0 && !box0 && !cargo0)){
    /// Small ball audio:
    audios->PlayFx(20,pos1);
    /// Very small wave:
    animations->HitWave3(pos1);
    };
    if(forces1.Y>=40){ /// Audio:
    effects->HitWave1((IRigidBody*)obj1);
    audios->PlayFx(15,pos1);
    };
    // cout << "Forces on ball1: " << forces1 << endl;
    if(forces1.X<=1 && forces1.Y<=1){
    audios->PlayFx(17,pos1);
    animations->HitWave2(pos1);
    removeobj1 = true;
    };
    };


    /// COLLISIONS: Balls - VS - Scene
    /*
    if(ball0 && scene1){
    };
    if(ball1 && scene0){
    };
    */


    /// ---------------------------------------------------------------------------------------------------------------


    /// COLLISIONS: Balls - VS - Boxes
    if(ball0 && box1){
    if(forces0>vector3df(20,20,0)){
    effects->HitBox(pos1);
    audios->PlayFx(7,pos1);
    /// Has Bonus?
    effects->HitHasBonus((IRigidBody*)obj1);
    /// Remove:
    // world->addToDeletionQueue((IRigidBody*)obj1);
    };
    };
    if(ball1 && box0){
    if(forces1>vector3df(20,20,0)){
    effects->HitBox(pos0);
    audios->PlayFx(7,pos0);
    /// Has Bonus?
    effects->HitHasBonus((IRigidBody*)obj0);
    /// Remove box:
    // world->addToDeletionQueue((IRigidBody*)obj0);
    };
    };


    /// ---------------------------------------------------------------------------------------------------------------


    /// COLLISIONS: Balls - VS - Cargo
    if(ball0 && cargo1){
    if(forces0>vector3df(30,30,0)){
    animations->HitWave2(pos1);
    audios->PlayFx(13, pos1);
    effects->HitScore(pos0, rand()%19+10, "yellow");
    };
    };
    if(ball1 && cargo0){
    if(forces1>vector3df(30,30,0)){
    animations->HitWave2(pos0);
    audios->PlayFx(13, pos0);
    effects->HitScore(pos1, rand()%19+10, "yellow");
    };
    };


    /// ---------------------------------------------------------------------------------------------------------------


    /// COLLISIONS: Balls - VS - Platform
    if(ball0 && platform1){
    if(forces0>vector3df(10,30,0)){
    audios->PlayFx(13, pos1);
    };
    };
    if(ball1 && platform0){
    if(forces1>vector3df(10,30,0)){
    audios->PlayFx(13, pos0);
    };
    };


    /// ---------------------------------------------------------------------------------------------------------------


    /// COLLISIONS: Balls - VS - Soldiers
    if(ball0 && soldier1){
    if(forces0>vector3df(20,20,0)){
    // cout << "lifetime: " << lifetime << " impulse: platforms" << impulse << " distance: " << distance << endl;
    /// Audio:
    audios->PlayFx(9, pos1);   /// Sound: jump
    audios->PlayTalk(6, pos1); /// Sound: oh!
    /// Smoke:
    effects->HitSoldier((IRigidBody*)obj1);
    /// Text Ballon:
    effects->SoldiersBalloon((IRigidBody*)obj1);
    effects->HitScore(pos1, rand()%4+1, "yellow");
    animations->HitWave2(pos1);
    };
    /// Jump:
    HitBodyJump(((IRigidBody*)obj1),5000);
    };
    if(ball1 && soldier0){
    if(forces1>vector3df(20,20,0)){
    // cout << "lifetime: " << lifetime << " impulse: " << impulse << " distance: " << distance << endl;
    /// Audio:
    audios->PlayFx(9, pos0);   /// Sound: jump
    audios->PlayTalk(6, pos0); /// Sound: oh!
    /// Smoke:
    effects->HitSoldier((IRigidBody*)obj0);
    /// Text Ballon:
    effects->SoldiersBalloon((IRigidBody*)obj0);
    effects->HitScore(pos0, rand()%4+1, "yellow");
    animations->HitWave2(pos0);
    };
    /// Jump:
    HitBodyJump(((IRigidBody*)obj0),5000);
    };


    /// ---------------------------------------------------------------------------------------------------------------


    /// COLLISIONS: Ball - VS - Targets
    if(ball0 && target1){
    /// Void too many and continuous hits on target:
    if(canHitTargetAgainTimer>=1000){
    /// Score:
    effects->HitScore(pos1, rand()%10+1, "yellow");
    animations->HitWave2(pos1);
    /// Audio:
    audios->PlayFx(32, pos1);
    /// Reset:
    canHitTargetAgainTimer = 0.0;
    /// Shake Camera:
    shakeCamera = 0.0;
    /// Everything Jumps:
    EveryoneJumps();
    /// Removes this target: (Must be the last function here)
    /// removeobj1 = true;
    };
    };
    if(ball1 && target0){
    /// Void too many and continuous hits on target:
    if(canHitTargetAgainTimer>=1000){
    /// Score:
    effects->HitScore(pos0, rand()%10+1, "yellow");
    animations->HitWave2(pos0);
    /// Audio:
    audios->PlayFx(32, pos0);
    /// Reset:
    canHitTargetAgainTimer = 0.0;
    /// Shake Camera:
    shakeCamera = 0.0;
    /// Everything Jumps:
    EveryoneJumps();
    /// Removes this target: (Must be the last function here)
    /// removeobj0 = true;
    };
    };


    /// Hit Target Timer:
    canHitTargetAgainTimer += 0.005*frameDeltaTime;
    if(canHitTargetAgainTimer>1000){ canHitTargetAgainTimer = 1000; };
    /// std::cout << "canHitTargetAgainTimer: " << canHitTargetAgainTimer << std::endl;


    /// ---------------------------------------------------------------------------------------------------------------


    /// COLLISIONS: Ball - VS - Jails
    if(ball0 && jail1){
    /// "Remove" Jail and Prisoner?...
    if(forces0>vector3df(20,20,0)){
    if(effects->HitJail(((IRigidBody*)obj1), 10)>0){
    effects->HitScore(pos0, 10, "yellow");
    animations->HitWave2(pos1);
    audios->PlayFx(4, pos0);
    };
    };
    };
    if(ball1 && jail0){
    /// "Remove" Jail and Prisoner?...
    if(forces1>vector3df(20,20,0)){
    if(effects->HitJail(((IRigidBody*)obj0), 10)>0){
    effects->HitScore(pos1, 10, "yellow");
    animations->HitWave2(pos0);
    audios->PlayFx(4, pos1);
    };
    };
    };


    /// ---------------------------------------------------------------------------------------------------------------



    };

    }; // Ends: if(obj0 && obj1){}
    /// }; // Ends: if(((IRigidBody*)obj0)->isInWorld() && ((IRigidBody*)obj1)->isInWorld()){ }
    /// }; // Ends: Loop trought all contacts






    /// REMOVE balls, if...
    /// Create Maximum Life Time for all the current balls: (Remove at >=40000)
    for(unsigned int i=0; i<balls.size(); i++) {
    balls[i].LifeTime = balls[i].LifeTime += 0.01*frameDeltaTime;
    if(balls[i].LifeTime>=40000 && balls[i].Node && balls[i].Body && balls[i].Body->isInWorld()){
    audios->PlayFx(17, balls[i].Node->getPosition());
    animations->HitWave2(balls[i].Node->getPosition());
    BodyRemove(balls[i].Body, "hit");
    };
    /// cout << balls[i].LifeTime << endl;
    };


    /// REMOVE objects... These must be outside the previous main collisions check loop!

    if(obj0){
    if(((IRigidBody*)obj0)->isInWorld()){
    if(removeobj0 == true){ BodyRemove((IRigidBody*)obj0, "hit"); }; /// Body set to be removed?... Remove it.
    BodyRemove((IRigidBody*)obj0, "limits"); /// An IF with BodyRemove() MUST be always in the end !!!
    };
    };

    if(obj1){
    if(((IRigidBody*)obj1)->isInWorld()){
    if(removeobj1 == true){ BodyRemove((IRigidBody*)obj1, "hit"); }; /// Body set to be removed?... Remove it.
    BodyRemove((IRigidBody*)obj1, "limits"); /// An IF with BodyRemove() MUST be always in the end !!!
    };
    };



    info->getPointer()->clearManifold();  // <- is this the right way to clear & delete the info?
    delete info;
    info = NULL;
    };


};










void Level::HitBodyJump(IRigidBody* body, f32 force){

    if(body){ /// Body is NULL?
    if(body->isInWorld()){ /// Body exists?

    /// Reset all forces:
    body->clearForces();
    /// Apply jumping force:
    body->applyImpulse(core::vector3df(0,force,0), core::vector3df(0,1,0));

    };
    };
};



void Level::EveryoneJumps(){

    for(unsigned int i=0; i<soldiers.size(); i++){
    if(((IRigidBody*)soldiers[i].Body) && ((IRigidBody*)soldiers[i].Body)->isInWorld()){
    ((IRigidBody*)soldiers[i].Body)->applyImpulse(core::vector3df(0,4000,0), core::vector3df(0,1,0));
    };
    };

    for(unsigned int i=0; i<knights.size(); i++){
    if(((IRigidBody*)knights[i].Body) && ((IRigidBody*)knights[i].Body)->isInWorld()){
    ((IRigidBody*)knights[i].Body)->applyImpulse(core::vector3df(0,4000,0), core::vector3df(0,1,0));
    };
    };

    for(unsigned int i=0; i<kings.size(); i++){
    if(((IRigidBody*)kings[i].Body) && ((IRigidBody*)kings[i].Body)->isInWorld()){
    ((IRigidBody*)kings[i].Body)->applyImpulse(core::vector3df(0,4000,0), core::vector3df(0,1,0));
    };
    };

    for(unsigned int i=0; i<prisoners.size(); i++){
    if(((IRigidBody*)prisoners[i].Body) && ((IRigidBody*)prisoners[i].Body)->isInWorld()){
    ((IRigidBody*)prisoners[i].Body)->applyImpulse(core::vector3df(0,4000,0), core::vector3df(0,1,0));
    };
    };

    for(unsigned int i=0; i<workers.size(); i++){
    if(((IRigidBody*)workers[i].Body) && ((IRigidBody*)workers[i].Body)->isInWorld()){
    ((IRigidBody*)workers[i].Body)->applyImpulse(core::vector3df(0,1000,0), core::vector3df(0,1,0));
    };
    };

    for(unsigned int i=0; i<priestswalking.size(); i++){
    if(((IRigidBody*)priestswalking[i].Body) && ((IRigidBody*)priestswalking[i].Body)->isInWorld()){
    ((IRigidBody*)priestswalking[i].Body)->applyImpulse(core::vector3df(0,4000,0), core::vector3df(0,1,0));
    };
    };

    for(unsigned int i=0; i<platforms.size(); i++){
    if(((IRigidBody*)platforms[i].Body) && ((IRigidBody*)platforms[i].Body)->isInWorld()){
    ((IRigidBody*)platforms[i].Body)->applyImpulse(core::vector3df(0,6000,0), core::vector3df(0,1,0));
    };
    };

    for(unsigned int i=0; i<chickens.size(); i++){
    if(((IRigidBody*)chickens[i].Body) && ((IRigidBody*)chickens[i].Body)->isInWorld()){
    ((IRigidBody*)chickens[i].Body)->applyImpulse(core::vector3df(0,3000,0), core::vector3df(0,1,0));
    };
    };

    for(unsigned int i=0; i<priests.size(); i++){
    if(((IRigidBody*)priests[i].Body) && ((IRigidBody*)priests[i].Body)->isInWorld()){
    ((IRigidBody*)priests[i].Body)->applyImpulse(core::vector3df(0, rand()%4000+3000, 0), core::vector3df(0,1,0));
    };
    };

    for(unsigned int i=0; i<musicians.size(); i++) {
    if(((IRigidBody*)musicians[i].Body) && ((IRigidBody*)musicians[i].Body)->isInWorld()){
    ((IRigidBody*)musicians[i].Body)->applyImpulse(core::vector3df(0, 2000, 0), core::vector3df(0,1,0));
    };
    };

    for(unsigned int i=0; i<bulls.size(); i++) {
    if(((IRigidBody*)bulls[i].Body) && ((IRigidBody*)bulls[i].Body)->isInWorld()){
    ((IRigidBody*)bulls[i].Body)->applyImpulse(core::vector3df(0, 3000, 0), core::vector3df(0,1,0));
    };
    };
};



void Level::ReverseImpulse(IRigidBody* body){

    if(body){              /// Body exists? (Could have been removed, if not exists it crashes!)
    if(body->isInWorld()){ /// Body exists? (Could have been removed, if not exists it crashes!)

    f32 lvX = body->getLinearVelocity().X;
    f32 lvY = body->getLinearVelocity().Y;
    f32 lvZ = body->getLinearVelocity().Z;

    lvX = -(lvX/0.5); /// invert forces (and reduce it a little bit) only on this axis!

    /// Reset all forces:
    body->clearForces();
    /// Apply new ones:
    body->setLinearVelocity(vector3df(lvX,lvY,lvZ));

    };
    };

};


void Level::BodyRemove(IRigidBody* inBody, std::string why){





    /// 1) REMOVE THIS BODY - BEING HIT:
    if(why == "hit"){
    if(inBody){ /// Body is NULL?...
    if(inBody->isInWorld()){ /// Body exists?...

    /// Effects:
    // vector3df pos = inBody->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)
    // if(pos.Y<=0){ effects->HitWater(pos);   audios->PlayFx(16, pos); };
    // if(pos.Y> 0){ effects->HitWave1(inBody); audios->PlayFx(17, pos); };

    int inID = inBody->getUniqueID();



    /// ALLWAYS search and remove bodies from its own vector array:
    if(inBody->getAttributes()->existsAttribute("ball") && !balls.empty()) {
    for(unsigned int i=0; i<balls.size(); i++) {
    if(inID == balls[i].BodyID){ balls.erase(balls.begin()+i); std::cout << "HIT: Removed ball ID: " << inID << std::endl; };
    };
    };

    /// ALLWAYS search and remove bodies from its own vector array:
    if(inBody->getAttributes()->existsAttribute("soldier") && !soldiers.empty()) {
    for(unsigned int i=0; i<soldiers.size(); i++) {
    if(inID == soldiers[i].BodyID){ soldiers.erase(soldiers.begin()+i); std::cout << "HIT: Removed soldier ID: " << inID << std::endl; };
    };
    };

    /// ALLWAYS search and remove bodies from its own vector array:
    if(inBody->getAttributes()->existsAttribute("box") && !boxes.empty()) {
    for(unsigned int i=0; i<boxes.size(); i++) {
    if(inID == boxes[i].BodyID){ boxes.erase(boxes.begin()+i); std::cout << "HIT: Removed box ID: " << inID << std::endl; };
    };
    };

    /// ALLWAYS search and remove bodies from its own vector array:
    if(inBody->getAttributes()->existsAttribute("cargo") && !cargos.empty()) {
    for(unsigned int i=0; i<cargos.size(); i++) {
    if(inID == cargos[i].BodyID){ cargos.erase(cargos.begin()+i); std::cout << "HIT: Removed cargo ID: " << inID << std::endl; };
    };
    };

    /// ALLWAYS search and remove bodies from its own vector array:
    if(inBody->getAttributes()->existsAttribute("target") && !targets.empty()) {
    for(unsigned int i=0; i<targets.size(); i++) {
    if(inID == targets[i].BodyID){ targets.erase(targets.begin()+i); std::cout << "HIT: Removed target ID: " << inID << std::endl; };
    };
    };

    /*
    /// ALLWAYS search and remove bodies from its own vector array:
    /// Remove both Jail and Prisoner:
    if(inBody->getAttributes()->existsAttribute("jail") && !jails.empty() && !prisoners.empty()) {
    for(unsigned int i=0; i<jails.size() && i<prisoners.size(); i++) {
    if(inID == jails[i].BodyID) {
    world->removeCollisionObject(jails[i].Body);     /// 1) Remove from bullet
    jails.erase(jails.begin()+i);                    /// 2) Remove from vector
    world->removeCollisionObject(prisoners[i].Body); /// 1) Remove from bullet
    prisoners.erase(prisoners.begin()+i);            /// 2) Remove from vector
    };
    };
    };
    */

    /// ONLY AFTER we are done, we remove the body from bullet:
    world->removeCollisionObject(world->getCollisionObjectByID(inID)); /// Remove from bullet
    world->removeCollisionObject(inBody); /// Remove from bullet
    /// if(boxes[i].Node){ boxes[i].Node->remove(); }; /// Remove from irrlicht (Not needed. Node already removed by irrBullet)
    std::cout << "HIT: Removed body ID: " << inID << std::endl;


    };
    };
    };


    /// REMOVE ALL BODIES BEYOUND LIMITS:
    if(why == "limits"){
    if(inBody){ /// Body is NULL?...
    if(inBody->isInWorld()){ /// Body exists?...

    bool beyondLimits = false;

    vector3df pos = inBody->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)
    if(pos.Y<0 || pos.Y>10000 || pos.X<-10000 || pos.X>10000){ beyondLimits = true; };

    if(beyondLimits == true){

    /// Effects:
    if(pos.Y<=0){ effects->HitWater(pos);   audios->PlayFx(16, pos); };
    if(pos.Y> 0){ effects->HitWave1(inBody); audios->PlayFx(17, pos); };

    u32 inID = inBody->getUniqueID();
    // effects->HitWater(pos); /// Particles: Send body position
    // effects->HitWave1(pos);
    // audios->PlayFx(8, pos);

    /// ALLWAYS search and remove bodies from its own vector array:
    if(inBody->getAttributes()->existsAttribute("ball") && !balls.empty()) {
    for(unsigned int i=0; i<balls.size(); i++) {
    if(inID == balls[i].BodyID){ balls.erase(balls.begin()+i); std::cout << "LIMITS: Removed ball ID: " << inID << std::endl; };
    };
    };

    /// ALLWAYS search and remove bodies from its own vector array:
    if(inBody->getAttributes()->existsAttribute("soldier") && !soldiers.empty()) {
    for(unsigned int i=0; i<soldiers.size(); i++) {
    if(inID == soldiers[i].BodyID){ soldiers.erase(soldiers.begin()+i); std::cout << "LIMITS: Removed soldier ID: " << inID << std::endl; };
    };
    };

    /// ALLWAYS search and remove bodies from its own vector array:
    if(inBody->getAttributes()->existsAttribute("box") && !boxes.empty()) {
    for(unsigned int i=0; i<boxes.size(); i++) {
    if(inID == boxes[i].BodyID){ boxes.erase(boxes.begin()+i); std::cout << "LIMITS: Removed box ID: " << inID << std::endl; };
    };
    };

    /// ALLWAYS search and remove bodies from its own vector array:
    if(inBody->getAttributes()->existsAttribute("cargo") && !cargos.empty()) {
    for(unsigned int i=0; i<cargos.size(); i++) {
    if(inID == cargos[i].BodyID){ cargos.erase(cargos.begin()+i); std::cout << "LIMITS: Removed cargo ID: " << inID << std::endl; };
    };
    };

    /// ONLY AFTER we are done, we remove the body from bullet:
    world->removeCollisionObject(world->getCollisionObjectByID(inID)); /// Remove from bullet
    world->removeCollisionObject(inBody); /// Remove from bullet
    /// if(boxes[i].Node){ boxes[i].Node->remove(); }; /// Remove from irrlicht (Not needed. Node already removed by irrBullet)
    std::cout << "LIMITS: Removed body ID: " << inID << std::endl;

    };

    };
    };
    };



};



/// This one REMOVES ALL balls:
void Level::RemoveBalls(){

    /// for(unsigned int i=0; i<balls.size(); i++){ BodyRemove(balls[i].Body, "hit"); };

    /// 1st REMOVE:
    for(unsigned int i=0; i<balls.size(); i++){
    if(balls[i].Body && balls[i].Body->isInWorld()){
    world->removeCollisionObject(balls[i].Body);
    }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };
    /// Reset:
    balls.clear();

};



void Level::CreateBall(const f32 cannon_power){

	if(nodeCannon){

    /// Limits:
    if(MAX_BALLS<=1 ){ MAX_BALLS = 1;  };
    if(MAX_BALLS>=10){ MAX_BALLS = 10; };

	/// Removes ONLY the EXTRA balls:
    if(balls.size()>=MAX_BALLS){
    /// 1) Search and remove the oldest ball from physics world (get and remove it by id):
    if(((IRigidBody*)balls[0].Body)->isInWorld()){ /// Voids if body have been removed = crashes!
    int id = balls[0].BodyID;
    if(world->getCollisionObjectByID(id)){
    /// ALLWAYS search and remove bodies from its own vector array:
    if(!balls.empty()) {
    for(unsigned int i=0; i<balls.size(); i++) {
    if(id == balls[i].BodyID){
    /// Wave: (on the ball that will be removed)
    vector3df pos = balls[i].Body->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)
    animations->HitWave3(pos);
    /// Remove body
    world->removeCollisionObject(world->getCollisionObjectByID(id));
    world->removeCollisionObject(balls[i].Body); /// Remove from bullet
    balls.erase(balls.begin()+i);
    };
    };
    };
    };
    };
    };


    /// Get positions and rotations:
    /// vector3df cannonJointPos = nodeCannon->getJointNode("joint_01")->getPosition(); // Get joint position
    vector3df cannonPos = nodeCannon->getPosition(); // position
    vector3df cannonRot = nodeCannon->getRotation(); // rotation


	/// Create Node:
	scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("ball_01.X"));
	node->setParent(nodeLevel); /// Attach to parent: Level
    node->setName("ball");
	node->setPosition(vector3df(cannonPos.X+5,cannonPos.Y-5,cannonPos.Z));
	node->setRotation(cannonRot);
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_SOLID);
    // node->setMaterialType(video::EMT_REFLECTION_2_LAYER);    // EMT_REFLECTION_2_LAYER
    /// Set textures:
    /// Apply texture to my specified material
    for(int m=0; m<node->getMaterialCount(); m++){
    node->getMaterial(m).Shininess = 15.0f; // Set size of specular highlights
	node->getMaterial(m).SpecularColor.set(0,100,100,100);
	node->getMaterial(m).DiffuseColor.set(0,0,0,0);
	node->getMaterial(m).EmissiveColor.set(0,0,0,0);
	node->getMaterial(m).ColorMaterial = video::ECM_DIFFUSE; /// Default is: video::ECM_DIFFUSE
	};

    // IAttributes* attribs = device->getFileSystem()->createEmptyAttributes();
    // node->serializeAttributes(attribs);
    // core::stringc name = attribs->getAttributeAsString("ball");
    // attribs->drop();

    /// Create Shape:
    f32 mass = 5.0f;
    // IGImpactMeshShape* wShape1 = new IGImpactMeshShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); /// IConvexHullShape* wShape1 = new IConvexHullShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0); /// IBoxShape* wShape1 = new IBoxShape(rodaNode1, 10.0, true);
	ICollisionShape* shape = new ISphereShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	shape->calculateLocalInertia(mass, vector3df(0,0,0));
	/// Create Body:
	IRigidBody* body = world->addRigidBody(shape, COL_BALLS, ballsCollidesWith);
	body->setFriction(0.05f);
	body->setDamping(0.0010f, 0.0010f); /// body->setDamping(linear, angular); == Atmosphere resistance
	body->setRestitution(0.85f); // bouncing factor
	/// body->setCcdValues(0.01,0.01); /// <-- Continuous Collision Detection (void tunneling: motion, radius) - Only needed on fast objects!
	/// body->setCcdValues(0.001,0.001); // Continuous Collision Detection (void tunneling: motion, radius) - Only needed on fast objects!
	/// body->getPointer()->setCcdMotionThreshold(1.1f);   // CCD = Continuous Collision Detection - Only needed on fast objects!
	/// body->getPointer()->setCcdSweptSphereRadius(1.1f); // CCD = Continuous Collision Detection - Only needed on fast objects!
		// body->getPointer()->setDeactivationTime(0);
	// body->getPointer()->setSleepingThresholds(5.4, 5.6);
	// body->getPointer()->setContactProcessingThreshold(0.25f);
    body->getPointer()->setContactProcessingThreshold(btScalar(-5.0)); /// <-- For MUCH better collision detection:
	// body->getPointer()->setCcdMotionThreshold(btScalar(0.0));
	// body->getPointer()->setCcdSweptSphereRadius(btScalar(0.0));
	// body->setName("ball");
	body->getAttributes()->addBool("ball", true);
	// body->getPointer()->setIslandTag(0);
	// body->getAttributes()->addString("ball", "ball");
	// body->getAttributes()->addBool("collide", true);
	// body->getAttributes()->addUserPointer("ball", 0);
	// body->getPointer()->setUserIndex(99);
	body->setLinearFactor(vector3df(1,1,0));  // Allowed positions axis
    body->setAngularFactor(vector3df(1,1,1)); // Allowed rotations axis
    /// body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->getPointer()->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT); // btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_KINEMATIC_OBJECT
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States



    /// Distribute power, according rotation, for the X axis only:
    btVector3 relativeForce = btVector3(cannon_power,0,0);
    btTransform boxTrans;
    body->getPointer()->getMotionState()->getWorldTransform(boxTrans);
    btVector3 correctedForce = (boxTrans*relativeForce)-boxTrans.getOrigin();

    /// Shoot Ball:
    body->getPointer()->applyCentralImpulse(correctedForce); // OK

    /// Attach Trail particles:
    effects->BallTrail(node);


    /// UPDATES all previous transforms (by nodes animations, etc) and updates coordinates relative to the physic world:
    // btVector3 aabbmin0, aabbmax0;
    // body->getPointer()->getCollisionShape()->getAabb(body->getPointer()->getWorldTransform(), aabbmin0, aabbmax0);  // extracts the object's min/max axis aligned bounding box world coordinates
    // world->getPointer()->getBroadphase()->setAabb(body->getPointer()->getBroadphaseHandle(), aabbmin0, aabbmax0, 0); // sets the proxy's aabb and updates the tree

    /// Object:
    Ball b;
    b.Node     = node;
    b.Body     = body;
    b.BodyID   = body->getUniqueID();
    b.LifeTime = 0.0;
    /// Add to object vector:
    balls.push_back(b);

    /// Camera will follow:
    ballToFollowID = body->getUniqueID();
    cameraGoToOrigin = false;

	};


};








void Level::LightSetPosition() {
    /*
    vector3df pos = cameras[0]->getPosition();
    /// Apply position to camera light:
    nodeCameraLight->setPosition(vector3df(pos.X, pos.Y+200, -1500));
    */
};



void Level::CameraSetTargetAndPosition() {


    f32 posBallX = cameraDefaultPosition.X;
    f32 posBallY = cameraDefaultPosition.Y;

    f32 posCamX = cameras[0]->getPosition().X;
    f32 posCamY = cameras[0]->getPosition().Y;
    f32 posCamZ = cameras[0]->getPosition().Z;



    /// Camera very far. Approaching -------------------------------------------------------------

    /// GAME STARTING: Cinematic camera only...
    static bool loopIsDone = false;
    if(USER_PRESSED_START == true && loopIsDone==false){
    posCamX -= 3.00*frameDeltaTime;
    posCamY -= 3.00*frameDeltaTime;
    posCamZ += 3.00*frameDeltaTime;

    /// Can press ESC to EXIT Game?...
    CAN_EXIT = false;

    /// Cinematic camera on target! Game starts now!...
    if(posCamZ >= minCamZoom) {
        /// Start play!...
        if(VIEW_TEXTS==true){ textInfos1->setVisible(true); };
        textTotalScore->setVisible(true);
        textAngle->setVisible(true);
        if(NAMES_TEXTS==true){ for(unsigned int i=0; i<personageName.size(); i++){ personageName[i]->setVisible(true); }; };
        musiciansAnimate = false;
        if(BACKGROUND_MUSIC==true){ audios->PlayMusic(1); musiciansAnimate = true; }; /// Music: Main Theme
        audios->PlayFx(6, cameras[0]->getPosition()); /// Sound: zoom
        LEVEL_READY = true;
        loopIsDone = true;
    } else {
        /// Doing camera yet!...
        LEVEL_READY = false;
        loopIsDone = false;
    };

    /// X: Limits on the camera, must be set after:
    if(posCamX <= maxCamLeft ){ posCamX = maxCamLeft;  };
    if(posCamX >= maxCamRight){ posCamX = maxCamRight; };
    /// Y: Limits on the camera, must be set after:
    if(posCamY <= maxCamBot  ){ posCamY = maxCamBot;   };
    if(posCamY >= maxCamTop  ){ posCamY = maxCamTop;   };
    /// Z: Limits on the camera, must be set after:
    if(posCamZ >= minCamZoom ){ posCamZ = minCamZoom;  };
    /// if(posCamZ <= maxZoom){ posCamZ = maxZoom; }; /// DISABLED: Needed for doing intro camera zoom in

    /// Apply new camera position and target position:
    cameras[0]->setPosition(vector3df(posCamX, posCamY, posCamZ));
    cameras[0]->setTarget(vector3df(posCamX, posCamY, cameraDefaultTarget.Z));
    };

    if(LEVEL_READY == false && loopIsDone==false){
    /// X: Limits on the camera, must be set after:
    if(posCamX <= maxCamLeft ){ posCamX = maxCamLeft;  };
    if(posCamX >= maxCamRight){ posCamX = maxCamRight; };
    /// Y: Limits on the camera, must be set after:
    if(posCamY <= maxCamBot  ){ posCamY = maxCamBot;   };
    if(posCamY >= maxCamTop  ){ posCamY = maxCamTop;   };
    /// Z: Limits on the camera, must be set after:
    if(posCamZ >= minCamZoom ){ posCamZ = minCamZoom;  };
    /// if(posCamZ <= maxZoom){ posCamZ = maxZoom; }; /// DISABLED: Needed for doing intro camera zoom in

    /// Apply new camera position and target position:
    cameras[0]->setPosition(vector3df(posCamX, posCamY, posCamZ));
    cameras[0]->setTarget(vector3df(posCamX, posCamY, cameraDefaultTarget.Z));
    };


    /// Camera very far. Approaching -------------------------------------------------------------




    /// Set camera to very far -------------------------------------------------------------------

    /// GAME OVER: Cinematic camera only...
    // static bool loopIsDone = false;
    if(USER_PRESSED_START == false){
    posCamX -= 3.00*frameDeltaTime;
    posCamY -= 3.00*frameDeltaTime;
    posCamZ -= 3.00*frameDeltaTime;

    /// Cinematic camera set it far again!...
    if(posCamZ <= initCamZoom) {
        /// To enable again the previous camera function:
        loopIsDone = false;
        /// Stop play!...
        textInfos1->setVisible(false);
        textTotalScore->setVisible(false);
        textAngle->setVisible(false);
        if(NAMES_TEXTS==true){ for(unsigned int i=0; i<personageName.size(); i++){ personageName[i]->setVisible(false); }; };
        musiciansAnimate = false;
        audios->StopMusic(1); /// Music: Main Theme
        /// Reset:
        PLAYER_TOTAL_SCORE = 0;
        PLAYER_LEVEL       = 0;
        PLAYER_NAME        = L"";
        /// Balls:
        cameraGoToOrigin = true;
        ballToFollowID = 0;
        /// Can press ESC to EXIT Game?...
        CAN_EXIT = true;
    };

    /// X: Limits on the camera, must be set after:
    if(posCamX <= maxCamLeft ){ posCamX = maxCamLeft;  };
    if(posCamX >= maxCamRight){ posCamX = maxCamRight; };
    /// Y: Limits on the camera, must be set after:
    if(posCamY <= maxCamBot  ){ posCamY = maxCamBot;   };
    if(posCamY >= maxCamTop  ){ posCamY = maxCamTop;   };
    /// Z: Limits on the camera, must be set after:
    if(posCamZ >= minCamZoom ){ posCamZ = minCamZoom;  };
    if(posCamZ <= initCamZoom){ posCamZ = initCamZoom; };

    /// Apply new camera position and target position:
    cameras[0]->setPosition(vector3df(posCamX, posCamY, posCamZ));
    cameras[0]->setTarget(vector3df(posCamX, posCamY, cameraDefaultTarget.Z));
    };

    /// Set camera to very far -------------------------------------------------------------------






    /// GAME PLAYING: Auto camera...
    if(LEVEL_READY == true){

    /// Camera follows ball body ---------------------------------------------------

    /// Option 1: (Functions must be in this order) If SPACE KEY is pressed avoid next functions:
    if(cameraGoToOrigin == false){

    /// Option 2: (Functions must be in this order) Stop the camera position where the ball was before being removed
    posBallX = cameras[0]->getPosition().X; /// Keep same X
    posBallY = cameraDefaultPosition.Y; /// Makes camera going down on Y

    /// Option 3: (Functions must be in this order) Get the position of the last followed ball:
    bool foundBall = false;
    for(unsigned int i=0; i<balls.size(); i++){
    if(ballToFollowID == balls[i].BodyID){
    btTransform trans = balls[i].Body->getPointer()->getWorldTransform();
    posBallX = trans.getOrigin().getX(); // /5.0f; /// divide by 5 == smooth camera
    posBallY = trans.getOrigin().getY(); // /5.0f; /// divide by 5 == smooth camera
    foundBall = true;
    };
    };

    /// Option 4: (Functions must be in this order) Get the position of a previous (now the last) ball:
    if(foundBall == false){
    for(unsigned int i=0; i<balls.size(); i++){
    btTransform trans = balls[i].Body->getPointer()->getWorldTransform();
    posBallX = trans.getOrigin().getX(); // /5.0f; /// divide by 5 == smooth camera
    posBallY = trans.getOrigin().getY(); // /5.0f; /// divide by 5 == smooth camera
    };
    };

    };

    /// Camera follows ball body ---------------------------------------------------




    /// Camera follow prisoner body -----------------------------------------------

    if(!prisoners.empty()){
    for(unsigned int i=0; i<prisoners.size(); i++) {
    if(prisoners[i].Body && prisoners[i].Body->isInWorld() && prisoners[i].Free == true){
    btTransform trans = prisoners[i].Body->getPointer()->getWorldTransform();
    posBallX = trans.getOrigin().getX(); // /5.0f; /// divide by 5 == smooth camera
    posBallY = trans.getOrigin().getY(); // /5.0f; /// divide by 5 == smooth camera
    };
    };
    };

    /// Camera follow prisoner body -----------------------------------------------




    /// Camera follow shark 2 node ------------------------------------------------

    if(nodeShark2 && cameraFollowShark2==true){
    posBallX = nodeShark2->getPosition().X;
    posBallY = nodeShark2->getPosition().Y;
    };

    /// Camera follow shark 2 node ------------------------------------------------




    /// Camera follow king body ---------------------------------------------------

    if(kingOrder.size()>0 || GAME_OVER == true){
    if(kings[0].Body && kings[0].Body->isInWorld()){
    btTransform trans = kings[0].Body->getPointer()->getWorldTransform();
    posBallX = trans.getOrigin().getX(); // /5.0f; /// divide by 5 == smooth camera
    posBallY = trans.getOrigin().getY(); // /5.0f; /// divide by 5 == smooth camera
    };
    };

    /// Camera follow king body ---------------------------------------------------




    /// User is moving camera?
    if(receiver.IsKeyDown(irr::KEY_LEFT)){
    userIsMovingCamera = true; userGoingLeft = true; userCameraSpeedX = 1.25;
    };
    if(receiver.IsKeyDown(irr::KEY_RIGHT)){
    userIsMovingCamera = true; userGoingLeft = false; userCameraSpeedX = 1.25;
    };
     if(receiver.IsKeyDown(irr::KEY_UP)){
    userIsMovingCamera = true; userGoingUp = true; userCameraSpeedY = 0.25;
    posCamZ -= (zoomCamSpeed)*frameDeltaTime; /// Zoom out
    };
    if(receiver.IsKeyDown(irr::KEY_DOWN)){
    userIsMovingCamera = true; userGoingUp = false; userCameraSpeedY = 0.25;
    posCamZ += (zoomCamSpeed)*frameDeltaTime; /// Zoom in
    };
    /// Keep constantly the camera Y to bottom - and - smooth X movements:
    if(userIsMovingCamera==true){
    if(userGoingLeft == true ){ posCamX -= userCameraSpeedX*frameDeltaTime; };
    if(userGoingLeft == false){ posCamX += userCameraSpeedX*frameDeltaTime; };
    if(userGoingUp   == true ){ posCamY += userCameraSpeedY*frameDeltaTime; };
    if(userGoingUp   == false){ posCamY -= userCameraSpeedY*frameDeltaTime; };
    userCameraSpeedX-=0.120; if(userCameraSpeedX<=0.0){ userCameraSpeedX = 0.0; };
    userCameraSpeedY-=0.060; if(userCameraSpeedY<=0.0){ userCameraSpeedY = 0.0; };
    };




    /// IF user is NOT moving camera:
    if(receiver.IsKeyUp(irr::KEY_LEFT) && receiver.IsKeyUp(irr::KEY_RIGHT) && receiver.IsKeyUp(irr::KEY_UP) && receiver.IsKeyUp(irr::KEY_DOWN) && userIsMovingCamera == false){


    /// --- ALL for X axis only: --- OK!

    /// Limits on the ball must be placed before:
    if(posBallX <= maxCamLeft ){ posBallX = maxCamLeft;  };
    if(posBallX >= maxCamRight){ posBallX = maxCamRight; };

    f32 speedX = (fabsf((posCamX-posBallX)/300.0)); /// Speed up or down, according distance (fabsf == floats will be always positive)
    if(speedX <= 0.10){ speedX = 0.10; }; /// Minimum X speed allowed

    if(posBallX> posCamX){ posCamX += speedX*frameDeltaTime; };
    if(posBallX==posCamX){ posCamX = posBallX; };
    if(posBallX< posCamX){ posCamX -= speedX*frameDeltaTime; };

    /// --- ALL for Z axis only: --- OK!

    /// Zooms:
    if(posBallY >= maxCamTop){ posCamZ -= zoomCamSpeed*frameDeltaTime; }; /// Zoom out
    if(posBallY <  maxCamTop){ posCamZ += zoomCamSpeed*frameDeltaTime; }; /// Zoom in

    /// --- ALL for Y axis only: --- OK!

    /// Limits on the ball must be placed before:
    if(posBallY <= maxCamBot){ posBallY = maxCamBot; };
    if(posBallY >= maxCamTop){ posBallY = maxCamTop; };

    f32 speedY = (fabsf((posCamY-posBallY)/1800.0)); /// Speed up or down, according distance (fabsf == floats will be always positive)
    if(speedY <= 0.10){ speedY = 0.10; }; /// Minimum Y speed allowed

    if(posBallY> posCamY){ posCamY += speedY*frameDeltaTime; };
    if(posBallY==posCamY){ posCamY = posBallY; };
    if(posBallY< posCamY){ posCamY -= speedY*frameDeltaTime; };

    };


    /// LIMIT and apply all calculated above values now...

    /// X: Limits on the camera, must be set after:
    if(posCamX <= maxCamLeft ){ posCamX = maxCamLeft;  };
    if(posCamX >= maxCamRight){ posCamX = maxCamRight; };
    /// Y: Limits on the camera, must be set after:
    if(posCamY <= maxCamBot  ){ posCamY = maxCamBot;   };
    if(posCamY >= maxCamTop  ){ posCamY = maxCamTop;   };
    /// Z: Limits on the camera, must be set after:
    if(posCamZ >= minCamZoom ){ posCamZ = minCamZoom;  };
    if(posCamZ <= maxCamZoom ){ posCamZ = maxCamZoom;  };

    /// User can fire again, only if its near the ship:
    int distance = abs(posCamX-cameraDefaultPosition.X);
    if(distance<=300){ cameraIsNearShip = true; } else { cameraIsNearShip = false; };

    /// Apply new camera position and target position:
    cameras[0]->setPosition(vector3df(posCamX, posCamY, posCamZ));
    cameras[0]->setTarget(vector3df(posCamX, posCamY, cameraDefaultTarget.Z));

    }; /// Ends: USER_INPUT


};


void Level::CameraShake(){

    /// To call this function: shakeCamera = 0.0;

    if(shakeCamera<=0.0){ camShakeOnY = 12.0; }; /// camShakeOnY = Camera vibration factor

    if(shakeCamera<1000.0){
    f32 PosY = cameras[0]->getPosition().Y;
    cameraShakingTime+=0.20*frameDeltaTime;

    if(cameraShakingTime<=5.0){
    PosY+=camShakeOnY;
    if(PosY>=maxCamTop){ PosY = maxCamTop; };
    cameras[0]->setPosition(vector3df(cameras[0]->getPosition().X, PosY, cameras[0]->getPosition().Z));
    };
    if(cameraShakingTime>5.0 && cameraShakingTime<=10.0){
    PosY-=camShakeOnY;
    if(PosY<=maxCamBot){ PosY = maxCamBot; };
    cameras[0]->setPosition(vector3df(cameras[0]->getPosition().X, PosY, cameras[0]->getPosition().Z));
    };
    if(cameraShakingTime>10.0){
    cameraShakingTime = 0.0;
    };
    shakeCamera+=2.00*frameDeltaTime; if(shakeCamera>=1000){ shakeCamera=1000; };
    camShakeOnY-=0.10*frameDeltaTime; if(camShakeOnY<=0){ camShakeOnY=0; }; /// Continuous vibration reducer
    };

};






void Level::DrawLines(){
    /*
    static video::SMaterial mtl;
    core::line3df line;
    line.start = nodeSoldier[0]->getAbsolutePosition();
    line.end = nodeSoldier[1]->getAbsolutePosition();
    mtl.Lighting = false;
    driver->setMaterial(mtl);
    core::matrix4 mat;
    driver->setTransform(video::ETS_WORLD, mat);
    driver->draw3DLine(line.start, line.end, video::SColor(255,255,0,0));
    */
};







// std::vector<vector3df> points;
std::vector<btVector3> points;

void Level::DrawTrajectoryLines(){

    /*
    f32 posBallX = cameraDefaultPosition.X;
    f32 posBallY = cameraDefaultPosition.Y;

    /// Get the ball to follow... IF exists:
    if(bodyBallToFollow!=0){ /// Using vector this way, for a body, never crashes! Even if a body do not exists!
    if(bodyBallToFollow){
    if(bodyBallToFollow->isInWorld()){


    /// Get ball position:
    /// btTransform trans = bodyBallToFollow[0]->getPointer()->getWorldTransform();
    /// posBallX = trans.getOrigin().getX(); // /5.0f; /// divide by 5 == smooth camera
    /// posBallY = trans.getOrigin().getY(); // /5.0f; /// divide by 5 == smooth camera


    /// static video::SMaterial mtl;
    /// core::line3df line;
    /// line.start = nodeSoldier[0]->getAbsolutePosition();
    /// line.end = nodeSoldier[1]->getAbsolutePosition();
    /// mtl.Lighting = false;
    /// driver->setMaterial(mtl);
    /// core::matrix4 mat;
    /// driver->setTransform(video::ETS_WORLD, mat);
    /// driver->draw3DLine(line.start, line.end, video::SColor(255,255,0,0));


    /// https://answers.unity.com/questions/606720/drawing-projectile-trajectory.html

    int verts = 50;
    points.clear();

    /// world->stepSimulation(frameDeltaTime*0.0100f, 60, f32(1.)/f64(60.));

    btTransform trans = ((btRigidBody*)bodyBallToFollow)->getWorldTransform();
    btVector3 pos = trans.getOrigin();
    btVector3 vel = ((btRigidBody*)bodyBallToFollow)->getLinearVelocity();
    btVector3 gravity = btVector3(0, 0, -5); // world->getPointer()->getGravity();


    vector3df cannonPos = nodeCannon->getPosition(); // position
    vector3df cannonRot = nodeCannon->getRotation(); // rotation

    points.push_back(irrlichtToBulletVector(cannonPos));

    for(unsigned int i=0; i<verts; i++) {
    vel += gravity * (frameDeltaTime*0.0100f);
    pos += vel * (frameDeltaTime*0.0100f);
    points.push_back(btVector3(pos.getX(), pos.getY(), -200.0));
    };


    // line.SetPosition(i, Vector3(pos.x, pos.y, 0));
    // driver->draw3DLine(line.start, line.end, video::SColor(255,255,0,0));

    };
    };
    };

    points.push_back(btVector3(100, 300, -200.0));

    // points.push_back(vector3df(0,-100,-200));
    points.push_back(vector3df(50,200,0));
    points.push_back(vector3df(100,300,0));
    points.push_back(vector3df(500,500,0));
    points.push_back(vector3df(800,1000,0));
    points.push_back(vector3df(1000,400,0));

    cout<< points.size() << endl;


    static video::SMaterial mtlb;
    mtlb.Lighting = false;
    driver->setMaterial(mtlb);
    core::matrix4 matb;
    driver->setTransform(video::ETS_WORLD, matb);

    for(unsigned int i=0; i<points.size()-1; i++){
    driver->draw3DLine(bulletToIrrlichtVector(points[i]), bulletToIrrlichtVector(points[i+1]), video::SColor(255,0,255,0));
    };
    */

};










