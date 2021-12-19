/// This class is part of Game class.
/// So depends of objects and variables already created in the Game class.



class Effects : public Game {

    public:

    Effects();  // Constructor
    ~Effects(); // Destructor

    void Reset();
    void Initialize();
    void CreateEffects();
    void RemoveEffects();
    void Loop();

    /// TOOL: Convert 3D positions to 2D positions:
    position2d<s32> Convert3DTo2DPositions(vector3df pos3D);

    void BallTrail(scene::ISceneNode* node);
    void WaterTrail(scene::ISceneNode* node);
    // void cannonFiring(scene::ISceneNode* node);
	void CannonFiring(scene::ISceneNode* node);
	bool CannonCanFireAgain();
    void CannonLoop();

    void HitWave1(IRigidBody* body);
    void HitWave1Loop();

    void HitScore(vector3df pos, int score, stringw color);
    void HitScoreLoop();

    void HitHasBonus(IRigidBody* body);
    void HitHasBonusLoop();

    int HitJail(IRigidBody* body, int remove_energy);
    void HitJail2DEnergyBarLoop();

    void HitTarget(ISceneNode* node);
    void HitTargetLoop();

    void HitSoldier(IRigidBody* body);
    void HitSoldierLoop();

    void HitBox(vector3df pos);
    void HitBoxLoop();

    void HitWater(vector3df pos);
    void HitWaterLoop();

    void SoldiersBalloon(IRigidBody* body);
    void SoldiersBalloonLooping();

    void TotalScoreAdd(int add_score);

    private:

    scene::IParticleSystemSceneNode* ps666  = 0;  // Particles
    scene::IParticleEmitter* em666          = 0;  // Particles

    f32 cannonDuration       = 0.0;
    f32 canFireAgain         = 0.0;
    bool cannonAbleToFire    = true;
    scene::IParticleSystemSceneNode* cannonPNode = 0; // Cannon particles node

    f32 hitTargetDuration       = 0.0;
    f32 hitSoldierDuration      = 0.0;
    f32 hitBoxDuration          = 0.0;
    f32 hitWaterDuration        = 0.0;

    /// Remove energy:
    int energyLeft = 0;
    f32 hitRemoveEnergyDuration = 0.0;
    vector3df energyBarPosition = vector3df(0.0,0.0,0.0);
    int energyBarAlpha1 = 0;
    int energyBarAlpha2 = 0;
    ITexture* energy_Frame = 0;
    ITexture* energy_Left  = 0;

    /// Hit Waves 1:
    core::array<video::ITexture*> hitWaves1Textures;
    vector3df hitWave1Position = vector3df(0.0,0.0,0.0);
    bool hitDoWave1Loop = false;
    unsigned int hitWave1TextNum = 0;
    f32 hitWave1TextureTimer = 0.0;

    /// Balloons:
    IRigidBody* bodySoldierForBalloons  = 0;
    std::vector<ITexture*> soldiersBalloonTextures;  /// Balloons for soldiers only
    std::vector<ITexture*> priestsBalloonTextures;   /// Balloons for priests only

    /// Text Balloon:
    vector3df balloonSolPosition; /// handles position, even if its body was removed
    bool doSolBalloon        = false;
    f32 balloonSolDuration   = 0.0;
    int randomSolBalloon     = 0;
    f32 newRandSolBalloon    = 0.0;
    bool doNewRandSolBalloon = true;


    f32 scorePosY   = 0.0;
    int scoreColorA = 255;
    int scoreColorR = 255;
    int scoreColorG = 255;
    int scoreColorB = 255;

    f32 bonusPosY   = 0.0;
    int bonusColorA = 255;
    int bonusColorR = 255;
    int bonusColorG = 255;
    int bonusColorB = 255;



    /// Only a pointer to other classes already included:
    // Scene* scene;

};

Effects::Effects(){};  // Constructor (Can accept arguments, but this one does not)
Effects::~Effects(){}; // Destructor (do not accept arguments)





void Effects::Reset(){

    /// Reset:
    energy_Frame = 0;
    energy_Left  = 0;

    /// Reset:
    soldiersBalloonTextures.clear();

    /// Reset:
    priestsBalloonTextures.clear();

    /// Reset:
    hitWaves1Textures.clear();

};



void Effects::Initialize(){

    /// Reset:
    Reset(); cout << "Effects->Reset() OK" << endl;

    /// Fill:
    energy_Frame = driver->getTexture("gui_power_frame.png");
    energy_Left  = driver->getTexture("gui_power_force.png");

    /// Fill:
    soldiersBalloonTextures.push_back(driver->getTexture("balloon_soldier_01.png"));
    soldiersBalloonTextures.push_back(driver->getTexture("balloon_soldier_02.png"));
    soldiersBalloonTextures.push_back(driver->getTexture("balloon_soldier_03.png"));
    soldiersBalloonTextures.push_back(driver->getTexture("balloon_soldier_04.png"));
    soldiersBalloonTextures.push_back(driver->getTexture("balloon_soldier_05.png"));
    soldiersBalloonTextures.push_back(driver->getTexture("balloon_soldier_06.png"));

    /// Fill:
    priestsBalloonTextures.push_back(driver->getTexture("balloon_soldier_01.png"));
    priestsBalloonTextures.push_back(driver->getTexture("balloon_soldier_02.png"));
    priestsBalloonTextures.push_back(driver->getTexture("balloon_soldier_03.png"));

    /// Fill:
    hitWaves1Textures.push_back(driver->getTexture("hit_waves_01.png"));
    hitWaves1Textures.push_back(driver->getTexture("hit_waves_02.png"));
    hitWaves1Textures.push_back(driver->getTexture("hit_waves_03.png"));
    hitWaves1Textures.push_back(driver->getTexture("hit_waves_04.png"));
    hitWaves1Textures.push_back(driver->getTexture("hit_waves_05.png"));
    hitWaves1Textures.push_back(driver->getTexture("hit_waves_06.png"));
    hitWaves1Textures.push_back(driver->getTexture("hit_waves_07.png"));

};


void Effects::BallTrail(scene::ISceneNode* node){

    if(node){
    /// Create particles - Trails for the cannon balls:
    scene::IParticleSystemSceneNode* ps;
    scene::IParticleEmitter* em;

    /// Create particle system: (RING)
    ps = smgr->addParticleSystemSceneNode(true);
    em = ps->createRingEmitter(
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &center
    5, // f32 radius
    1, // f32 ringThickness
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &direction
    100, // u32 minParticlesPerSecond
    200, // u32 maxParticlesPerSecond
    SColor(255,255,255,255), // video::SColor &minStartColor
    SColor(255,255,255,255), // video::SColor &maxStartColor
    400, // u32 lifeTimeMin
    800, // u32 lifeTimeMax
    0,   // s32 maxAngleDegrees
    core::dimension2df(20.0f, 20.0f), // core::dimension2df &minStartSize
    core::dimension2df(40.0f, 40.0f)  // core::dimension2df &maxStartSize
    );

   	ps->setEmitter(em); // Grabs the emitter
	em->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(SColor(0,150,150,150), 500); // Fade out particles
	ps->addAffector(paf); // Same goes for the affector
	paf->drop();

	ps->setParent(node); // Attach to the node
	ps->setPosition(vector3df(0,0,0));
	ps->setRotation(core::vector3df(90,0,0));
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("smoke.png"));
	// ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    };
};




void Effects::WaterTrail(scene::ISceneNode* node){

    if(node){
    /// Create particles - Trails for the cannon balls:
    scene::IParticleSystemSceneNode* ps;
    scene::IParticleEmitter* em;

    /// Create particle system: (RING)
    ps = smgr->addParticleSystemSceneNode(true);
    em = ps->createRingEmitter(
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &center
    5, // f32 radius
    1, // f32 ringThickness
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &direction
    100, // u32 minParticlesPerSecond
    200, // u32 maxParticlesPerSecond
    SColor(255,255,255,255), // video::SColor &minStartColor
    SColor(255,255,255,255), // video::SColor &maxStartColor
    400, // u32 lifeTimeMin
    800, // u32 lifeTimeMax
    0,   // s32 maxAngleDegrees
    core::dimension2df(20.0f, 20.0f), // core::dimension2df &minStartSize
    core::dimension2df(40.0f, 40.0f)  // core::dimension2df &maxStartSize
    );

   	ps->setEmitter(em); // Grabs the emitter
	em->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(SColor(0,150,150,150), 500); // Fade out particles
	ps->addAffector(paf); // Same goes for the affector
	paf->drop();

	ps->setParent(node); // Attach to the node
	ps->setPosition(vector3df(0,0,0));
	ps->setRotation(core::vector3df(90,0,0));
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("smoke.png"));
	// ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    };
};


void Effects::CannonFiring(ISceneNode* node){

    /// Reset:
    cannonDuration = 0.0;
    canFireAgain   = 0.0;
    cannonAbleToFire = false;

    if(!cannonPNode && node){
    cannonPNode = smgr->addParticleSystemSceneNode(true);
 	cannonPNode->setParent(node); /// Attach this new node to the incoming node == So, will follows parent position and rotation
	cannonPNode->setPosition(vector3df(90,0,0));
	cannonPNode->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	cannonPNode->setMaterialTexture(0, driver->getTexture("smoke.png"));
	cannonPNode->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	cannonPNode->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	cannonPNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    };

    if(node){
	/// Create particles:
    scene::IParticleEmitter* em = 0;
    em = cannonPNode->createSphereEmitter(
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &center
    /// vector3df(0.0f, cy, 0.0f), // core::vector3df &center
    20, // f32 radius
    vector3df(0.08f, 0.08f, 0.08f), // core::vector3df &direction
    /// vector3df(0.05f+dy, dy, 0.0f), // core::vector3df &direction
    2000,  // u32 minParticlesPerSecond
    2000,  // u32 maxParticlesPerSecond
    SColor(0,255,255,255), // video::SColor &minStartColor
    SColor(0,255,255,255), // video::SColor &maxStartColor
    500, // u32 lifeTimeMin
    500, // u32 lifeTimeMax
    360,  // s32 maxAngleDegrees
    core::dimension2df(20.0f, 20.0f), // core::dimension2df &minStartSize
    core::dimension2df(50.0f, 50.0f)  // core::dimension2df &maxStartSize
    );

   	cannonPNode->setEmitter(em); // Grabs the emitter
	em->drop(); // We can drop it here, without deleting it

	/// createSphereEmitter + createFadeOutParticleAffector + createScaleParticleAffector + createAttractionAffector

	/// Fade out:
	scene::IParticleAffector* paf1 = cannonPNode->createFadeOutParticleAffector(SColor(0,250,250,250), 400); // Fade out particles
	/// scene::IParticleAffector* paf1 = ps->createFadeOutParticleAffector(); // Fade out particles
	cannonPNode->addAffector(paf1);
	paf1->drop();

	/// Scale:
	// scene::IParticleAffector* paf2 = ps->createScaleParticleAffector(dimension2df(30,30)); // Fade out particles
	// ps->addAffector(paf2);
	// paf2->drop();

	/// Attraction:
	// scene::IParticleAffector* paf3 = cannonPNode->createAttractionAffector(vector3df(0,0,-10), true, true, true, true); // Fade out particles
	// cannonPNode->addAffector(paf3);
	// paf3->drop();



	/// Delete ps node after x time...
    // scene::ISceneNodeAnimator* anim = smgr->createDeleteAnimator(2000);
	// ps->addAnimator(anim);
	// anim->drop();

	};

};


bool Effects::CannonCanFireAgain(){
    return cannonAbleToFire;
};

void Effects::CannonLoop(){
    /// Can fire again?
    canFireAgain += 1*frameDeltaTime;
    if(canFireAgain>500){
    cannonAbleToFire = true;
    };

    if(cannonPNode){
    /// Removing particles:
    int minP = cannonPNode->getEmitter()->getMinParticlesPerSecond(); minP-=3*frameDeltaTime;
    int maxP = cannonPNode->getEmitter()->getMinParticlesPerSecond(); maxP-=3*frameDeltaTime;
    if(minP<=0){ minP = 0; }; if(maxP<=0){ maxP = 0; };
    cannonPNode->getEmitter()->setMinParticlesPerSecond(minP);
    cannonPNode->getEmitter()->setMaxParticlesPerSecond(maxP);
    };

};


/// Hit Waves - Type 1:
void Effects::HitWave1(IRigidBody* body){
    if(body && body->isInWorld()){
    hitDoWave1Loop = true;
    hitWave1TextureTimer = 0.0;
    hitWave1TextNum = 0;
    hitWave1Position = body->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)
    };
};

void Effects::HitWave1Loop(){
    if(hitDoWave1Loop){
    hitWave1TextureTimer+=1.50*frameDeltaTime;
    if(hitWave1TextureTimer>=20){ /// Duration between textures (20 is perfect)
    /// Change texture;
    if(hitWave1TextNum<hitWaves1Textures.size()){
    /// Convert 3D to 2D positions:
    position2d<s32> pos = Convert3DTo2DPositions(hitWave1Position);
    pos.X -= 128/2; // half the image width
    pos.Y -= 128/2; // half the image height
    /// DRAW "energy bar" image background:
    if(hitWaves1Textures[hitWave1TextNum]){ driver->draw2DImage(hitWaves1Textures[hitWave1TextNum], pos, core::rect<s32>(0,0,128,128), 0, video::SColor(255,255,255,255), true); };
    };
    hitWave1TextureTimer = 0.0;
    hitWave1TextNum+=1;
    };
    if(hitWave1TextNum>=hitWaves1Textures.size()){
    hitWave1TextNum = 0;
    hitDoWave1Loop = false;
    };
    };
};



/// Draw 2D stuff: http://irrlicht.sourceforge.net/docu/classirr_1_1video_1_1_i_video_driver.html#aaf1318379f3d70c9347cfa853b944ad4
void Effects::HitScore(vector3df pos, int score, stringw color){
    if(textScore){

    /// Reset:
    scorePosY = pos.Y;

    /// Score Texts:
    scoreColorA = 255; scoreColorR = 255; scoreColorG = 255; scoreColorB = 255;
    if(color==L"white" ){ scoreColorA = 255; scoreColorR = 255; scoreColorG = 255; scoreColorB = 255; };
    if(color==L"red"   ){ scoreColorA = 255; scoreColorR = 255; scoreColorG =  10; scoreColorB =  10; };
    if(color==L"yellow"){ scoreColorA = 255; scoreColorR = 255; scoreColorG = 255; scoreColorB =  10;  };

    /// Show Score:
    stringw sc = L"";
    sc += score;
    textScore->setPosition(vector3df(pos.X, scorePosY, pos.Z));
    textScore->setText(sc.c_str());
    textScore->setTextColor(SColor(scoreColorA,scoreColorR,scoreColorG,scoreColorB));
    textScore->setVisible(true);
    /// Add to Total Score:
    TotalScoreAdd(score);

    };
};

void Effects::HitScoreLoop(){
    if(textScore){

    scoreColorA -= 0.15*frameDeltaTime;
    scorePosY += 0.35*frameDeltaTime;
    textScore->setTextColor(SColor(scoreColorA, scoreColorR, scoreColorG, scoreColorB));
    textScore->setPosition(vector3df(textScore->getPosition().X, scorePosY, textScore->getPosition().Z));
    // if(balloonTextures[0]){ driver->draw2DImage(balloonTextures[0], core::position2d<s32>(textScore->getPosition().X,textScore->getPosition().Y), core::rect<s32>(0,0,128,128), 0, SColor(255,255,255,255), true); };
    if(scoreColorA<=0){
    textScore->setVisible(false);
    };

    };
};





void Effects::HitHasBonus(IRigidBody* body){

    if(textBonus && body && body->isInWorld()){

    vector3df pos = body->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)
    /// Reset:
    bonusPosY = pos.Y;

    int bonus = body->getAttributes()->getAttributeAsInt("box_bonus");
    stringw bo = L"";
    if(bonus==0){ bo += "+5"; TotalScoreAdd(5); };
    if(bonus==1){ bo += "+10"; TotalScoreAdd(10); };
    if(bonus==2){ bo += "+20"; TotalScoreAdd(20); };
    // if(bonus==1){ bo += "Extra Ball"; /* Add extra ball */  };

    /// Bonus Texts:
    stringw color = "white";
    bonusColorA = 255; bonusColorR = 255; bonusColorG = 255; bonusColorB = 255;
    if(color==L"white" ){ bonusColorA = 255; bonusColorR = 255; bonusColorG = 255; bonusColorB = 255; };
    if(color==L"red"   ){ bonusColorA = 255; bonusColorR = 255; bonusColorG =  10; bonusColorB =  10; };
    if(color==L"yellow"){ bonusColorA = 255; bonusColorR = 255; bonusColorG = 255; bonusColorB =  10; };

    textBonus->setPosition(vector3df(pos.X, bonusPosY, pos.Z));
    textBonus->setText(bo.c_str());
    textBonus->setTextColor(SColor(bonusColorA,bonusColorR,bonusColorG,bonusColorB));
    textBonus->setVisible(true);

    };
};


void Effects::HitHasBonusLoop(){
    if(textBonus){

    bonusColorA -= 0.06*frameDeltaTime;
    bonusPosY += 0.30*frameDeltaTime;
    textBonus->setTextColor(SColor(bonusColorA, bonusColorR, bonusColorG, bonusColorB));
    textBonus->setPosition(vector3df(textBonus->getPosition().X, bonusPosY, textBonus->getPosition().Z));
    if(bonusColorA<=0){ textBonus->setVisible(false); };

    };
};






int Effects::HitJail(IRigidBody* body, int remove_energy){

    if(body && body->isInWorld()){

    int inID = body->getUniqueID();
    energyBarPosition = body->getWorldTransform().getTranslation();

    for(unsigned int i=0; i<jails.size();  i++) {
    if(jails[i].Body && jails[i].Body->isInWorld() && jails[i].Node){
    int id = jails[i].BodyID;
    if(inID == id) {
    /// Set new energy: (Will be needed for the 2D Energy Bar too)
    jails[i].Energy -= remove_energy;
    energyLeft = jails[i].Energy;
    if(energyLeft> 0){ hitRemoveEnergyDuration = 6000.0; };
    if(energyLeft<=0){ hitRemoveEnergyDuration =    0.0; };
    energyBarAlpha1 = 255;
    energyBarAlpha2 = 195;
    /// Apply damaged texture:
    if(jails[i].Energy<=80 && jails[i].Energy>50){ for(int m=0; m<jails[i].Node->getMaterialCount(); m++){ jails[i].Node->getMaterial(m).setTexture(0, driver->getTexture("jail_01B.png")); }; };
    if(jails[i].Energy<=50 && jails[i].Energy>0 ){ for(int m=0; m<jails[i].Node->getMaterialCount(); m++){ jails[i].Node->getMaterial(m).setTexture(0, driver->getTexture("jail_01C.png")); }; };
    if(jails[i].Energy<=0){

    /// Prisoner is ready to escape!... Mark him!
    for(unsigned int p=0; p<prisoners.size(); p++) {
    if(prisoners[p].Body && prisoners[p].Body->isInWorld()){
    if(jails[i].JailNumber == prisoners[p].PrisonerNumber){ prisoners[p].Free = true; };
    };
    };


    for(int m=0; m<jails[i].Node->getMaterialCount(); m++){
    /// Change texture:
    jails[i].Node->getMaterial(m).setTexture(0, driver->getTexture("jail_01D.png"));

    /// AND... Make jail "invisible" to the physics:
    jails[i].Body->getPointer()->setCollisionFlags(DISABLE_SIMULATION);
    /// ALLWAYS search and remove bodies from its own vector array: (Needed to remove from jail array, so it matches its prisoner array equally!!!...)
    if(!jails.empty()) {
    for(int a=0; a<jails.size(); a++) { if(inID == jails[a].BodyID){ jails.erase(jails.begin()+a); std::cout << "FREE PRISONER: Removed jail ID: " << id << " only from its array!" << std::endl; }; };
    };

    };
    };
    cout << "Jail energy: " << jails[i].Energy << endl;
    };
    };
    };

    return energyLeft;

    };

    return 100;
};



void Effects::HitJail2DEnergyBarLoop(){

    if(hitRemoveEnergyDuration>0.0){
    /// All 2D must render after smgr->DrawAll():
    int bar_percentage = (100*energyLeft)/100; /// Percentage between 2 numbers: (100*current)/total;
    /// Convert 3D to 2D positions:
    position2d<s32> pos = Convert3DTo2DPositions(energyBarPosition);
    pos.X -= 50; // half the bar length
    pos.Y -= 70; // above the enemy
    /// DRAW "energy bar" image background:
    if(energy_Left){ driver->draw2DImage(energy_Left, pos, core::rect<s32>(0,0,100,10), 0, video::SColor(energyBarAlpha2,0,0,0), true); };
    /// DRAW "energy bar" image:
    if(energy_Left){ driver->draw2DImage(energy_Left, pos, core::rect<s32>(0,0,bar_percentage,10), 0, video::SColor(energyBarAlpha1,255,20,20), true); };
    /// DRAW "energy bar" frame image:
    if(energy_Frame){ driver->draw2DImage(energy_Frame, pos, core::rect<s32>(0,0,100,10), 0, video::SColor(energyBarAlpha1,255,255,255), true); };
    };
    if(hitRemoveEnergyDuration<500.0){
    /// Alphas:
    energyBarAlpha1 -= 0.55*frameDeltaTime;
    energyBarAlpha2 -= 0.55*frameDeltaTime;
    if(energyBarAlpha1<=0){ energyBarAlpha1=0; };
    if(energyBarAlpha2<=0){ energyBarAlpha2=0; };
    };
    /// Timer:
    hitRemoveEnergyDuration -= 0.50*frameDeltaTime;
    if(hitRemoveEnergyDuration<=0.0){ hitRemoveEnergyDuration=0.0; };
};



void Effects::HitTarget(ISceneNode* node){

    /// Reset:
    hitTargetDuration = 0.0f;

    if(node){
	/// Create particles:
    scene::IParticleSystemSceneNode* ps;
    scene::IParticleEmitter* em;

    /// Create particle system: (RING)
    ps = smgr->addParticleSystemSceneNode(true);
    em = ps->createRingEmitter(
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &center
    50, // f32 radius
    1,  // f32 ringThickness
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &direction
    100, // u32 minParticlesPerSecond
    200, // u32 maxParticlesPerSecond
    SColor(0,255,0,0), // video::SColor &minStartColor
    SColor(0,255,0,0), // video::SColor &maxStartColor
    400, // u32 lifeTimeMin
    800, // u32 lifeTimeMax
    0,   // s32 maxAngleDegrees
    core::dimension2df(30.0f, 30.0f), // core::dimension2df &minStartSize
    core::dimension2df(60.0f, 60.0f)  // core::dimension2df &maxStartSize
    );

   	ps->setEmitter(em); // Grabs the emitter
	em->drop(); // We can drop it here, without deleting it

	/// scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(SColor(255,150,150,150), 1000); // Fade out particles
	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(SColor(0,255,0,0), 1000); // Fade out particles
	ps->addAffector(paf); // Same goes for the affector
	paf->drop();

	ps->setParent(node); /// Attach this new node to the incoming node == So, will follows parent position and rotation
	ps->setPosition(vector3df(0,0,0));
	ps->setRotation(core::vector3df(90,0,0));
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("smoke.png"));
	// ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	/// Delete IParticleSystemSceneNode after x time...
    scene::ISceneNodeAnimator* anim = smgr->createDeleteAnimator(1000);
	ps->addAnimator(anim);
	anim->drop();
	};
};


void Effects::HitTargetLoop(){
    /*
    hitTargetDuration += 1*frameDeltaTime;
    if(hitTargetDuration>1000){
    /// Remove previous node, if it exists:
    scene::ISceneNode* n = smgr->getSceneNodeFromName("nodeTarget_FX");
    if(n){ n->remove(); };
    };
    */
};



void Effects::HitSoldier(IRigidBody* body){

    /// Reset:
    hitSoldierDuration = 0.0f;

    if(body && body->isInWorld()){
    if(!soldiers.empty()){

    /// Search node by its body ID:
    u32 id = body->getUniqueID();

    for(unsigned int i=0; i<soldiers.size(); i++) {
    if(id == (soldiers[i].Body->getUniqueID())) {
    soldiers[i].Node->setName("nodeSoldier_ANGRY");
    for(u32 m=0; m<soldiers[i].Node->getMaterialCount(); m++){
    /// Set soldier face texture to: Angry
	soldiers[i].Node->getMaterial(m).setTexture(0, driver->getTexture("soldier_01C.jpg"));
	};
    };
    };

    /// Get body position (in irrBullet):
    vector3df pos = body->getWorldTransform().getTranslation();

    /// Remove if exists:
    scene::ISceneNode* n = smgr->getSceneNodeFromName("nodeSoldier_PARTICLES");
    if(n){ n->remove(); };

    /// Create dummy node: (So particles could be removed)
    scene::ISceneNode* node = smgr->addCubeSceneNode(0);
	node->setName("nodeSoldier_PARTICLES");
	// vector3df post = textScore->getPosition();
	// node->setPosition(vector3df(pos.X,pos.Y,pos.Z-300));
	node->setPosition(vector3df(pos));

	/// Create particles:
    scene::IParticleSystemSceneNode* ps;
    scene::IParticleEmitter* em;
    SColor ccc = SColor(255,255,255,255); /// White smoke

    /// Create particle system: (RING STARS)
    ps = smgr->addParticleSystemSceneNode(true);
    em = ps->createRingEmitter(
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &center
    30, // f32 radius
    1,  // f32 ringThickness
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &direction
    30,  // u32 minParticlesPerSecond
    30,  // u32 maxParticlesPerSecond
    SColor(ccc), // video::SColor &minStartColor
    SColor(ccc), // video::SColor &maxStartColor
    200, // u32 lifeTimeMin
    400, // u32 lifeTimeMax
    0,   // s32 maxAngleDegrees
    core::dimension2df(20.0f, 20.0f), // core::dimension2df &minStartSize
    core::dimension2df(40.0f, 40.0f)  // core::dimension2df &maxStartSize
    );

   	ps->setEmitter(em); // Grabs the emitter
	em->drop(); // We can drop it here, without deleting it

	/// scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(SColor(255,150,150,150), 1000); // Fade out particles
	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(SColor(ccc), 1000); // Fade out particles
	ps->addAffector(paf); // Same goes for the affector
	paf->drop();

	ps->setParent(node); // Attach to the node
	ps->setPosition(vector3df(0,0,0));
	ps->setRotation(core::vector3df(90,0,0));
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("smoke.png"));
	// ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    };
    };
};



void Effects::HitSoldierLoop(){
    hitSoldierDuration += 1*frameDeltaTime;
    if(hitSoldierDuration>500){
    /// Remove previous particles node, if it exists:
    scene::ISceneNode* node1 = smgr->getSceneNodeFromName("nodeSoldier_PARTICLES");
    if(node1){ node1->remove(); };
    /// Remove previous style node, if it exists:
    scene::ISceneNode* node2 = smgr->getSceneNodeFromName("nodeSoldier_ANGRY");
    if(node2){
    node2->setName("");
    /// Apply texture to my specified material
    for(u32 m=0; m<node2->getMaterialCount(); m++){
	/// Set soldier face texture to: Normal
	node2->getMaterial(m).setTexture(0, driver->getTexture("soldier_01A.jpg"));
	};
    };
    };
};





void Effects::HitBox(vector3df pos){

    /// Reset:
    hitBoxDuration = 0.0f;

    /// Remove if exists:
    scene::ISceneNode* n = smgr->getSceneNodeFromName("nodeBox_PARTICLES");
    if(n){ n->remove(); };

    /// Create dummy node: (So particles could be removed)
    scene::ISceneNode* node = smgr->addCubeSceneNode(0);
	node->setName("nodeBox_PARTICLES");
	// vector3df post = textScore->getPosition();
	// node->setPosition(vector3df(pos.X,pos.Y,pos.Z-300));
	node->setPosition(vector3df(pos));
	node->setRotation(core::vector3df(90,0,0)); /// Important set rotation here, to match all scene

	/// Create particles:
    scene::IParticleSystemSceneNode* ps;
    scene::IParticleEmitter* em;
    SColor ccc = SColor(255,255,255,255);

    /// Create particle system: (RING STARS)
    ps = smgr->addParticleSystemSceneNode(true);
    em = ps->createRingEmitter(
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &center
    5,  // f32 radius
    10, // f32 ringThickness
    vector3df(0.0f, 0.15f, 0.0f), // core::vector3df &direction
    60,   // u32 minParticlesPerSecond
    120,  // u32 maxParticlesPerSecond
    SColor(ccc), // video::SColor &minStartColor
    SColor(ccc), // video::SColor &maxStartColor
    1000, // u32 lifeTimeMin
    2000, // u32 lifeTimeMax
    180,  // s32 maxAngleDegrees
    core::dimension2df(20.0f, 20.0f), // core::dimension2df &minStartSize
    core::dimension2df(50.0f, 50.0f)  // core::dimension2df &maxStartSize
    );

   	ps->setEmitter(em); // Grabs the emitter
	em->drop(); // We can drop it here, without deleting it

	/// scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(SColor(255,150,150,150), 1000); // Fade out particles
	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(SColor(ccc), 500); // Fade out particles
	ps->addAffector(paf); // Same goes for the affector
	paf->drop();

	ps->setParent(node); // Attach to the node
	ps->setPosition(vector3df(0,0,0));
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("particles_box_01.png"));
	ps->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	// ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	// ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
};

void Effects::HitBoxLoop(){
    hitBoxDuration += 1*frameDeltaTime;
    if(hitBoxDuration>800){
    /// Remove previous node, if it exists:
    scene::ISceneNode* node = smgr->getSceneNodeFromName("nodeBox_PARTICLES");
    if(node){ node->remove(); };
    };
};


void Effects::HitWater(vector3df pos){

    /// Reset:
    hitWaterDuration = 0.0f;

    /// Remove if exists:
    scene::ISceneNode* n = smgr->getSceneNodeFromName("nodeWater_PARTICLES");
    if(n){ n->remove(); };

    /// Create dummy node: (So particles could be removed)
    scene::ISceneNode* node = smgr->addCubeSceneNode(0);
	node->setName("nodeWater_PARTICLES");
	// vector3df post = textScore->getPosition();
	// node->setPosition(vector3df(pos.X,pos.Y,pos.Z-300));
	node->setPosition(vector3df(pos));

	/// Create particles:
    scene::IParticleSystemSceneNode* ps;
    scene::IParticleEmitter* em;
    SColor ccc = SColor(100,255,255,255);

    /// Create particle system: (RING)
    ps = smgr->addParticleSystemSceneNode(true);
    em = ps->createRingEmitter(
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &center
    13, // f32 radius
    1,  // f32 ringThickness
    vector3df(0.0f, 0.15f, 0.0f), // core::vector3df &direction
    100,  // u32 minParticlesPerSecond
    200,  // u32 maxParticlesPerSecond
    SColor(ccc), // video::SColor &minStartColor
    SColor(ccc), // video::SColor &maxStartColor
    500,  // u32 lifeTimeMin
    1000, // u32 lifeTimeMax
    0,    // s32 maxAngleDegrees
    core::dimension2df(20.0f, 20.0f), // core::dimension2df &minStartSize
    core::dimension2df(30.0f, 30.0f)  // core::dimension2df &maxStartSize
    );

   	ps->setEmitter(em); // Grabs the emitter
	em->drop(); // We can drop it here, without deleting it

	/// scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(SColor(255,150,150,150), 1000); // Fade out particles
	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(SColor(ccc), 1000); // Fade out particles
	ps->addAffector(paf); // Same goes for the affector
	paf->drop();

	ps->setParent(node); // Attach to the node
	ps->setPosition(vector3df(0,0,0));
	ps->setRotation(core::vector3df(90,0,0));
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("smoke.png"));
	// ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

};


void Effects::HitWaterLoop(){
    hitWaterDuration += 1*frameDeltaTime;
    if(hitWaterDuration>600){
    /// Remove previous node, if it exists:
    scene::ISceneNode* n = smgr->getSceneNodeFromName("nodeWater_PARTICLES");
    if(n){ n->remove(); };
    };
};


void Effects::TotalScoreAdd(int add_score){
    PLAYER_TOTAL_SCORE += add_score;
    stringw str = L"SCORE: "; str += PLAYER_TOTAL_SCORE;
    textTotalScore->setText(str.c_str());
};



/*
    vector3df balloonSolPosition; /// handles position, even if its body was removed
    bool doSolBalloon        = false;
    f32 balloonSolDuration   = 0.0;
    int randomSolBalloon     = 0;
    f32 newRandSolBalloon    = 0.0;
    bool doNewRandSolBalloon = true;
*/


void Effects::SoldiersBalloon(IRigidBody* body){
    if(body){ /// Body exists? (Could have been removed, if not exists it crashes!)
    if(body->isInWorld()){ /// Body exists? (Could have been removed, if not exists it crashes!)
    bodySoldierForBalloons = body;
    balloonSolDuration = 0.0;
    doSolBalloon = true;

    if(bodySoldierForBalloons->getAttributes()->existsAttribute("soldier")){
    if(doNewRandSolBalloon){
    randomSolBalloon = rand()%soldiersBalloonTextures.size();
    doNewRandSolBalloon = false;
    };
    };

    /// nodeBalloons[0]->setVisible(true);
    };
    };
};


/// Code snippets: http://irrlicht.sourceforge.net/forum/viewtopic.php?t=34385
void Effects::SoldiersBalloonLooping(){

    /// Text Ballon duration:
    balloonSolDuration += 1*frameDeltaTime;
    newRandSolBalloon += 1*frameDeltaTime;

    if(balloonSolDuration>1500){
    doSolBalloon = false;
    };

    if(newRandSolBalloon>1000){
    doNewRandSolBalloon = true;
    newRandSolBalloon = 0.0;
    };

    if(doSolBalloon == true){
    if(bodySoldierForBalloons){ /// Body exists? (Could have been removed, if not exists it crashes!)
    if(bodySoldierForBalloons->isInWorld()){ /// Body exists? (Could have been removed, if not exists it crashes!)
    /// Convert 3D to 2D positions:
    balloonSolPosition = bodySoldierForBalloons->getWorldTransform().getTranslation(); /// Get body position (in irrBullet)
    position2d<s32> pos = Convert3DTo2DPositions(balloonSolPosition);
    pos.X -= 32; // half the image length
    pos.Y -= 96; // above the enemy
    /// DRAW "energy bar" image background:
    if(soldiersBalloonTextures[randomSolBalloon]){ driver->draw2DImage(soldiersBalloonTextures[randomSolBalloon], pos, core::rect<s32>(0,0,64,64), 0, video::SColor(255,255,255,255), true); };
    };
    };
    };
};





/// TOOL - Convert 3D to 2D positions:
/// http://irrlicht.sourceforge.net/forum/viewtopic.php?t=93
position2d<s32> Effects::Convert3DTo2DPositions(vector3df pos3D) {

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



void Effects::Loop(){

    /// Cannon:
    CannonLoop();

    /// Hit wave 1:
    /// HitWave1Loop(); // <-- Loop called in "game-level.cpp" ( 2D draws must be after smgr->drawAll() )

    /// Score:
    HitScoreLoop();

    /// Bonus:
    HitHasBonusLoop();

    /// Remove Energy:
    /// Hit2DEnergyBarLoop(); // <-- Loop called in "game-level.cpp" ( 2D draws must be after smgr->drawAll() )

    /// Smoke:
    HitTargetLoop();

    /// Hit box:
    HitBoxLoop();

    /// Hit soldier:
    HitSoldierLoop();

    /// Bubbles:
    HitWaterLoop();

    /// Balloons:
    /// BalloonLooping(); // <-- Loop called in "game-level.cpp" ( 2D draws must be after smgr->drawAll() )

};





