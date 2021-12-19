/// This class is part of Game class.
/// So depends of objects already created in the Game class.


/// PHYSICS: COLLISIONS ENUMERATOR ---------------------------------------------------------------------------------------------

/// One body collides with another body... Counts?...
/// How-To: http://bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Filtering
#define BIT(x) (1<<(x))
enum collisionTypes {
    COL_NOTHING          = 0,
    COL_SCENARY          = BIT(1),
    COL_SOLDIERS         = BIT(2),
    COL_PRISONERS        = BIT(3),
    COL_BALLS            = BIT(4),
    COL_TARGETS          = BIT(5),
    COL_BOXES            = BIT(6),
    COL_JAILS            = BIT(7),
    COL_BIRDS            = BIT(8),
    COL_CARGOS           = BIT(9),
    COL_CHICKENS         = BIT(10),
    COL_BELLS            = BIT(11),
    COL_PRIESTS          = BIT(12),
    COL_PRIESTS_WALKING  = BIT(13),
    COL_CHAINS           = BIT(14),
    COL_PLATFORMS        = BIT(15),
    COL_WORKERS          = BIT(16),
    COL_KNIGHTS          = BIT(17),
    COL_KINGS            = BIT(18),
    COL_MUSICIANS        = BIT(19),
    COL_BULLS            = BIT(20)
};
int scenaryCollidesWith         = COL_SOLDIERS | COL_PRISONERS | COL_BALLS | COL_BOXES | COL_CARGOS | COL_CHICKENS | COL_WORKERS | COL_KNIGHTS | COL_PRIESTS | COL_PRIESTS_WALKING | COL_KINGS | COL_MUSICIANS | COL_BULLS;
int soldiersCollidesWith        = COL_SCENARY | COL_BALLS | COL_BOXES; /// (Soldiers do not collide with each other)
int prisonersCollidesWith       = COL_SCENARY;
int ballsCollidesWith           = COL_SCENARY | COL_BALLS | COL_SOLDIERS | COL_TARGETS | COL_BOXES | COL_JAILS | COL_BIRDS | COL_CARGOS | COL_CHICKENS | COL_BELLS | COL_PLATFORMS | COL_WORKERS | COL_KNIGHTS;
int targetsCollidesWith         = COL_BALLS;
int boxesCollidesWith           = COL_SCENARY | COL_BOXES | COL_CARGOS | COL_SOLDIERS | COL_BALLS | COL_JAILS | COL_CHICKENS;
int jailsCollidesWith           = COL_BOXES | COL_CARGOS | COL_BALLS;
int birdsCollidesWith           = COL_BALLS;
int cargosCollidesWith          = COL_CARGOS | COL_BALLS | COL_BOXES | COL_JAILS | COL_SCENARY | COL_CHICKENS | COL_PLATFORMS;
int chickensCollidesWith        = COL_BALLS | COL_CARGOS | COL_BOXES | COL_SCENARY | COL_PLATFORMS;
int bellsCollidesWith           = COL_BALLS;
int priestsCollidesWith         = COL_SCENARY;
int priestsWalkingCollidesWith  = COL_SCENARY;
int chainsCollidesWith          = COL_NOTHING;
int platformsCollidesWith       = COL_BALLS | COL_CHICKENS | COL_CARGOS;
int workersCollidesWith         = COL_BALLS | COL_SCENARY;
int knightsCollidesWith         = COL_BALLS | COL_SCENARY;
int kingsCollidesWith           = COL_SCENARY;
int musiciansCollidesWith       = COL_SCENARY;
int bullsCollidesWith           = COL_SCENARY;

/// PHYSICS: COLLISIONS ENUMERATOR ---------------------------------------------------------------------------------------------



/// IRRLICHT:
scene::ISceneNode* nodeLevel;
scene::IAnimatedMeshSceneNode* nodeSky;
RealisticWaterSceneNode* waterNode;
scene::IAnimatedMeshSceneNode* nodePirateShip;
scene::IAnimatedMeshSceneNode* nodeCannon;
scene::IAnimatedMeshSceneNode* nodeShark1;
scene::IAnimatedMeshSceneNode* nodeShark2;
scene::IAnimatedMeshSceneNode* nodeSceneTransparents;
scene::IAnimatedMeshSceneNode* nodeSceneDecoration;
scene::IAnimatedMeshSceneNode* nodeScenePhysics;


/// Camera Light:
/// scene::ISceneNode* nodeCameraLight;
/// Fade IN Materials:
f32 fadeInMaterials = 0;
bool fadein_materials = false;
/// Fade OUT Materials:
f32 fadeOutMaterials  = 255;
bool fadeout_materials = false;



class Scene : public Game {

    public:
    Scene();  // Constructor
    ~Scene(); // Destructor
    void Initialize();
    void CreateCommons();
    void CreateFlags();
    void CreateTargets();
    void RemoveBoxes();
    void CreateBoxes();
    void CreateBells();
    void CreateChains();
    void CreatePlatforms();
    void CreateChainsDecorations();
    void RemoveCargos();
    void CreateCargos();
    void CreateSoldiers();
    void RemoveJails();
    void CreateJails();
    void RemovePrisoners();
    void CreatePrisoners();
    void CreateBirds();
    void CreateChickens();
    void CreatePriests();
    void CreatePriestsWalking();
    void CreateWorkers();
    void CreateKnights();
    void CreateKings();
    void CreateMusicians();
    void CreateBulls();
    void StartMaterials(); /// Last functions: To get all the created nodes
    void FadeMaterials();  /// Last functions: To get all the created nodes
    void Loop();

    private:

    /// Only a pointer to other classes already included:
    // Effects* effects;

};

Scene::Scene(){};  // Constructor (Can accept arguments, but this one does not)
Scene::~Scene(){}; // Destructor (do not accept arguments)




void Scene::Initialize(){

    CreateCommons();            cout << "Scene->CreateCommons() OK" << endl;
    CreateFlags();              cout << "Scene->CreateFlags() OK" << endl;
    CreateTargets();            cout << "Scene->CreateTargets() OK" << endl;

    RemoveBoxes();
    CreateBoxes();              cout << "Scene->CreateBoxes() OK" << endl;

    /// RemoveCargos(); /// <-- Called in animations
    /// CreateCargos(); /// <-- Called in animations

    CreateChains();             cout << "Scene->CreateChains() OK" << endl;
    CreatePlatforms();          cout << "Scene->CreatePlatforms() OK" << endl;
    CreateChainsDecorations();  cout << "Scene->CreateChainsDecorations() OK" << endl;
    CreateBells();              cout << "Scene->CreateBells() OK" << endl;
    CreateBirds();              cout << "Scene->CreateBirds() OK" << endl;

    RemoveJails();
    CreateJails();              cout << "Scene->CreateJails() OK" << endl;

    RemovePrisoners();          cout << "Scene->CreatePrisoners() OK" << endl;
    CreatePrisoners();          cout << "Scene->CreatePrisoners() OK" << endl;

    CreateSoldiers();           cout << "Scene->CreateSoldiers() OK" << endl;
    CreateChickens();           cout << "Scene->CreateChickens() OK" << endl;
    CreatePriests();            cout << "Scene->CreatePriests() OK" << endl;
    CreatePriestsWalking();     cout << "Scene->CreatePriestsWalking() OK" << endl;
    CreateWorkers();            cout << "Scene->CreateWorkers() OK" << endl;
    CreateKnights();            cout << "Scene->CreateKnights() OK" << endl;
    CreateKings();              cout << "Scene->CreateKings() OK" << endl;
    CreateMusicians();          cout << "Scene->CreateMusicians() OK" << endl;
    CreateBulls();              cout << "Scene->CreateBulls() OK" << endl;

    StartMaterials();
    /// FadeMaterials(); /// Last functions: To get all the created nodes

};




void Scene::CreateCommons(){

    /// Commons => Must be created only once (Specially if they are global nodes)

    /// Create main LIGHT:
    smgr->setAmbientLight(video::SColor(0,250,250,250)); // Ambient Light => SColor (u32 a, u32 r, u32 g, u32 b)

    /// Create FOG:
    /// driver->setFog(video::SColor(0,255,255,255), video::EFT_FOG_LINEAR, 50, 100, 10.05f, false, false);
    driver->setFog(video::SColor(100,100,0,0), video::EFT_FOG_LINEAR, 10, 100, 1.0, true, false);


    /// Create:
    /// THE PARENT NODE (Handles all this level nodes):
    // nodeLevel->remove();
    // nodeLevel->drop();

    if(!nodeLevel){
    nodeLevel = smgr->addEmptySceneNode();
    nodeLevel->setPosition(vector3df(0.0,0.0,0.0));
    nodeLevel->setRotation(vector3df(0.0,0.0,0.0));
    };


    /// Create (Loads Object) SKY:
    if(!nodeSky){
	nodeSky = smgr->addAnimatedMeshSceneNode(smgr->getMesh("sky_01.obj"));
	nodeSky->setParent(nodeLevel); /// Attach to parent: Level
	nodeSky->setPosition(vector3df(3000,50,2000));
	nodeSky->setRotation(vector3df(0,180,0));
	nodeSky->setScale(core::vector3df(1,1,1));
	nodeSky->setMaterialFlag(video::EMF_LIGHTING, false);          // Node is affected by LIGHT?
    nodeSky->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeSky->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);  // Render both sides !!! Affects water reflex !!!
    nodeSky->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeSky->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeSky->setMaterialType(video::EMT_SOLID);
    /// Set texture:
    for(unsigned int m=0; m<nodeSky->getMaterialCount(); m++){
	nodeSky->getMaterial(m).setTexture(0, driver->getTexture("sky_blue.jpg"));
	};
    };

	/// ---------------------------------------------------------------------------------------------------------------------

    /// NODE -> WATER: (Sea)
    /// TO-DO: THIS ONE CAUSES MEMORY LEACKS ON RELOAD !!!!!!! ...
    if(!waterNode){
    waterNode = new RealisticWaterSceneNode(smgr, 10000, 10000, "water", irr::core::dimension2du(512, 512), nodeLevel, -1);
    /// waterNode->setParent(nodeLevel); /// Attach to parent: Level
    waterNode->setPosition(vector3df(2500,0,0));
    waterNode->setRotation(vector3df(0,0,0));
    waterNode->setWindForce(10.0f);
    waterNode->setWindDirection(irr::core::vector2df(5.f, -5.f));
    waterNode->setWaveHeight(0.3f);
    waterNode->setWaterColor(video::SColor(0,50,50,50)); /// Black
    waterNode->setColorBlendFactor(0.15);

    /// Materials:
	waterNode->setMaterialFlag(video::EMF_LIGHTING, true);          // Node is affected by LIGHT?
    waterNode->setMaterialFlag(video::EMF_FOG_ENABLE, true);        /// Node is affected by FOG? - (Its the only node with the fog enabled)
    waterNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true); // Render both sides !!! Affects water reflex !!!
    waterNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    /// waterNode->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    waterNode->setMaterialType(video::EMT_SOLID);
    };

    /// Add animator:
    // virtual ISceneNodeAnimator* irr::scene::ISceneManager::createFlyStraightAnimator(core::vector3df& startPoint, core::vector3df& endPoint, u32 timeForWay, bool loop = false, bool pingpong = false )
    // scene::ISceneNodeAnimator* anim = smgr->createFlyStraightAnimator(core::vector3df(0,8,0), core::vector3df(0,-5,0), 5000.0f, true, true);
    // waterNode->addAnimator(anim);
    // anim->drop();


    /// ---------------------------------------------------------------------------------------------------------------------

    /// Create (Loads Object) SHARK 1:
    if(!nodeShark1){
	nodeShark1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("shark_01.X"));
	nodeShark1->setParent(nodeLevel); /// Attach to parent: Level
	nodeShark1->setPosition(vector3df(-1000,-40,2000));
	nodeShark1->setRotation(vector3df(0,0,0));
	nodeShark1->setScale(core::vector3df(1,1,1));
	nodeShark1->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeShark1->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeShark1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides !!! Affects water reflex !!!
    nodeShark1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeShark1->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeShark1->setMaterialType(video::EMT_SOLID);
    nodeShark1->setVisible(true);

    if(true){
    /// Create particles - SHARK 1 - BUBBLES:
    scene::IParticleSystemSceneNode* ps;
    scene::IParticleEmitter* em;
    /// Create particle system: (CYLINDER)
    ps = smgr->addParticleSystemSceneNode(true);
    em = ps->createCylinderEmitter(
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &center
    40,    // radius
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &direction
    150,   // length
    true,  // outline only
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &normal
    50,   // min
    200,  // max
    SColor(255,255,255,255), // video::SColor &minStartColor
    SColor(255,255,255,255), // video::SColor &maxStartColor
    1000, // lifeTimeMin
    3000, // lifeTimeMax
    0,    // Angle
    core::dimension2df(10.0f, 10.0f), // core::dimension2df &minStartSize
    core::dimension2df(40.0f, 40.0f)  // core::dimension2df &maxStartSize
    );

   	ps->setEmitter(em); // Grabs the emitter
	em->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(SColor(0,150,150,150), 250); // Fade out particles
	ps->addAffector(paf); // Same goes for the affector
	paf->drop();

	ps->setParent(nodeShark1); // Attach to the node
	ps->setPosition(vector3df(0,54,0));
	ps->setRotation(core::vector3df(0,0,0));
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("smoke.png"));
	ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	ps->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    };

    };


    /// ---------------------------------------------------------------------------------------------------------------------

    /// Create (Loads Object) SHARK 2:
    if(!nodeShark2){
	nodeShark2 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("shark_01.X"));
	nodeShark2->setParent(nodeLevel); /// Attach to parent: Level
	nodeShark2->setPosition(vector3df(-500,-68,-750));
	nodeShark2->setRotation(vector3df(0,0,0));
	nodeShark2->setScale(core::vector3df(1,1,1));
	nodeShark2->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeShark2->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeShark2->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides !!! Affects water reflex !!!
    nodeShark2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeShark2->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeShark2->setMaterialType(video::EMT_SOLID);
    nodeShark2->setVisible(false);

    if(true){
    /// Create particles - SHARK 2 - BUBBLES:
    scene::IParticleSystemSceneNode* ps;
    scene::IParticleEmitter* em;
    /// Create particle system: (CYLINDER)
    ps = smgr->addParticleSystemSceneNode(true);
    em = ps->createCylinderEmitter(
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &center
    40,    // radius
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &direction
    150,   // length
    true,  // outline only
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &normal
    100,   // min
    300,   // max
    SColor(255,255,255,255), // video::SColor &minStartColor
    SColor(255,255,255,255), // video::SColor &maxStartColor
    1000,  // lifeTimeMin
    3000,  // lifeTimeMax
    0,     // Angle
    core::dimension2df(30.0f, 30.0f), // core::dimension2df &minStartSize
    core::dimension2df(80.0f, 80.0f)  // core::dimension2df &maxStartSize
    );

   	ps->setEmitter(em); // Grabs the emitter
	em->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(SColor(0,150,150,150), 250); // Fade out particles
	ps->addAffector(paf); // Same goes for the affector
	paf->drop();

	ps->setParent(nodeShark2); // Attach to the node
	ps->setPosition(vector3df(0,54,0));
	ps->setRotation(core::vector3df(0,0,0));
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("smoke.png"));
	ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	ps->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    };

    };


    /// ---------------------------------------------------------------------------------------------------------------------

    /// Create (Loads Object) PIRATE SHIP:
    if(!nodePirateShip){
	nodePirateShip = smgr->addAnimatedMeshSceneNode(smgr->getMesh("ship_01.X"));
	nodePirateShip->setParent(nodeLevel); /// Attach to parent: Level
	nodePirateShip->setPosition(vector3df(0,0,0));
	nodePirateShip->setRotation(vector3df(0,0,0));
	nodePirateShip->setScale(core::vector3df(1,1,1));
	nodePirateShip->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodePirateShip->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodePirateShip->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides !!! Affects water reflex !!!
    nodePirateShip->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodePirateShip->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodePirateShip->setMaterialType(video::EMT_SOLID);
    /// Get personage text name and attach it:
	if(NAMES_TEXTS==true){
    personageName[0]->setParent(nodePirateShip); personageName[0]->setPosition(vector3df(-50,210,0));
	};
    };


    /// ---------------------------------------------------------------------------------------------------------------------

    /// Create (Loads Object) CANNON:
    if(!nodeCannon){
	nodeCannon = smgr->addAnimatedMeshSceneNode(smgr->getMesh("cannon_01.X"));
	nodeCannon->setName("cannon");
	nodeCannon->setParent(nodePirateShip); /// Attach to parent: The pirate ship!
	nodeCannon->setPosition(vector3df(17,90,0)); /// Position relative to its parent
	nodeCannon->setRotation(vector3df(0,0,0));
	nodeCannon->setScale(core::vector3df(1,1,1));
	nodeCannon->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeCannon->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeCannon->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides !!! Affects water reflex !!!
    nodeCannon->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeCannon->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeCannon->setMaterialType(video::EMT_SOLID);
    // nodeCannon->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    // nodeCannon->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    };


    /// ---------------------------------------------------------------------------------------------------------------------


    /// Create (Loads Object) SCENE TRANSPARENTS ONLY:
    if(!nodeSceneTransparents){
    nodeSceneTransparents = smgr->addAnimatedMeshSceneNode(smgr->getMesh("scene_transparents_01.obj")); /// This one MUST be ".obj"
    nodeSceneTransparents->setParent(nodeLevel); /// Attach to parent: Level
	nodeSceneTransparents->setPosition(vector3df(0,0,0));
	nodeSceneTransparents->setRotation(vector3df(0,180,0));
	nodeSceneTransparents->setScale(core::vector3df(1,1,1));
	nodeSceneTransparents->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeSceneTransparents->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    nodeSceneTransparents->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    nodeSceneTransparents->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeSceneTransparents->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    // nodeSceneTransparents->setMaterialFlag(video::EMF_GOURAUD_SHADING, false);
    nodeSceneTransparents->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
    // nodeSceneTransparents->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    nodeSceneTransparents->setMaterialFlag(video::EMF_ANTI_ALIASING, true);

	/// Settings and textures to materials:
    for(unsigned int m=0; m<nodeSceneTransparents->getMaterialCount(); m++){
	nodeSceneTransparents->setMaterialTexture(0, driver->getTexture("scene_transparents_01.png"));
	// node->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	// node->getMaterial(m).TextureLayer[0].LODBias = -5; // (Sharp textures) where number is a signed 8bit integer (0 is the default value)
	};
    };


	/// ---------------------------------------------------------------------------------------------------------------------


    /// Create (Loads Object) SCENE DECORATION ONLY:
    if(!nodeSceneDecoration){
    nodeSceneDecoration = smgr->addAnimatedMeshSceneNode(smgr->getMesh("scene_deco_01.obj")); /// This one MUST be ".obj"
    nodeSceneDecoration->setParent(nodeLevel); /// Attach to parent: Level
	nodeSceneDecoration->setPosition(vector3df(0,0,0));
	nodeSceneDecoration->setRotation(vector3df(0,180,0));
	nodeSceneDecoration->setScale(core::vector3df(1,1,1));
	nodeSceneDecoration->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeSceneDecoration->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeSceneDecoration->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    nodeSceneDecoration->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeSceneDecoration->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeSceneDecoration->setMaterialType(video::EMT_SOLID);
    // nodeSceneDecoration->setMaterialType(video::EMT_REFLECTION_2_LAYER);    // EMT_REFLECTION_2_LAYER

    if(true){
    /// Create particles - FOUNTAIN - Water on top:
    scene::IParticleSystemSceneNode* ps;
    scene::IParticleEmitter* em;
    /// Create particle system: (CYLINDER)
    ps = smgr->addParticleSystemSceneNode(true);
    em = ps->createCylinderEmitter(
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &center
    4,    // radius
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &direction
    100,  // length
    true, // outline only
    vector3df(0.0f, 0.200f, 0.0f), // core::vector3df &normal
    80,   // min
    100,  // max
    SColor(255,255,255,255), // video::SColor &minStartColor
    SColor(255,255,255,255), // video::SColor &maxStartColor
    4000, // u32 lifeTimeMin
    9000, // u32 lifeTimeMax
    16,   // Angle
    core::dimension2df( 5.0f,  5.0f), // core::dimension2df &minStartSize
    core::dimension2df(25.0f, 25.0f)  // core::dimension2df &maxStartSize
    );

   	ps->setEmitter(em); // Grabs the emitter
	em->drop(); // We can drop it, without deleting it

    /// Affectors: http://irrlicht.sourceforge.net/docu/classirr_1_1scene_1_1_i_particle_system_scene_node.html
	scene::IParticleAffector* paf1 = ps->createGravityAffector(vector3df(0.0f,-0.15f,0.0f), 2000);
	ps->addAffector(paf1);
	paf1->drop(); // We can drop it, without deleting it

	/// scene::IParticleAffector* paf2 = ps->createScaleParticleAffector(dimension2df(10,10)); // Fade out particles
	/// ps->addAffector(paf2); // Same goes for the affector
	/// paf2->drop();

	ps->setParent(nodeSceneDecoration); // Attach to the node
	ps->setPosition(vector3df(-4700,525,-320));
	ps->setRotation(core::vector3df(0,0,-8)); /// Particles = Angle/2
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("smoke.png"));
	ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	ps->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    };


    if(true){
    /// Create particles - FOUNTAIN - Water on bottom:
    scene::IParticleSystemSceneNode* ps;
    scene::IParticleEmitter* em;
    /// Create particle system: (CYLINDER)
    ps = smgr->addParticleSystemSceneNode(true);
    em = ps->createCylinderEmitter(
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &center
    65,   // radius
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &direction
    150,  // length
    true, // outline only
    vector3df(0.0f, -0.030f, 0.0f), // core::vector3df &normal
    150,  // min
    250,  // max
    SColor(255,255,255,255), // video::SColor &minStartColor
    SColor(255,255,255,255), // video::SColor &maxStartColor
    2000, // u32 lifeTimeMin
    8000, // u32 lifeTimeMax
    0,    // Angle
    core::dimension2df( 5.0f,  5.0f), // core::dimension2df &minStartSize
    core::dimension2df(30.0f, 30.0f)  // core::dimension2df &maxStartSize
    );

   	ps->setEmitter(em); // Grabs the emitter
	em->drop(); // We can drop it, without deleting it

	scene::IParticleAffector* paf1 = ps->createFadeOutParticleAffector(SColor(0,150,150,150), 250); // Fade out particles
	ps->addAffector(paf1);
	paf1->drop(); // We can drop it, without deleting it

	ps->setParent(nodeSceneDecoration); // Attach to the node
	ps->setPosition(vector3df(-4705,346,-320));
	ps->setRotation(core::vector3df(0,0,0));
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("smoke.png"));
	ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	ps->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    };


    if(true){
    /// Create particles - CANDLE in the church:
    scene::IParticleSystemSceneNode* ps;
    scene::IParticleEmitter* em;
    /// Create particle system: (RING)
    ps = smgr->addParticleSystemSceneNode(true);
    em = ps->createRingEmitter(
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &center
    3, // f32 radius
    1, // f32 ringThickness
    vector3df(0.0f, 0.055f, 0.0f), // core::vector3df &direction
    25, // u32 minParticlesPerSecond
    50, // u32 maxParticlesPerSecond
    SColor(255,255,255,255), // video::SColor &minStartColor
    SColor(255,255,255,255), // video::SColor &maxStartColor
    100, // u32 lifeTimeMin
    2000, // u32 lifeTimeMax
    0,   // s32 maxAngleDegrees
    core::dimension2df(10.0f, 10.0f), // core::dimension2df &minStartSize
    core::dimension2df(25.0f, 25.0f)  // core::dimension2df &maxStartSize
    );

   	ps->setEmitter(em); // Grabs the emitter
	em->drop(); // We can drop it, without deleting it

	scene::IParticleAffector* paf1 = ps->createFadeOutParticleAffector(SColor(0,150,150,150), 250); // Fade out particles
	ps->addAffector(paf1);
	paf1->drop(); // We can drop it, without deleting it

	ps->setParent(nodeSceneDecoration); // Attach to the node
	ps->setPosition(vector3df(-6175,725,0)); /// Candle position
	ps->setRotation(core::vector3df(0,0,0));
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("smoke.png"));
	// ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    };

    };


    /// ---------------------------------------------------------------------------------------------------------------------



    /// Create (Loads Object) SCENE OBSTACLES WITH PHYSICS:
    if(!nodeScenePhysics){
	nodeScenePhysics = smgr->addAnimatedMeshSceneNode(smgr->getMesh("scene_physics_01.obj")); /// This one MUST be ".obj"
    nodeScenePhysics->setParent(nodeLevel); /// Attach to parent: Level
	nodeScenePhysics->setPosition(vector3df(0,0,0));
	nodeScenePhysics->setRotation(vector3df(0,180,0));
	nodeScenePhysics->setScale(core::vector3df(1,1,1));
	nodeScenePhysics->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeScenePhysics->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeScenePhysics->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    nodeScenePhysics->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeScenePhysics->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeScenePhysics->setMaterialType(video::EMT_SOLID);
    // nodeScenePhysics->setMaterialType(video::EMT_REFLECTION_2_LAYER);    // EMT_REFLECTION_2_LAYER
    // };


	/// Add to physics world:
	scene::IMesh* meshScene = nodeScenePhysics->getMesh();
	/// Prefer Primitives: http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Shapes
	/// The btBvhTriangleMeshShape is a static-triangle mesh shape, it can only be used for fixed/non-moving objects.
	u32 mass = 0;
	/// Using GImpact for nodeScenePhysics, else the whole mesh will be like a box, in physics world:
	IGImpactMeshShape* shapeScene = new IGImpactMeshShape(nodeScenePhysics, meshScene, mass);
	/// IBvhTriangleMeshShape* shapeScene = new IBvhTriangleMeshShape(nodeScenePhysics, meshScene, mass);
	/// ICollisionShape* shapeScene = new IBoxShape(nodeScenePhysics, mass, true);
	// roadshape = new IBvhTriangleMeshShape(roadNode, roadNode->getMesh(), 0.0);
	// roadshape = new IBvhTriangleMeshShape(roadNode, device->getSceneManager()->getMesh("road.obj"), 0.0);
	shapeScene->setMargin(0.0f); /// <-- This object is the ONLY with margin 0.0!!! So all the other objects could stay in place
	/// shapeScene->calculateLocalInertia(mass, vector3df(0.0,0.0,0.0));
    // The rigid body will be placed at the origin of the node that the collision shape is controlling, so we do not need to set the position after positioning the node.
	IRigidBody* bodyScene = world->addRigidBody(shapeScene, COL_SCENARY, scenaryCollidesWith);
	bodyScene->setDamping(0.0f, 0.0f);
	bodyScene->setRestitution(0.85f); /// > 1.0f!, So the other bodies do not get stuck now with the scene, this repulses them! :D --> Its the bouncing factor (to affect other bodies too)
	bodyScene->setFriction(0.33f);
    bodyScene->setContactProcessingThreshold(btScalar(0.0)); /// <-- For MUCH better collision detection
	/// bodyScene->setCcdValues(0.01f, 0.2f); // CCD = Continuous Collision Detection (void tunneling: motion, radius) - Only needed on fast objects!
	/// bodyScene->setActivationState(EAS_WANTS_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
	bodyScene->getAttributes()->addBool("scene", true);
	// bodyScene->setCollisionFlags(ECollisionFlag::ECF_STATIC_OBJECT); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
	// bodyScene->getPointer()->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT); // btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_KINEMATIC_OBJECT
	bodyScene->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
	// bodyScene->setActivationState(EActivationState::EAS_WANTS_DEACTIVATION);
	// bodyScene->setActivationState(EActivationState::EAS_WANTS_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    // bodyScene->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States


    /// bodyScene->getPointer()->setUserPointer(nodeScenePhysics);
    // bodyScene->getPointer()->setUserPointer((void*)"scenary");

    /// FORCING an physics update:
    /// 1st) Get body transform:
    btTransform trans = bodyScene->getPointer()->getCenterOfMassTransform();
    /// 2nd) Apply new positions to body:
    trans.setOrigin(btVector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ())); /// Apply new position - btVector3(X,Y,Z)
    bodyScene->getPointer()->getMotionState()->setWorldTransform(trans); /// Update physics #1
    bodyScene->getPointer()->setCenterOfMassTransform(trans); /// Update physics #2

    };


};








/// ------------------------------------------------------------------------------------------------------


/// Object for: Flag
struct Flag {
IAnimatedMeshSceneNode* Node;
f32 Speed = 0.0;
};
/// Vars for: Flag
std::vector<Flag> flags;

void Scene::CreateFlags(){

    /// REMOVE from...
    for(unsigned int i=0; i<flags.size(); i++){
    if(flags[i].Node){ flags[i].Node->remove(); }; /// Remove from: Irrlicht world
    };

    /// Reset:
    flags.clear();

    /// Temporary:
    std::vector<vector3df> positions;
    std::vector<f32> speed;

    /// Add flags:
    positions.push_back(vector3df(1200,600,150));
    speed.push_back(2.8);
    positions.push_back(vector3df(2150,833,150));
    speed.push_back(2.1);
    positions.push_back(vector3df(3150,833,150));
    speed.push_back(2.0);
    positions.push_back(vector3df(6000,1100,150));
    speed.push_back(2.6);
    // positions.push_back(vector3df(2680,833,860)); /// Far way on Z
    // speed.push_back(2.0);
    // positions.push_back(vector3df(4215,833,860)); /// Far way on Z
    // speed.push_back(2.8);


    for(unsigned int i=0; i<positions.size(); i++){
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("flag_01.X"));
    node->setParent(nodeLevel); /// Attach to parent: Level
    node->setPosition(vector3df(positions[i]));
    node->setRotation(vector3df(0,0,0));
    node->setScale(vector3df(1.0,1.0,1.0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    // node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    // node->setMaterialType(video::EMT_REFLECTION_2_LAYER);    // EMT_REFLECTION_2_LAYER


    /// Create object:
    Flag f;
    f.Node   = node;
    f.Speed  = speed[i];
    /// Add to global vector:
    flags.push_back(f);
    };
};




/// ------------------------------------------------------------------------------------------------------



/// Object for: Bird
struct Bird {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID = 0;
int Energy = 0;
int Score  = 0;
int Bonus  = 0;
f32 Speed  = 0.0;
bool MovingRight = true;
bool MovingUp = true;
};
/// Vars for: Birds
std::vector<Bird> birds;

void Scene::CreateBirds(){

    /// REMOVE from...
    for(unsigned int i=0; i<birds.size(); i++){
    if(birds[i].Body && birds[i].Body->isInWorld()){ world->removeCollisionObject(birds[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    birds.clear();

    /// Temporary:
    std::vector<vector3df> positions;
    std::vector<f32> speed;

    positions.push_back(vector3df(-700,1300,0));
    speed.push_back(20);

    positions.push_back(vector3df(6000,1200,0));
    speed.push_back(15);


    for(unsigned int i=0; i<positions.size(); i++){
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("bird_01.X"));
    node->setParent(nodeLevel); /// Attach to parent: Level
    node->setPosition(vector3df(positions[i]));
    node->setRotation(vector3df(0,0,0));
    if(i==0){ node->setScale(vector3df(0.8,0.8,0.8)); };
    if(i==1){ node->setScale(vector3df(0.7,0.7,0.7)); };
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    // node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    // node->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    /// Apply texture to my specified material
    for(unsigned int m=0; m<node->getMaterialCount(); m++){
	if(i==0){ node->getMaterial(m).setTexture(0, driver->getTexture("bird_01.jpg")); };
	if(i==1){ node->getMaterial(m).setTexture(0, driver->getTexture("bird_02.jpg")); };
	};


	u32 mass = 0;
	/// Prefer Primitives: http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Shapes
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	shape->calculateLocalInertia(mass, vector3df(0,0,0));
	IRigidBody* body = world->addRigidBody(shape, COL_BIRDS, birdsCollidesWith);
	body->setFriction(0.0f);
	body->setDamping(0.0f, 0.0f);
	body->setRestitution(0.85f); // bouncing factor
    body->setContactProcessingThreshold(btScalar(0.0)); /// For MUCH better collision detection
	// body->setCcdValues(0.01f, 0.2f); // CCD = Continuous Collision Detection (void tunneling: motion, radius) - Only needed on fast objects!
	body->getAttributes()->addBool("bird", true);
    // body->getPointer()->setUserIndex(55);
	body->setLinearFactor(vector3df(0,0,0));  // Allowed positions axis
    body->setAngularFactor(vector3df(0,0,0)); // Allowed rotations axis
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    // body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States

    /// Create object:
    Bird b;
    b.Node   = node;
    b.Body   = body;
    b.BodyID = body->getUniqueID();
    b.Energy = 0;
    b.Score  = 0;
    b.Bonus  = 0;
    b.Speed  = speed[i];
    b.MovingRight = true;
    b.MovingUp = true;
    /// Add to global vector:
    birds.push_back(b);
    };

};



/// ------------------------------------------------------------------------------------------------------



/// Object for: Chicken
struct Chicken {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
bool Colliding = false;
int BodyID   = 0;
f32 Rotation = 0.0;
vector3df StartPos = vector3df(0.0,0.0,0.0);
bool MoveRight = true;
f32 MinPosX  = 0.0;
f32 MaxPosX  = 0.0;
f32 PosY     = 0.0;
f32 VelX     = 0.0;
f32 VelY     = 0.0;
};
/// Vars for: Chickens
std::vector<Chicken> chickens;

void Scene::CreateChickens(){

    /// REMOVE from...
    for(unsigned int i=0; i<chickens.size(); i++){
    if(chickens[i].Body && chickens[i].Body->isInWorld()){ world->removeCollisionObject(chickens[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    chickens.clear();

    /// Temporary:
    std::vector<vector3df> StartPos;
    std::vector<f32> MinPosX;
    std::vector<f32> MaxPosX;
    std::vector<f32> VelX;

    /// Chickens:
	StartPos.push_back(vector3df(4200,220,0)); /// (galo)
	MinPosX.push_back(4200); MaxPosX.push_back(5000); VelX.push_back(5);

    StartPos.push_back(vector3df(4200,220,10)); /// Different Z (galinha)
	MinPosX.push_back(4200); MaxPosX.push_back(5000); VelX.push_back(7);

	StartPos.push_back(vector3df(4200,220,80)); /// Different Z (pintos)
	MinPosX.push_back(4200); MaxPosX.push_back(5650); VelX.push_back(10);



    for(unsigned int i=0; i<StartPos.size(); i++){
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("chicken_01.X"));
    node->setParent(nodeLevel); /// Attach to parent: Level
    node->setPosition(vector3df(StartPos[i]));
    node->setRotation(vector3df(0,0,0));
    if(i==0){ node->setScale(vector3df(1.2,1.2,1.2)); };
    if(i==1){ node->setScale(vector3df(1.0,1.0,1.0)); };
    if(i==2){ node->setScale(vector3df(0.6,0.6,0.6)); };
    // if(i==3){ node->setScale(vector3df(0.6,0.6,0.6)); };
    // if(i==4){ node->setScale(vector3df(0.6,0.6,0.6)); };
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    // node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    // node->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    /// Apply texture to my specified material
    for(unsigned int m=0; m<node->getMaterialCount(); m++){
    if(i==0){ node->getMaterial(m).setTexture(0, driver->getTexture("chicken_03.jpg")); };
	if(i==1){ node->getMaterial(m).setTexture(0, driver->getTexture("chicken_02.jpg")); };
	if(i==2){ node->getMaterial(m).setTexture(0, driver->getTexture("chicken_04.jpg")); };
	};


	u32 mass = 10;
	/// Prefer Primitives: http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Shapes
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	shape->calculateLocalInertia(mass, vector3df(0,0,0));
	IRigidBody* body = world->addRigidBody(shape, COL_CHICKENS, chickensCollidesWith);
	body->setFriction(0.0f);
	body->setDamping(0.005f, 0.050f);
	body->setRestitution(0.95f); // bouncing factor
    body->setContactProcessingThreshold(btScalar(0.0)); /// For MUCH better collision detection
	// body->setCcdValues(0.01f, 0.2f); // CCD = Continuous Collision Detection (void tunneling: motion, radius) - Only needed on fast objects!
	body->getAttributes()->addBool("chicken", true);
    // body->getPointer()->setUserIndex(55);
	body->setLinearFactor(vector3df(1,1,0));  // Allowed positions axis
    body->setAngularFactor(vector3df(0,1,0)); // Allowed rotations axis
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    // body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States

    /// Create object:
    Chicken c;
    c.Node      = node;
    c.Body      = body;
    c.BodyID    = body->getUniqueID();
    c.Colliding = false;
    c.Rotation  = 0.0;
    c.StartPos  = StartPos[i];
    c.MoveRight = true;
    c.MinPosX   = MinPosX[i];
    c.MaxPosX   = MaxPosX[i];
    c.PosY      = 0.0;
    c.VelX      = VelX[i];
    c.VelY      = 5.0;
    /// Add to global vector:
    chickens.push_back(c);
    };

};


/// ------------------------------------------------------------------------------------------------------


/// Object for: Jail
struct Jail {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID     = 0;
int Energy     = 100;
int JailNumber = 0; /// Will be: 0 to 3 (To match the PrisonerNumber)
};
/// Vars for: Jail
std::vector<Jail> jails;


void Scene::RemoveJails(){

    /// REMOVE from...
    for(unsigned int i=0; i<jails.size(); i++){
    if(jails[i].Body && jails[i].Body->isInWorld()){ world->removeCollisionObject(jails[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    jails.clear();

};

void Scene::CreateJails(){

    /// REMOVE from...
    RemoveJails();

    /// Temporary:
    std::vector<vector3df> positions;
    positions.push_back(vector3df(2161,470,0)); // Near prisoner
    positions.push_back(vector3df(3161,240,0)); // Middle bottom prisoner
    positions.push_back(vector3df(3161,701,0)); // Middle upper prisoner
    positions.push_back(vector3df(6000,240,0)); // Far prisoner

    for(unsigned int i=0; i<positions.size(); i++){
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("jail_01.X"));
    node->setName("jail");
    node->setParent(nodeLevel); /// Attach to parent: Level
    node->setScale(vector3df(1.0,1.0,1.0));
    node->setPosition(vector3df(positions[i]));
    node->setRotation(vector3df(0,0,0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    // node->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    // node->setMaterialType(video::EMT_SOLID);

    // node->setMaterialType(video::EMT_TRANSPARENT_REFLECTION_2_LAYER);
    /// smgr->getMeshManipulator()->setVertexColorAlpha(smgr->getMesh("jail_01.X"), 100); /// <-- Cool effect. 0 is transparent and 255 is solid
    /// node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR); /// <-- Cool effect.
    // node->getMaterial(0).setTexture(0, driver->getTexture("jail_01.png")); /// Normal
    // node->getMaterial(1).setTexture(0, driver->getTexture("reflex_01.jpg")); /// Reflex

    /// Apply texture to my specified material
    for(unsigned int m=0; m<node->getMaterialCount(); m++){
	node->getMaterial(m).setTexture(0, driver->getTexture("jail_01A.png"));
	/// node->getMaterial(m).ZWriteEnable = false; /// For correct transparency
	};


	u32 mass = 0;
	/// Prefer Primitives: http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Shapes
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	shape->calculateLocalInertia(mass, vector3df(0,0,0));
	IRigidBody* body = world->addRigidBody(shape, COL_JAILS, jailsCollidesWith);
	body->setFriction(0.5f);
	body->setDamping(0.0f, 0.0f);
	body->setRestitution(0.85f); // bouncing factor
    body->setContactProcessingThreshold(btScalar(0.0)); /// <-- For MUCH better collision detection:
	// body->setCcdValues(0.01f, 0.2f); // CCD = Continuous Collision Detection (void tunneling: motion, radius) - Only needed on fast objects!
	body->getAttributes()->addBool("jail", true);
    // body->getPointer()->setUserIndex(55);
	body->setLinearFactor(vector3df(0,0,0));  // Allowed positions axis
    body->setAngularFactor(vector3df(0,0,0)); // Allowed rotations axis
    // body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    // body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    body->setCollisionFlags(ECollisionFlag::ECF_STATIC_OBJECT); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;

    /// Create object:
    Jail j;
    j.Node   = node;
    j.Body   = body;
    j.BodyID = body->getUniqueID();
    j.Energy = 100;
    j.JailNumber = i;  /// Will be: 0 to 3
    /// Add to global vector:
    jails.push_back(j);
    };

};


/// ------------------------------------------------------------------------------------------------------


/// STRUCT for PRISONERS:
struct Prisoner {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID   = 0;
f32 Rotation = 0.0;
f32 VelX     = 0.0;
f32 VelY     = 0.0;
f32 VelZ     = 0.0;
int PrisonerNumber   = 0; /// Will be: 0 to 3 (To match the JailNumber)
bool AttachParticles = false;
bool MoveFront       = true;
bool MoveRight       = false;
bool Free            = false;
};
/// GLOBAL:
std::vector<Prisoner> prisoners;



void Scene::RemovePrisoners(){

    /// REMOVE from...
    for(unsigned int i=0; i<prisoners.size(); i++){
    if(prisoners[i].Body && prisoners[i].Body->isInWorld()){ world->removeCollisionObject(prisoners[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    prisoners.clear();

};



void Scene::CreatePrisoners(){

    /// Remove:
    RemovePrisoners();

    /// Temporary:
    std::vector<vector3df> positions;
    positions.push_back(vector3df(2160,440,0));
    positions.push_back(vector3df(3145,235,0));
    positions.push_back(vector3df(3145,695,0));
    positions.push_back(vector3df(6010,235,0));


    /// NODES: Prisoners
    for(unsigned int i=0; i<positions.size(); i++){
	scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("prisoner_01.X"));
	node->setParent(nodeLevel); /// Attach to parent: Level
	node->setName("prisoner");
	node->setPosition(positions[i]);
	node->setRotation(vector3df(0,0,0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_SOLID);
    // nodeSoldier[i]->setMaterialType(video::EMT_REFLECTION_2_LAYER);    // EMT_REFLECTION_2_LAYER
    /// Apply texture to my specified material
    for(unsigned int m=0; m<node->getMaterialCount(); m++){
	node->getMaterial(m).setTexture(0, driver->getTexture("prisoner_01A.jpg"));
	};

	/// Get personage text name and attach it: TODO: On remove prisoner, if he has a name, it CRASHES !!!...
	if(NAMES_TEXTS==true){
    /*
	if(i==0){ personageName[20]->setParent(node); personageName[20]->setPosition(vector3df(0,70,0)); };
	if(i==1){ personageName[21]->setParent(node); personageName[21]->setPosition(vector3df(0,70,0)); };
	if(i==2){ personageName[22]->setParent(node); personageName[22]->setPosition(vector3df(0,70,0)); };
	if(i==3){ personageName[23]->setParent(node); personageName[23]->setPosition(vector3df(0,70,0)); };
	*/
	};


	// IGImpactMeshShape* wShape1 = new IGImpactMeshShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); /// IConvexHullShape* wShape1 = new IConvexHullShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0); /// IBoxShape* wShape1 = new IBoxShape(rodaNode1, 10.0, true);
	/// scene::IMesh* raycastSoldier = smgr->getMesh("soldier.obj");
	/// shape = new IBvhTriangleMeshShape(node, raycastSoldier, 80);
	/// shape = new IBoxShape(node, 80, false);
	/// ICollisionShape* shape = new IGImpactMeshShape(node, device->getSceneManager()->getMesh("soldier.obj"), false);
	u32 mass = 50;
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.25,1.25,1.25), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	/// shape->getPointer()->getAabb(); /// Get shape dimensions
	shape->calculateLocalInertia(mass, vector3df(0,0,0));
	IRigidBody* body = world->addRigidBody(shape, COL_PRISONERS, prisonersCollidesWith);
	body->setFriction(0.0f);
	body->setDamping(0.001f, 0.001f);
	body->setRestitution(0.95f); /// Its the bouncing factor (to affect other bodies too)
    body->setContactProcessingThreshold(btScalar(0.0)); /// For MUCH better collision detection
	body->getAttributes()->addBool("prisoner", true);
	/// body->setLinearVelocity(vector3df(0,10,0));
	body->setLinearFactor(vector3df(0,1,0));  // Allowed positions axis
    body->setAngularFactor(vector3df(0,0,0)); // Allowed rotations axis
    // body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States

    /// Create object:
    Prisoner p;
    p.Node       = node;
    p.Body       = body;
    p.BodyID     = body->getUniqueID();
    p.Rotation   = 0.00;
    p.VelX       = 25.0;
    p.VelY       = 0.00;
    p.VelZ       = 0.01;
    p.PrisonerNumber  = i;
    p.AttachParticles = false;
    p.MoveFront       = true;
    p.MoveRight       = false;
    p.Free            = false;

    /// For DEBUG only:
    // if(i==0){ p.Free = true; };
    // if(i==1){ p.Free = true; };
    // if(i==2){ p.Free = true; };
    // if(i==3){ p.Free = true; };

    /// Add to object vector:
    prisoners.push_back(p);
    };

};



/// ------------------------------------------------------------------------------------------------------


/// Object struct for: TARGETS
struct Target {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID = 0;
int Score  = 0;
};
std::vector<Target> targets;

void Scene::CreateTargets(){

    /// REMOVE from...
    for(unsigned int i=0; i<targets.size(); i++){
    if(targets[i].Body && targets[i].Body->isInWorld()){ world->removeCollisionObject(targets[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    targets.clear();

    /// NODES: Targets
    std::vector<vector3df> positions;
    positions.push_back(vector3df(2305,700,0));
	positions.push_back(vector3df(2930,250,0));


    for(unsigned int i=0; i<positions.size(); i++){
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("target_01.X"));
	node->setParent(nodeLevel); /// Attach to parent: Level
	node->setPosition(vector3df(positions[i]));
	node->setRotation(vector3df(0,90,0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_SOLID);
    // node->setMaterialType(video::EMT_REFLECTION_2_LAYER);    // EMT_REFLECTION_2_LAYER

    // IGImpactMeshShape* wShape1 = new IGImpactMeshShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); /// IConvexHullShape* wShape1 = new IConvexHullShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0); /// IBoxShape* wShape1 = new IBoxShape(rodaNode1, 10.0, true);
	/// scene::IMesh* raycastSoldier = smgr->getMesh("king.obj");
	/// shape = new IBvhTriangleMeshShape(node, raycastSoldier, 80);
	/// shape = new IBoxShape(node, 80, false);
	/// ICollisionShape* shape = new IGImpactMeshShape(node, device->getSceneManager()->getMesh("king.obj"), false);
	u32 mass = 0;
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	shape->calculateLocalInertia(mass, vector3df(0,0,0));
    // The rigid body will be placed at the origin of the node that the collision shape is controlling, so we do not need to set the position after positioning the node.
	IRigidBody* body = world->addRigidBody(shape, COL_TARGETS, targetsCollidesWith);
	body->setRestitution(0.0f); /// Its the bouncing factor (to affect other bodies too)
	body->setFriction(0.0f);
	body->setDamping(0.0f, 0.0f);
    body->setContactProcessingThreshold(btScalar(0.0)); /// <-- For MUCH better collision detection:
	/// body->setCcdValues(0.01f, 0.2f); // CCD = Continuous Collision Detection (void tunneling: motion, radius) - Only needed on fast objects!
	// body->setName("target");
	body->getAttributes()->addBool("target", true);
	// body->getAttributes()->addString("target", "target");
	// body->getAttributes()->addBool("collide", true);
	// body->getAttributes()->addUserPointer("target", 0);
	body->setLinearFactor(vector3df(0,0,0));  // Allowed positions axis
    body->setAngularFactor(vector3df(0,0,0)); // Allowed rotations axis
    // body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    // body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States
    body->setCollisionFlags(ECollisionFlag::ECF_STATIC_OBJECT); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;

    /// Create object:
    Target ta;
    ta.Node   = node;
    ta.Body   = body;
    ta.BodyID = body->getUniqueID();
    ta.Score  = 100;
    /// Add to global vector:
    targets.push_back(ta);
    };
};




/// ------------------------------------------------------------------------------------------------------


/// Objects struct for: BOXES
struct Box {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID = 0;
int Energy = 0;
int Score  = 0;
int Bonus  = 0;
vector3df Positions = vector3df(0.0,0.0,0.0);
};
std::vector<Box> boxes;


void Scene::RemoveBoxes(){

    /// REMOVE from...
    for(unsigned int i=0; i<boxes.size(); i++){
    if(boxes[i].Body && boxes[i].Body->isInWorld()){ world->removeCollisionObject(boxes[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };
    /// Reset:
    boxes.clear();

};


void Scene::CreateBoxes(){

    /// REMOVE from...
    RemoveBoxes();

    /// Temporary:
    std::vector<vector3df> positions;
    std::vector<int> bonus;

    /// NODES: Boxes (boxes height and width = 68)
    /// Boxes Bonus: 0 = Nothing, 1 = Extra ball, 3 = Score

    positions.push_back(vector3df(1200,544,0)); bonus.push_back(0);
    positions.push_back(vector3df(1250,544,0)); bonus.push_back(2);
    positions.push_back(vector3df(1150,476,0)); bonus.push_back(2);
    positions.push_back(vector3df(1200,476,0)); bonus.push_back(0);
    positions.push_back(vector3df(1250,476,0)); bonus.push_back(1);
    positions.push_back(vector3df(1300,476,0)); bonus.push_back(0);
    positions.push_back(vector3df(1150,408,0)); bonus.push_back(1);
    positions.push_back(vector3df(1200,408,0)); bonus.push_back(0);
    positions.push_back(vector3df(1250,408,0)); bonus.push_back(1);
    positions.push_back(vector3df(1300,408,0)); bonus.push_back(2);

    // positions.push_back(vector3df(2100,748,0)); bonus.push_back(2);
    // positions.push_back(vector3df(2150,748,0)); bonus.push_back(0);
    // positions.push_back(vector3df(2050,680,0)); bonus.push_back(1);
    // positions.push_back(vector3df(2100,680,0)); bonus.push_back(0);
    // positions.push_back(vector3df(2150,680,0)); bonus.push_back(0);
    // positions.push_back(vector3df(2200,680,0)); bonus.push_back(2);
	// positions.push_back(vector3df(2050,612,0)); bonus.push_back(2);
    // positions.push_back(vector3df(2100,612,0)); bonus.push_back(0);
    // positions.push_back(vector3df(2150,612,0)); bonus.push_back(0);
    // positions.push_back(vector3df(2200,612,0)); bonus.push_back(0);

    positions.push_back(vector3df(3060,442,0)); bonus.push_back(2);
    positions.push_back(vector3df(3110,442,0)); bonus.push_back(0);
    positions.push_back(vector3df(3160,442,0)); bonus.push_back(1);
    positions.push_back(vector3df(3060,374,0)); bonus.push_back(2);
    positions.push_back(vector3df(3110,374,0)); bonus.push_back(0);
    positions.push_back(vector3df(3160,374,0)); bonus.push_back(1);
    positions.push_back(vector3df(3210,374,0)); bonus.push_back(0);
    positions.push_back(vector3df(3060,316,0)); bonus.push_back(1);
    positions.push_back(vector3df(3110,316,0)); bonus.push_back(0);
    positions.push_back(vector3df(3160,316,0)); bonus.push_back(1);
    positions.push_back(vector3df(3210,316,0)); bonus.push_back(2);


    srand(time(0)); /// Random initialize

    for(unsigned int i=0; i<positions.size(); i++){

    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("box_01.X"));
    node->setParent(nodeLevel); /// Attach to parent: Level
    node->setPosition(vector3df(positions[i]));
    // f32 rotY = ; /// Random rotation on Y
    node->setRotation(vector3df(0, f32(rand()%90), 0));
    node->setScale(vector3df(1.0,1.0,1.0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_SOLID);
    // node->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER

    if(bonus[i]==0){ node->getMaterial(0).setTexture(0, driver->getTexture("box_01.jpg")); }; /// Bonus: Nothing
    if(bonus[i]==1){ node->getMaterial(0).setTexture(0, driver->getTexture("box_02.jpg")); }; /// Bonus: Extra Ball
    if(bonus[i]==2){ node->getMaterial(0).setTexture(0, driver->getTexture("box_03.jpg")); }; /// Bonus: Score



    // IGImpactMeshShape* wShape1 = new IGImpactMeshShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); /// IConvexHullShape* wShape1 = new IConvexHullShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0); /// IBoxShape* wShape1 = new IBoxShape(rodaNode1, 10.0, true);
	/// shapeBox[i] = new IBoxShape(nodeBox[i], 20, true);
	u32 mass = 10;
	/// Prefer Primitives: http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Shapes
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f); /// 1.5 --> To overlap the other boxes, so they all could be activated on ball impact, too.
	vector3df inertia = vector3df(0.0,0.0,0.0); /// The inertia value
	shape->calculateLocalInertia(mass, inertia);
	IRigidBody* body = world->addRigidBody(shape, COL_BOXES, boxesCollidesWith);
	body->setFriction(0.5f);
	body->setDamping(0.0f, 0.0f);
	body->setRestitution(0.85f); // bouncing factor
	// body->getPointer()->setDeactivationTime(0);
	// body->getPointer()->setSleepingThresholds(5.4, 5.6);
	// body->getPointer()->setContactProcessingThreshold(0.25f);
    body->getPointer()->setContactProcessingThreshold(btScalar(0.0)); /// <-- For MUCH better collision detection:
	body->getPointer()->setCcdMotionThreshold(1.0f);
	body->getPointer()->setCcdSweptSphereRadius(1.0f);

	// body->setCcdValues(0.01f, 0.2f); // CCD = Continuous Collision Detection (void tunneling: motion, radius) - Only needed on fast objects!
	// body->setName("box");
	body->getAttributes()->addBool("box", true);
	body->getAttributes()->addInt("box_bonus", bonus[i]);
    // body->getPointer()->setUserIndex(55);
	body->setLinearFactor( vector3df(1,1,1)); // Allowed positions axis
    body->setAngularFactor(vector3df(1,1,1)); // Allowed rotations axis
    // body->setLinearVelocity( vector3df(0,0,0), ERBTransformSpace::ERBTS_WORLD);
    // body->setAngularVelocity(vector3df(0,0,0), ERBTransformSpace::ERBTS_WORLD);
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->getPointer()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT); // <-- http://bulletphysics.org/Bullet/BulletFull/classbtCollisionObject.html#a020d84cc1ef3199bd68a036274519ea4
    // body->setCollisionFlags(ECollisionFlag::ECF_STATIC_OBJECT); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States

    /// IMPORTANT: Apply position: TODO: Apply in all other objects
    /*
    btTransform trans = body->getPointer()->getCenterOfMassTransform();
    vector3df pos(node->getPosition());
    trans.setOrigin(irrlichtToBulletVector(vector3df(pos))); /// Y goes up or down according cannon rotation
    trans.setRotation(btQuaternion(irrlichtToBulletVector(vector3df(1,1,1)), 0)); /// To match directions with irrlicht!
    body->getPointer()->setCenterOfMassTransform(trans);
    */


    /// Object:
    Box b;
    b.Node      = node;
    b.Body      = body;
    b.BodyID    = body->getUniqueID();
    b.Energy    = 10;
    b.Score     = 100;
    b.Bonus     = bonus[i];
    b.Positions = vector3df(positions[i]);
    /// Add to object vector:
    boxes.push_back(b);
    };
};



/// ------------------------------------------------------------------------------------------------------


/// Objects struct for: BELLS
struct Bell {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID = 0;
vector3df Positions = vector3df(0.0,0.0,0.0);
bool MovingRight = true;
};
std::vector<Bell> bells;

void Scene::CreateBells(){

    /// REMOVE from...
    for(unsigned int i=0; i<bells.size(); i++){
    if(bells[i].Body && bells[i].Body->isInWorld()){ world->removeCollisionObject(bells[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    bells.clear();

    /// Temporary:
    std::vector<vector3df> positions;

    positions.push_back(vector3df(6000,1080,0)); /// For constraint only: Just for the dummy body
    positions.push_back(vector3df(0,0,0));       /// The bell


    for(unsigned int i=0; i<positions.size(); i++){

    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("bell_01.X"));
    node->setName("bell");
    node->setParent(nodeLevel); /// Attach to parent: Level
    node->setPosition(vector3df(positions[i]));
    node->setRotation(vector3df(0,0,0));
    node->setScale(vector3df(1.0,1.0,1.0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    // node->setMaterialType(video::EMT_SOLID);
    node->setMaterialType(video::EMT_REFLECTION_2_LAYER); /// EMT_REFLECTION_2_LAYER

    node->getMaterial(0).setTexture(0, driver->getTexture("bell_01.jpg"));   /// Normal
    node->getMaterial(1).setTexture(0, driver->getTexture("reflex_01.jpg")); /// Reflex


	u32 mass = 80;
	if(i==0){ mass = 0; }; /// For constraint only: The 1st is just a dummy body --> The first body must have 0 (so it will be fixed in the air)!!
	/// Prefer Primitives: http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Shapes
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	if(i==0){ shape->setLocalScaling(vector3df(0.0,0.0,0.0), EScalingPair::ESP_BOTH); }; /// For constraint only: The 1st is just a dummy body --> ESP_BOTH = Scales: Node + Physics shape!!
	shape->setMargin(1.0f);
	vector3df inertia = vector3df(0.0,0.0,0.0); /// The inertia value
	shape->calculateLocalInertia(mass, inertia);
	IRigidBody* body = world->addRigidBody(shape, COL_BELLS, bellsCollidesWith);
	body->setFriction(0.5f);
	body->setDamping(0.020f, 0.020f); /// For constraint only: Must have values, else the spring movement will never stop!!
	body->setRestitution(0.85f); // bouncing factor

	// body->setCcdValues(0.01f, 0.2f); // CCD = Continuous Collision Detection (void tunneling: motion, radius) - Only needed on fast objects!
	body->getAttributes()->addBool("bell", true);
    // body->getPointer()->setUserIndex(55);
	body->setLinearFactor( vector3df(1,1,0)); // Allowed positions axis
    body->setAngularFactor(vector3df(0,0,1)); // Allowed rotations axis
    // body->setLinearVelocity( vector3df(0,0,0), ERBTransformSpace::ERBTS_WORLD);
    // body->setAngularVelocity(vector3df(0,0,0), ERBTransformSpace::ERBTS_WORLD);
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->getPointer()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT); // <-- http://bulletphysics.org/Bullet/BulletFull/classbtCollisionObject.html#a020d84cc1ef3199bd68a036274519ea4
    // body->setCollisionFlags(ECollisionFlag::ECF_STATIC_OBJECT); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States


    /// Object:
    Bell b;
    b.Node      = node;
    b.Body      = body;
    b.BodyID    = body->getUniqueID();
    b.Positions = vector3df(positions[i]);
    /// Add to object vector:
    bells.push_back(b);
    };


    /// CONSTRAINT: Code for Bell effect
    for(unsigned int i=0; i<bells.size()-1; i++){

    /// 1) Parent body:
	btRigidBody* bodyA = bells[i+0].Body->getPointer(); // parentObject->getRigidBody()->getPointer();
    /// 2) Child body: (Will be attached to the previous one)
    btRigidBody* bodyB = bells[i+1].Body->getPointer(); // parentObject->getRigidBody()->getPointer();

    /// 1) Create transform: (Affects the previous body N.1)
    btTransform transA;
    transA.setIdentity();
    transA.setOrigin(btVector3(0,0,0));
    transA.getBasis().setEulerZYX(0,0,0);
    /// 2) Create transform: (Affects the previous body N.2)
    btTransform transB;
    transB.setIdentity();
    transB.setOrigin(btVector3(transA.getOrigin().getX(), transA.getOrigin().getY()+130, transA.getOrigin().getZ()));
    transA.getBasis().setEulerZYX(0,0,0);

    /// Create the constraint:
    btGeneric6DofSpringConstraint* spring = new btGeneric6DofSpringConstraint(*bodyA, *bodyB, transA, transB, true);
    /// Set constraint limits:
    spring->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0,0,0)));   // Set limits
    spring->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,0,0)));   // Set limits (Y bouncing)
    spring->setAngularUpperLimit(irrlichtToBulletVector(vector3df(0,0, 0.20))); // Set angle limits
    spring->setAngularLowerLimit(irrlichtToBulletVector(vector3df(0,0,-0.20))); // Set angle limits
    spring->setEquilibriumPoint();
    /// Add the constraint to the physics world:
    world->getPointer()->addConstraint(spring, true);

    };


    /// CONSTRAINT: Code for Rope effect
    /*
    for(u32 i=0; i<bells.size()-1; i++){
    /// Get bodies and transforms:
    btTransform transA = btTransform::getIdentity();
    transA.setOrigin(btVector3(0,0,0));

    btTransform transB = btTransform::getIdentity();
    f32 moreY = transA.getOrigin().getY()+120;
    transB.setOrigin(btVector3(0,moreY,0));

    btRigidBody* bodyA = bells[i+0].Body->getPointer(); // parentObject->getRigidBody()->getPointer();
    btRigidBody* bodyB = bells[i+1].Body->getPointer(); // parentObject->getRigidBody()->getPointer();

    /// Create constraints:
    btGeneric6DofSpringConstraint* spring = new btGeneric6DofSpringConstraint(*bodyA, *bodyB, transA, transB, true);
    spring->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0,0,0)));   // Set limits
    spring->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,0.2,0)));   // Set limits (Y bouncing)
    spring->setAngularUpperLimit(irrlichtToBulletVector(vector3df(0,0, 0.3))); // Set limits
    spring->setAngularLowerLimit(irrlichtToBulletVector(vector3df(0,0,-0.3))); // Set limits
    /// Enabling the spring behavior for the Y coordinate (index = 1)
    spring->enableSpring(1,  true);
    spring->setStiffness(1, 50.0f);
    spring->setDamping  (1, 0.01f);
    spring->setEquilibriumPoint();

    /// Add then to the world:
    world->getPointer()->addConstraint(spring, true);
    };
    */


};



/// ------------------------------------------------------------------------------------------------------


/// Objects struct for: CHAINS
struct Chain {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID = 0;
bool MovingRight = true;
vector3df Positions = vector3df(0.0,0.0,0.0);
};
std::vector<Chain> chains;

void Scene::CreateChains(){

    /// REMOVE from...
    for(unsigned int i=0; i<chains.size(); i++){
    if(chains[i].Body && chains[i].Body->isInWorld()){ world->removeCollisionObject(chains[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    chains.clear();

    /// Temporary:
    std::vector<vector3df> positions;

    f32 posX = 5400; /// First chain must match --> Animations::PlatformLoop() !!!
    f32 posY = 1020; /// First chain must match --> Animations::PlatformLoop() !!!

    positions.push_back(vector3df(posX,posY,0));
    posY-=30;
    positions.push_back(vector3df(posX,posY,0));
    posY-=30;
    positions.push_back(vector3df(posX,posY,0));


    for(unsigned int i=0; i<positions.size(); i++){

    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("chain_01.X"));
    node->setParent(nodeLevel); /// Attach to parent: Level
    node->setPosition(vector3df(positions[i]));
    // f32 rotY = ; /// Random rotation on Y
    node->setRotation(vector3df(0, 0, 0));
    node->setScale(vector3df(1.0,1.0,1.0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    // node->setMaterialType(video::EMT_SOLID);
    node->setMaterialType(video::EMT_REFLECTION_2_LAYER); /// EMT_REFLECTION_2_LAYER

    node->getMaterial(0).setTexture(0, driver->getTexture("chain_01.jpg"));  /// Normal
    node->getMaterial(1).setTexture(0, driver->getTexture("reflex_01.jpg")); /// Reflex



	f32 mass = 1.0;
	if(i==0){ mass = 0.0; }; /// The first body must have 0 (so it will be fixed in the air)
	/// Prefer Primitives: http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Shapes
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	vector3df inertia = vector3df(0.0,0.0,0.0); /// The inertia value
	shape->calculateLocalInertia(mass, inertia);
	IRigidBody* body = world->addRigidBody(shape, COL_CHAINS, chainsCollidesWith);
	body->setFriction(0.5f);
	body->setDamping(0.05f, 0.05f);
	body->setRestitution(0.85f); // bouncing factor

	body->getAttributes()->addBool("chain", true);
	body->setLinearFactor( vector3df(1,1,0)); // Allowed positions axis
    body->setAngularFactor(vector3df(1,1,1)); // Allowed rotations axis
    // body->setLinearVelocity( vector3df(0,0,0), ERBTransformSpace::ERBTS_WORLD);
    // body->setAngularVelocity(vector3df(0,0,0), ERBTransformSpace::ERBTS_WORLD);
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->getPointer()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT); // <-- http://bulletphysics.org/Bullet/BulletFull/classbtCollisionObject.html#a020d84cc1ef3199bd68a036274519ea4
    // body->setCollisionFlags(ECollisionFlag::ECF_STATIC_OBJECT); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States

    /// Object:
    Chain c;
    c.Node      = node;
    c.Body      = body;
    c.BodyID    = body->getUniqueID();
    c.Positions = vector3df(positions[i]);
    /// Add to object vector:
    chains.push_back(c);
    };



    for(unsigned int i=0; i<chains.size()-1; i++){
    /// Get bodies and transforms:
    btTransform transA = btTransform::getIdentity();
    transA.setOrigin(btVector3(0,0,0));

    btTransform transB = btTransform::getIdentity();
    f32 moreY = transA.getOrigin().getY()+30;
    transB.setOrigin(btVector3(0,moreY,0));

    btRigidBody* bodyA = chains[i+0].Body->getPointer(); // parentObject->getRigidBody()->getPointer();
    btRigidBody* bodyB = chains[i+1].Body->getPointer(); // parentObject->getRigidBody()->getPointer();

    /// Create constraints:
    btGeneric6DofSpringConstraint* con = new btGeneric6DofSpringConstraint(*bodyA, *bodyB, transA, transB, true);
    con->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0,0,0)));     // Set limits
    con->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,0,0)));     // Set limits
    con->setAngularUpperLimit(irrlichtToBulletVector(vector3df(0,0, 0.001))); // Set limits
    con->setAngularLowerLimit(irrlichtToBulletVector(vector3df(0,0,-0.001))); // Set limits

    /// Add then to the world:
    world->getPointer()->addConstraint(con, true);
    };

};



/// ------------------------------------------------------------------------------------------------------


/// Objects struct for: CHAINS PLATFORMS
struct Platform {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID = 0;
vector3df Positions = vector3df(0.0,0.0,0.0);
};
std::vector<Platform> platforms;

void Scene::CreatePlatforms(){

    /// REMOVE from...
    for(unsigned int i=0; i<platforms.size(); i++){
    if(platforms[i].Body && platforms[i].Body->isInWorld()){ world->removeCollisionObject(platforms[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    platforms.clear();

    /// Get parent data: (Data from last chain)
    if(!chains.empty()){
    int lastchain = chains.size()-1;
    vector3df lastchainPos = chains[lastchain].Positions;
    vector3df pos = vector3df(lastchainPos.X, lastchainPos.Y+420, lastchainPos.Z);


    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("chain_platform_01.X"));
    node->setParent(nodeLevel); /// Attach to parent: Level
    node->setPosition(vector3df(pos));
    node->setRotation(vector3df(0.0,45.0,0.0));
    node->setScale(vector3df(1.0,1.0,1.0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_SOLID);
    // node->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER


	scene::IMesh* mesh = node->getMesh();
	f32 mass = 90.0;
	/// Prefer Primitives: http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Shapes
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	/// IGImpactMeshShape* shape = new IGImpactMeshShape(node, mesh, mass);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	vector3df inertia = vector3df(0.0,0.0,0.0); /// The inertia value
	shape->calculateLocalInertia(mass, inertia);
	IRigidBody* body = world->addRigidBody(shape, COL_PLATFORMS, platformsCollidesWith);
	body->setFriction(0.5f);
	body->setDamping(0.05f, 0.05f);
	body->setRestitution(0.85f); // bouncing factor

	body->getAttributes()->addBool("platform", true);
	body->setLinearFactor( vector3df(1,1,0)); // Allowed positions axis
    body->setAngularFactor(vector3df(1,1,1)); // Allowed rotations axis
    // body->setLinearVelocity( vector3df(0,0,0), ERBTransformSpace::ERBTS_WORLD);
    // body->setAngularVelocity(vector3df(0,0,0), ERBTransformSpace::ERBTS_WORLD);
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->getPointer()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT); // <-- http://bulletphysics.org/Bullet/BulletFull/classbtCollisionObject.html#a020d84cc1ef3199bd68a036274519ea4
    // body->setCollisionFlags(ECollisionFlag::ECF_STATIC_OBJECT); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States

    /// Object:
    Platform cp;
    cp.Node      = node;
    cp.Body      = body;
    cp.BodyID    = body->getUniqueID();
    cp.Positions = vector3df(pos);
    /// Add to object vector:
    platforms.push_back(cp);


    /// Get bodies and transforms:
    btTransform transA = btTransform::getIdentity();
    transA.setOrigin(btVector3(0,0,0));

    btTransform transB = btTransform::getIdentity();
    f32 moreY = transA.getOrigin().getY()+420;
    transB.setOrigin(btVector3(0,moreY,0));

    btRigidBody* bodyA = chains[lastchain].Body->getPointer(); // parentObject->getRigidBody()->getPointer();
    btRigidBody* bodyB = platforms[0].Body->getPointer(); // parentObject->getRigidBody()->getPointer();

    /// Create constraints:
    btGeneric6DofSpringConstraint* con = new btGeneric6DofSpringConstraint(*bodyA, *bodyB, transA, transB, true);
    con->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0,0,0)));     // Set limits
    con->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,0,0)));     // Set limits
    con->setAngularUpperLimit(irrlichtToBulletVector(vector3df(0,0, 0.001))); // Set limits
    con->setAngularLowerLimit(irrlichtToBulletVector(vector3df(0,0,-0.001))); // Set limits

    /// Add then to the world:
    world->getPointer()->addConstraint(con, true);
    };

};



/// ------------------------------------------------------------------------------------------------------


/// Objects struct for: CHAINS DECORATIONS (no physics applied)
struct ChainDecorations {
IAnimatedMeshSceneNode* Node = 0;
};
std::vector<ChainDecorations> chainsdecorations;

void Scene::CreateChainsDecorations(){

    /// REMOVE from...
    for(unsigned int i=0; i<chainsdecorations.size(); i++){
    if(chainsdecorations[i].Node){ chainsdecorations[i].Node->remove(); }; /// Remove from: Irrlicht world
    };

    /// Reset:
    chainsdecorations.clear();

    if(!platforms.empty() && !chains.empty()){

    if(true){
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("chain_ropes_01.X"));
    node->setParent(platforms[0].Node); /// Attach to parent: PLATFORM !!!
    node->setPosition(vector3df(0,0,0));
    node->setRotation(vector3df(0,0,0));
    node->setScale(vector3df(1.0,1.0,1.0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_SOLID);

    /// Object:
    ChainDecorations cd;
    cd.Node = node;
    /// Add to object vector:
    chainsdecorations.push_back(cd);
    };

    if(true){
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("chain_motor_01.X"));
    node->setParent(chains[0].Node); /// Attach to parent: 1ST CHAIN !!!
    node->setPosition(vector3df(0,55,0));
    node->setRotation(vector3df(0,0,6));
    node->setScale(vector3df(1.0,1.0,1.0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_SOLID);
    };

    if(true){
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("priest_drunk_01.X"));
    node->setParent(platforms[0].Node); /// Attach to parent: PLATFORM !!!
    node->setName("priest_drunk");
    node->setPosition(vector3df(-165,66,125));
    node->setRotation(vector3df(0,0,0));
    node->setScale(vector3df(1.0,1.0,1.0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_SOLID);

    /// Settings and textures to materials:
    for(unsigned int m=0; m<node->getMaterialCount(); m++){
	node->setMaterialTexture(0, driver->getTexture("priest_drunk_01.jpg"));
	};

    /// Get personage text name and attach it:
    if(NAMES_TEXTS==true){
	personageName[4]->setParent(node); personageName[4]->setPosition(vector3df(0,100,0));
    };

    /// Set animation frames limits:
    /*
    f32 fstart = node->getStartFrame();
    f32 fend = node->getEndFrame()-1;
    node->setFrameLoop(fstart,fend);
    node->setCurrentFrame(fstart);
    node->setAnimationSpeed(2000); /// node->setAnimationSpeed(0);
    node->setLoopMode(true); /// node->setLoopMode(false);
    */

    /// Create particles - BEER BUBBLES:
    scene::IParticleSystemSceneNode* ps;
    scene::IParticleEmitter* em;
    /// Create particle system: (CYLINDER)
    ps = smgr->addParticleSystemSceneNode(true);
    em = ps->createCylinderEmitter(
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &center
    5,    // radius
    vector3df(0.0f, 0.0f, 0.0f), // core::vector3df &direction
    10,   // length
    true, // outline only
    vector3df(0.0f, -0.0001f, 0.0f), // core::vector3df &normal
    40,   // min
    90,   // max
    SColor(255,255,255,255), // video::SColor &minStartColor
    SColor(255,255,255,255), // video::SColor &maxStartColor
    300,  // lifeTimeMin
    900,  // lifeTimeMax
    0,    // Angle
    core::dimension2df(20.0f, 20.0f), // core::dimension2df &minStartSize
    core::dimension2df(40.0f, 40.0f)  // core::dimension2df &maxStartSize
    );

   	ps->setEmitter(em); // Grabs the emitter
	em->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(SColor(0,150,150,150), 150); // Fade out particles
	ps->addAffector(paf); // Same goes for the affector
	paf->drop();

	ps->setParent(node); // Attach to the node
	ps->setPosition(vector3df(50,20,-35));
	ps->setRotation(core::vector3df(0,0,0));
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("smoke.png"));
	ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	ps->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
	ps->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    };

    };

};



/// ------------------------------------------------------------------------------------------------------


/// Objects struct for: CARGO
struct Cargo {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID = 0;
};
std::vector<Cargo> cargos;


void Scene::RemoveCargos(){
    /// 1st REMOVE:
    for(unsigned int i=0; i<cargos.size(); i++){
    if(cargos[i].Body && cargos[i].Body->isInWorld()){
    world->removeCollisionObject(cargos[i].Body);
    }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };
    /// 2nd REMOVE:
    // while(world->getPointer()->getCollisionObjectArray()[0]){
    // obj1->getAttributes()->existsAttribute("ball");
    // world->removeCollisionObject(world->getCollisionObjectByIndex(0));
    // };
    /// Reset:
    cargos.clear();
};


void Scene::CreateCargos(){

    /// REMOVE:
    RemoveCargos();

    /// Temporary:
    std::vector<vector3df> positions;

    if(!platforms.empty()){
    f32 posX = platforms[0].Node->getPosition().X-110; /// Get platform X position, to place the barrels.
    positions.push_back(vector3df(posX,485,0));
    posX += 125;
    positions.push_back(vector3df(posX,485,0));
    // posX += 125;
    // positions.push_back(vector3df(posX,480,0));

    for(unsigned int i=0; i<positions.size(); i++){

    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("barrel_01.X"));
    node->setParent(nodeLevel); /// Attach to parent: Level
    node->setPosition(vector3df(positions[i]));
    // f32 rotY = ; /// Random rotation on Y
    node->setRotation(vector3df(0,0,0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_SOLID);
    // node->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER

	u32 mass = 20;
	/// Prefer Primitives: http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Shapes
	ICollisionShape* shape = new ICylinderShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	shape->calculateLocalInertia(mass, vector3df(0.0,0.0,0.0));
	IRigidBody* body = world->addRigidBody(shape, COL_CARGOS, cargosCollidesWith);
	body->setFriction(0.15f);
	body->setDamping(0.05f, 0.05f); // linear, angle
	body->setRestitution(0.85f); // bouncing factor
    body->setContactProcessingThreshold(btScalar(0.0)); /// For MUCH better collision detection
	// body->setCcdValues(0.01f, 0.2f); // CCD = Continuous Collision Detection (void tunneling: motion, radius) - Only needed on fast objects!
	// body->setName("box");
	body->getAttributes()->addBool("cargo", true);
    // body->getPointer()->setUserIndex(55);
	body->setLinearFactor(vector3df(1,1,1));  // Allowed positions axis
    body->setAngularFactor(vector3df(1,1,1)); // Allowed rotations axis
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    // body->setActivationState(EActivationState::EAS_ACTIVE); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States

    /// Object:
    Cargo c;
    c.Node      = node;
    c.Body      = body;
    c.BodyID    = body->getUniqueID();
    /// Add to object vector:
    cargos.push_back(c);
    };
    };
};





/// ------------------------------------------------------------------------------------------------------


/// STRUCT for SOLDIERS:
struct Soldier {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID = 0;
int Energy = 0;
int Score  = 0;
int Bonus  = 0;
bool MoveRight = true;
int MoveToStairs = 0;
f32 Rotation = 0.0;
vector3df StartPos = vector3df(0.0,0.0,0.0);
f32 PosX    = 0.0;
f32 PosY    = 0.0;
f32 MinPosX = 0.0;
f32 MaxPosX = 0.0;
f32 RotY    = 0.0;
f32 VelX    = 0.0;
f32 VelY    = 0.0;
};
/// GLOBAL:
std::vector<Soldier> soldiers;

void Scene::CreateSoldiers(){

    /// REMOVE from...
    for(unsigned int i=0; i<soldiers.size(); i++){
    if(soldiers[i].Body && soldiers[i].Body->isInWorld()){ world->removeCollisionObject(soldiers[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    soldiers.clear();

    /// Temporary vars:
    std::vector<vector3df> StartPos;
    std::vector<f32> PosX;
    std::vector<f32> PosY;
    std::vector<f32> MinPosX;
    std::vector<f32> MaxPosX;
    std::vector<f32> VelX;
    std::vector<f32> VelY;

    /// 1) Positions for soldiers in platforms:
    StartPos.push_back(vector3df(2000,930,0));
	MinPosX.push_back(2000); MaxPosX.push_back(2000+350); VelX.push_back(11); /// Max 18, so it wont take long to descend if stuck in upper platform

	StartPos.push_back(vector3df(1050,695,0));
	MinPosX.push_back(1050); MaxPosX.push_back(1050+350); VelX.push_back(10); /// Max 18, so it wont take long to descend if stuck in upper platform

	StartPos.push_back(vector3df(2610,695,0));
	MinPosX.push_back(2610); MaxPosX.push_back(2610+250); VelX.push_back(9); /// Max 18, so it wont take long to descend if stuck in upper platform

    /// 2) Positions for soldiers in the ground:
	StartPos.push_back(vector3df(1250,235,0));
	MinPosX.push_back(1250); MaxPosX.push_back(2050); VelX.push_back(15); /// Max 18, so it wont take long to descend if stuck in upper platform

	StartPos.push_back(vector3df(2050,235,0));
	MinPosX.push_back(1250); MaxPosX.push_back(2050); VelX.push_back(15); /// Max 18, so it wont take long to descend if stuck in upper platform

	StartPos.push_back(vector3df(2450,235,0));
	MinPosX.push_back(2450); MaxPosX.push_back(2800); VelX.push_back(13); /// Max 18, so it wont take long to descend if stuck in upper platform

	StartPos.push_back(vector3df(5000,235,0));
	MinPosX.push_back(5000); MaxPosX.push_back(5350); VelX.push_back(9); /// Max 18, so it wont take long to descend if stuck in upper platform

	StartPos.push_back(vector3df(2500,550,900));
    MinPosX.push_back(2500); MaxPosX.push_back(4000); VelX.push_back(10); /// far on Z

	StartPos.push_back(vector3df(1900,930,900));
    MinPosX.push_back(1050); MaxPosX.push_back(1900); VelX.push_back(17); /// far on Z

	StartPos.push_back(vector3df(3510,235,0));
    MinPosX.push_back(3510); MaxPosX.push_back(3510); VelX.push_back(0); /// Max 18, so it wont take long to descend if stuck in upper platform

	StartPos.push_back(vector3df(5630,235,0));
    MinPosX.push_back(5630); MaxPosX.push_back(5630); VelX.push_back(0); /// Max 18, so it wont take long to descend if stuck in upper platform


    for(unsigned int i=0; i<StartPos.size(); i++){
	scene::IAnimatedMeshSceneNode* node;
	if(i== 0){ node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("soldier_axe_01.X")); };
	if(i== 1){ node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("soldier_axe_01.X")); };
	if(i== 2){ node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("soldier_sword_01.X")); };
	if(i== 3){ node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("soldier_sword_01.X")); };
	if(i== 4){ node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("soldier_axe_01.X")); };
	if(i== 5){ node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("soldier_sword_01.X")); };
	if(i== 6){ node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("soldier_sword_01.X")); };
	if(i== 7){ node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("soldier_sword_01.X")); };
	if(i== 8){ node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("soldier_axe_01.X")); };
	if(i== 9){ node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("soldier_sword_01.X")); };
	if(i==10){ node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("soldier_sword_01.X")); };
	node->setParent(nodeLevel); /// Attach to parent: Level
	node->setName("soldier");
	node->setPosition(vector3df(StartPos[i]));
	node->setRotation(vector3df(0,0,0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_SOLID);
    // nodeSoldier[i]->setMaterialType(video::EMT_REFLECTION_2_LAYER);    // EMT_REFLECTION_2_LAYER

    /// Settings and textures to materials:
    for(unsigned int m=0; m<node->getMaterialCount(); m++){
	node->getMaterial(m).setTexture(0, driver->getTexture("soldier_01A.jpg"));
	};

	/// Get personage text name and attach it:
	if(NAMES_TEXTS==true){
	if(i== 0){ personageName[12]->setParent(node); personageName[12]->setPosition(vector3df(0,100,0)); };
	if(i== 1){ personageName[13]->setParent(node); personageName[13]->setPosition(vector3df(0,100,0)); };
	if(i== 2){ personageName[14]->setParent(node); personageName[14]->setPosition(vector3df(0,100,0)); };
	if(i== 3){ personageName[15]->setParent(node); personageName[15]->setPosition(vector3df(0,100,0)); };
	if(i== 4){ personageName[16]->setParent(node); personageName[16]->setPosition(vector3df(0,100,0)); };
	if(i== 5){ personageName[17]->setParent(node); personageName[17]->setPosition(vector3df(0,100,0)); };
	if(i== 6){ personageName[18]->setParent(node); personageName[18]->setPosition(vector3df(0,100,0)); };
	if(i== 7){ personageName[19]->setParent(node); personageName[19]->setPosition(vector3df(0,100,0)); };
	if(i== 8){ personageName[26]->setParent(node); personageName[26]->setPosition(vector3df(0,100,0)); };
	if(i== 9){ personageName[27]->setParent(node); personageName[27]->setPosition(vector3df(0,100,0)); };
	if(i==10){ personageName[28]->setParent(node); personageName[28]->setPosition(vector3df(0,100,0)); };
	};

    /// Set animation frames limits:
    f32 fstart = node->getStartFrame();
    f32 fend = node->getEndFrame()-1;
    node->setFrameLoop(fstart, fend);
    node->setCurrentFrame(fstart);
    node->setAnimationSpeed(0); /// node->setAnimationSpeed(0);
    node->setLoopMode(false);   /// node->setLoopMode(false);


    // IGImpactMeshShape* wShape1 = new IGImpactMeshShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); /// IConvexHullShape* wShape1 = new IConvexHullShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0); /// IBoxShape* wShape1 = new IBoxShape(rodaNode1, 10.0, true);
	/// scene::IMesh* raycastSoldier = smgr->getMesh("soldier.obj");
	/// shape = new IBvhTriangleMeshShape(node, raycastSoldier, 80);
	/// shape = new IBoxShape(node, 80, false);
	/// ICollisionShape* shape = new IGImpactMeshShape(node, device->getSceneManager()->getMesh("soldier.obj"), false);
	u32 mass = 80;
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	/// shape->getPointer()->getAabb(); /// Get shape dimensions
	shape->calculateLocalInertia(mass, vector3df(0,0,0));
	// m_pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.2f, 0.6f, 0.1f, 1.0f).normalized(), bt(m_aabb.GetCenter())));
	// btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, NULL, shape, inertia);
	IRigidBody* body = world->addRigidBody(shape, COL_SOLDIERS, soldiersCollidesWith);
	body->setFriction(0.0f);
	body->setDamping(0.0f, 0.0f);
	body->setRestitution(0.85f); /// Its the bouncing factor (to affect other bodies too)
    body->setContactProcessingThreshold(btScalar(0.0)); /// For MUCH better collision detection
	// body->getPointer()->setFlags(NO_CONTACT_RESPONSE);
	/// body->getPointer()->setCcdMotionThreshold(1.1f);   // CCD = Continuous Collision Detection - Only needed on fast objects!
	/// body->getPointer()->setCcdSweptSphereRadius(0.8f); // CCD = Continuous Collision Detection - Only needed on fast objects!
	// body->setName("soldier");
	body->getAttributes()->addBool("soldier", true);
	// body->getPointer()->setUserPointer(node);
	// body->getPointer()->setIslandTag(0);
	// body->getAttributes()->addString("soldier", "soldier");
	// body->getAttributes()->addBool("collide", true);
	// body->getAttributes()->addUserPointer("soldier", 0);
	// body->getPointer()->setUserIndex(999);
	/// body->setLinearVelocity(vector3df(0,10,0));
    body->setLinearFactor(vector3df(0,0,0));  // Allowed positions axis
    body->setAngularFactor(vector3df(0,0,0)); // Allowed rotations axis
    // body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->setCollisionFlags(ECollisionFlag::ECF_STATIC_OBJECT); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States

    /*
    btTransform axis;
    axis.setIdentity(); /// Temporary:
    std::vector<vector3df> positions;
    std::vector<int> bonus;

    /// NODES: Boxes (boxes height and width = 68)
    /// Boxes Bonus: 0 = Nothing, 1 = Extra ball, 3 = Score

    positions.push_back(vector3df(2100,748,0)); bonus.push_back(2);
    axis.setOrigin(btVector3(0,50,0));
    axis.setRotation(btQuaternion(0,0,0)); /// Bullet order is: Y,X,Z!
    btRigidBody* bodyBT = body->getPointer(); /// body->getPointer() == Needed to get the physics from irrBullet to btBullet
    btSliderConstraint* constraint = new btSliderConstraint(*bodyBT, axis, true);

    constraint->setLowerLinLimit(-2);
    constraint->setUpperLinLimit(2);
    // constraint->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0, 0.1,0)));    // linearUpperLimit
    // constraint->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,-0.1,0)));    // linearLowerLimit
    // constraint->setAngularUpperLimit(irrlichtToBulletVector(vector3df(0,0,0)));      // angularUpperLimit
    // constraint->setAngularLowerLimit(irrlichtToBulletVector(vector3df(0,0,0)));      // angularLowerLimit

    world->getPointer()->addConstraint(constraint, true);
    // world->addConstraint(constraint, true);
    */

    /// Create object:
    Soldier s;
    s.Node     = node;
    s.Body     = body;
    s.BodyID   = body->getUniqueID();
    s.Energy   = 0;
    s.Score    = 0;
    s.Bonus    = 0;
    s.MoveRight = true;
    s.MoveToStairs = 0;
    s.Rotation = 0.0;
    s.StartPos = vector3df(StartPos[i]);
    s.PosX     = 0.0;
    s.PosY     = 0.0;
    s.MinPosX  = MinPosX[i];
    s.MaxPosX  = MaxPosX[i];
    s.RotY     = 0.0;
    s.VelX     = VelX[i];
    s.VelY     = 0.0;
    /// Add to object vector:
    soldiers.push_back(s);
    };
};




/// ------------------------------------------------------------------------------------------------------



/// STRUCT for PRIESTS:
struct Priest {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID = 0;
f32 Speed = 0.0;
};
/// GLOBAL:
std::vector<Priest> priests;

void Scene::CreatePriests(){

    /// REMOVE from...
    for(unsigned int i=0; i<priests.size(); i++){
    if(priests[i].Body && priests[i].Body->isInWorld()){ world->removeCollisionObject(priests[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    priests.clear();

    /// Temporary:
    std::vector<vector3df> positions;
    std::vector<f32> speed;

    positions.push_back(vector3df(5920,740,200)); speed.push_back(0.80);
    positions.push_back(vector3df(6075,740,200)); speed.push_back(0.90);
    positions.push_back(vector3df(5920,740,750)); speed.push_back(0.40);
    positions.push_back(vector3df(6075,740,750)); speed.push_back(0.60);


    /// NODES: Priests
    for(unsigned int i=0; i<positions.size(); i++){
	scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("priest_01.X"));
	node->setParent(nodeLevel); /// Attach to parent: Level
	node->setName("priest");
	node->setPosition(positions[i]);
	node->setRotation(vector3df(0,0,0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_SOLID);
    // nodeSoldier[i]->setMaterialType(video::EMT_REFLECTION_2_LAYER);    // EMT_REFLECTION_2_LAYER

    /// Settings and textures to materials:
    for(unsigned int m=0; m<node->getMaterialCount(); m++){
	node->getMaterial(m).setTexture(0, driver->getTexture("priest_01A.jpg"));
	};

	/// Get personage text name and attach it:
	if(NAMES_TEXTS==true){
	if(i==0){ personageName[8]->setParent(node);  personageName[8]->setPosition(vector3df(0,80,0));  };
	if(i==1){ personageName[9]->setParent(node);  personageName[9]->setPosition(vector3df(0,80,0));  };
	if(i==2){ personageName[10]->setParent(node); personageName[10]->setPosition(vector3df(0,80,0)); };
	if(i==3){ personageName[11]->setParent(node); personageName[11]->setPosition(vector3df(0,80,0)); };
	};




	// IGImpactMeshShape* wShape1 = new IGImpactMeshShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); /// IConvexHullShape* wShape1 = new IConvexHullShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0); /// IBoxShape* wShape1 = new IBoxShape(rodaNode1, 10.0, true);
	/// scene::IMesh* raycastSoldier = smgr->getMesh("soldier.obj");
	/// shape = new IBvhTriangleMeshShape(node, raycastSoldier, 80);
	/// shape = new IBoxShape(node, 80, false);
	/// ICollisionShape* shape = new IGImpactMeshShape(node, device->getSceneManager()->getMesh("soldier.obj"), false);
	u32 mass = 60;
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	/// shape->getPointer()->getAabb(); /// Get shape dimensions
	shape->calculateLocalInertia(mass, vector3df(0,0,0));
	IRigidBody* body = world->addRigidBody(shape, COL_PRIESTS, priestsCollidesWith);
	body->setFriction(0.1f);
	body->setDamping(0.0f, 0.0f);
	body->setRestitution(0.85f); /// Its the bouncing factor (to affect other bodies too)
    body->setContactProcessingThreshold(btScalar(0.0)); /// For MUCH better collision detection
	body->getAttributes()->addBool("priest", true);
	/// body->setLinearVelocity(vector3df(0,10,0));
	body->setLinearFactor(vector3df(0,1,0));  // Allowed positions axis
    body->setAngularFactor(vector3df(0,0,0)); // Allowed rotations axis
    // body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States

    /// Create object:
    Priest p;
    p.Node     = node;
    p.Body     = body;
    p.BodyID   = body->getUniqueID();
    p.Speed    = speed[i];
    /// Add to object vector:
    priests.push_back(p);
    };

};




/// ------------------------------------------------------------------------------------------------------


/// STRUCT for PRIESTS WALKING:
struct PriestWalking {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID = 0;
bool MoveRight = true;
vector3df StartPos = vector3df(0,0,0);
f32 MinPosX  = 0.0;
f32 MaxPosX  = 0.0;
f32 VelX     = 0.0;
f32 VelY     = 0.0;
f32 PosY     = 0.0;
f32 Rotation = 0.0;
};
/// GLOBAL:
std::vector<PriestWalking> priestswalking;

void Scene::CreatePriestsWalking(){

    /// REMOVE from...
    for(unsigned int i=0; i<priestswalking.size(); i++){
    if(priestswalking[i].Body && priestswalking[i].Body->isInWorld()){ world->removeCollisionObject(priestswalking[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    priestswalking.clear();

    /// Temporary:
    std::vector<vector3df> StartPos;
    std::vector<f32> MinPosX;
    std::vector<f32> MaxPosX;
    std::vector<f32> VelX;

    StartPos.push_back(vector3df(4250,235,650));
    MinPosX.push_back(4250); MaxPosX.push_back(5670); VelX.push_back(7); /// Priest walking in church

    StartPos.push_back(vector3df(5650,235,550));
    MinPosX.push_back(4250); MaxPosX.push_back(5670); VelX.push_back(8); /// Priest walking in church

    StartPos.push_back(vector3df(5050,740,550));
    MinPosX.push_back(5050); MaxPosX.push_back(5670); VelX.push_back(6); /// Priest walking on top of the church



    /// NODES: Prisoners
    for(unsigned int i=0; i<StartPos.size(); i++){
	scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("priest_walking_01.X"));
	node->setParent(nodeLevel); /// Attach to parent: Level
	node->setName("priestswalking");
	node->setPosition(StartPos[i]);
	node->setRotation(vector3df(0,0,0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_SOLID);
    // nodeSoldier[i]->setMaterialType(video::EMT_REFLECTION_2_LAYER); // EMT_REFLECTION_2_LAYER

    /// Settings and textures to materials:
    for(unsigned int m=0; m<node->getMaterialCount(); m++){
	if(i==0){ node->getMaterial(m).setTexture(0, driver->getTexture("priest_walking_01A.jpg")); };
	if(i==1){ node->getMaterial(m).setTexture(0, driver->getTexture("priest_walking_01B.jpg")); };
	if(i==2){ node->getMaterial(m).setTexture(0, driver->getTexture("priest_walking_01B.jpg")); };
	};

	/// Get personage text name and attach it:
	if(NAMES_TEXTS==true){
	if(i==0){ personageName[6]->setParent(node); personageName[6]->setPosition(vector3df(0,60,0)); };
	if(i==1){ personageName[7]->setParent(node); personageName[7]->setPosition(vector3df(0,60,0)); };
	if(i==2){ personageName[25]->setParent(node); personageName[25]->setPosition(vector3df(0,60,0)); };
	};


	u32 mass = 80;
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	/// shape->getPointer()->getAabb(); /// Get shape dimensions
	shape->calculateLocalInertia(mass, vector3df(0,0,0));
	IRigidBody* body = world->addRigidBody(shape, COL_PRIESTS_WALKING, priestsWalkingCollidesWith);
	body->setFriction(0.0f);
	body->setDamping(0.0f, 0.0f);
	body->setRestitution(0.85f); /// Its the bouncing factor (to affect other bodies too)
    body->setContactProcessingThreshold(btScalar(0.0)); /// For MUCH better collision detection
	body->getAttributes()->addBool("priestswalking", true);
	/// body->setLinearVelocity(vector3df(0,10,0));
	body->setLinearFactor(vector3df(0,1,0));  // Allowed positions axis
    body->setAngularFactor(vector3df(0,0,0)); // Allowed rotations axis
    // body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States

    /// Create object:
    PriestWalking pw;
    pw.Node      = node;
    pw.Body      = body;
    pw.BodyID    = body->getUniqueID();
    pw.MoveRight = true;
    pw.StartPos  = StartPos[i];
    pw.MinPosX   = MinPosX[i];
    pw.MaxPosX   = MaxPosX[i];
    pw.VelX      = VelX[i];
    pw.VelY      = 0.0;
    pw.PosY      = 0.0;
    pw.Rotation  = 0.0;

    /// Add to object vector:
    priestswalking.push_back(pw);
    };

};



/// ------------------------------------------------------------------------------------------------------


/// Object for: Worker
struct Worker {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID = 0;
f32 Speed = 0.0;
};
/// Vars for: Worker
std::vector<Worker> workers;

void Scene::CreateWorkers(){

    /// REMOVE from...
    for(unsigned int i=0; i<workers.size(); i++){
    if(workers[i].Body && workers[i].Body->isInWorld()){ world->removeCollisionObject(workers[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    workers.clear();

    /// Temporary:
    std::vector<vector3df> positions;
    std::vector<f32> speed;

    /// Add workers:
    positions.push_back(vector3df(5845,1170,0));
    speed.push_back(20.0);


    for(unsigned int i=0; i<positions.size(); i++){
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("worker_01.X"));
    node->setParent(nodeLevel); /// Attach to parent: Level
    node->setPosition(vector3df(positions[i]));
    node->setRotation(vector3df(0,80,0));
    node->setScale(vector3df(1.0,1.0,1.0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    // node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    // node->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER

	/// Get personage text name and attach it:
	if(NAMES_TEXTS==true){
	if(i==0){ personageName[5]->setParent(node); personageName[5]->setPosition(vector3df(0,80,0)); };
	};


    // IGImpactMeshShape* wShape1 = new IGImpactMeshShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); /// IConvexHullShape* wShape1 = new IConvexHullShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0); /// IBoxShape* wShape1 = new IBoxShape(rodaNode1, 10.0, true);
	/// scene::IMesh* raycastSoldier = smgr->getMesh("soldier.obj");
	/// shape = new IBvhTriangleMeshShape(node, raycastSoldier, 80);
	/// shape = new IBoxShape(node, 80, false);
	/// ICollisionShape* shape = new IGImpactMeshShape(node, device->getSceneManager()->getMesh("soldier.obj"), false);
	u32 mass = 80;
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	/// shape->getPointer()->getAabb(); /// Get shape dimensions
	shape->calculateLocalInertia(mass, vector3df(0,0,0));
	// m_pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.2f, 0.6f, 0.1f, 1.0f).normalized(), bt(m_aabb.GetCenter())));
	// btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, NULL, shape, inertia);
	IRigidBody* body = world->addRigidBody(shape, COL_WORKERS, workersCollidesWith);
	body->setFriction(5.0f);
	body->setDamping(0.05f, 0.05f);
	body->setRestitution(0.75f); /// Its the bouncing factor (to affect other bodies too)
    body->setContactProcessingThreshold(btScalar(0.0)); /// For MUCH better collision detection
	// body->getPointer()->setFlags(NO_CONTACT_RESPONSE);
	/// body->getPointer()->setCcdMotionThreshold(1.1f);   // CCD = Continuous Collision Detection - Only needed on fast objects!
	/// body->getPointer()->setCcdSweptSphereRadius(0.8f); // CCD = Continuous Collision Detection - Only needed on fast objects!
	// body->setName("soldier");
	body->getAttributes()->addBool("worker", true);
	/// body->setLinearVelocity(vector3df(0,10,0));
    body->setLinearFactor(vector3df(0,1,0));  // Allowed positions axis
    body->setAngularFactor(vector3df(0,0,0)); // Allowed rotations axis
    // body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->setCollisionFlags(ECollisionFlag::ECF_STATIC_OBJECT); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States

    /// Create object:
    Worker w;
    w.Node   = node;
    w.Body   = body;
    w.BodyID = body->getUniqueID();
    w.Speed  = speed[i];
    /// Add to global vector:
    workers.push_back(w);
    };
};




/// ------------------------------------------------------------------------------------------------------


/// Object for: Knight
struct Knight {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID   = 0;
f32 Rotation = 0.0;
vector3df StartPos = vector3df(0.0,0.0,0.0);
bool MoveRight = true;
f32 MinPosX  = 0.0;
f32 MaxPosX  = 0.0;
f32 PosY     = 0.0;
f32 VelX     = 0.0;
f32 VelY     = 0.0;
};
/// Vars for: Knight
std::vector<Knight> knights;

void Scene::CreateKnights(){

    /// REMOVE from...
    for(unsigned int i=0; i<knights.size(); i++){
    if(knights[i].Body && knights[i].Body->isInWorld()){ world->removeCollisionObject(knights[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    knights.clear();

    /// Temporary:
    std::vector<vector3df> StartPos;
    std::vector<f32> MinPosX;
    std::vector<f32> MaxPosX;
    std::vector<f32> VelX;

    /// Add knights:
    StartPos.push_back(vector3df(4300,235,630));
    MinPosX.push_back(1700); MaxPosX.push_back(4100); VelX.push_back(12);

    StartPos.push_back(vector3df(1700,235,600));
    MinPosX.push_back(1700); MaxPosX.push_back(4100); VelX.push_back(11);


    for(unsigned int i=0; i<StartPos.size(); i++){
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("knight_01.X"));
    node->setParent(nodeLevel); /// Attach to parent: Level
    node->setPosition(vector3df(StartPos[i]));
    node->setRotation(vector3df(0,0,0));
    node->setScale(vector3df(1.0,1.0,1.0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    // node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    // node->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER

    /// Apply texture to my specified material
    for(unsigned int m=0; m<node->getMaterialCount(); m++){
	if(i==0){ node->getMaterial(m).setTexture(0, driver->getTexture("knight_01.jpg")); };
	if(i==1){ node->getMaterial(m).setTexture(0, driver->getTexture("knight_02.jpg")); };
	};

    /// Get personage text name and attach it:
    if(NAMES_TEXTS==true){
	if(i==0){ personageName[3]->setParent(node); personageName[3]->setPosition(vector3df(0,100,0)); };
	if(i==1){ personageName[2]->setParent(node); personageName[2]->setPosition(vector3df(0,100,0)); };
    };

    // IGImpactMeshShape* wShape1 = new IGImpactMeshShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); /// IConvexHullShape* wShape1 = new IConvexHullShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0); /// IBoxShape* wShape1 = new IBoxShape(rodaNode1, 10.0, true);
	/// scene::IMesh* raycastSoldier = smgr->getMesh("soldier.obj");
	/// shape = new IBvhTriangleMeshShape(node, raycastSoldier, 80);
	/// shape = new IBoxShape(node, 80, false);
	/// ICollisionShape* shape = new IGImpactMeshShape(node, device->getSceneManager()->getMesh("soldier.obj"), false);
	u32 mass = 80;
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	/// shape->getPointer()->getAabb(); /// Get shape dimensions
	shape->calculateLocalInertia(mass, vector3df(0,0,0));
	// m_pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.2f, 0.6f, 0.1f, 1.0f).normalized(), bt(m_aabb.GetCenter())));
	// btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, NULL, shape, inertia);
	IRigidBody* body = world->addRigidBody(shape, COL_KNIGHTS, knightsCollidesWith);
	body->setFriction(5.0f);
	body->setDamping(0.05f, 0.05f);
	body->setRestitution(0.75f); /// Its the bouncing factor (to affect other bodies too)
    body->setContactProcessingThreshold(btScalar(0.0)); /// For MUCH better collision detection
	// body->getPointer()->setFlags(NO_CONTACT_RESPONSE);
	/// body->getPointer()->setCcdMotionThreshold(1.1f);   // CCD = Continuous Collision Detection - Only needed on fast objects!
	/// body->getPointer()->setCcdSweptSphereRadius(0.8f); // CCD = Continuous Collision Detection - Only needed on fast objects!
	// body->setName("soldier");
	body->getAttributes()->addBool("knight", true);
	/// body->setLinearVelocity(vector3df(0,10,0));
    body->setLinearFactor(vector3df(0,1,0));  // Allowed positions axis
    body->setAngularFactor(vector3df(0,0,0)); // Allowed rotations axis
    // body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->setCollisionFlags(ECollisionFlag::ECF_STATIC_OBJECT); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States

    /// Create object:
    Knight k;
    k.Node   = node;
    k.Body   = body;
    k.BodyID = body->getUniqueID();
    k.Rotation = 0.0;
    k.StartPos = StartPos[i];
    k.MoveRight = true;
    k.MinPosX  = MinPosX[i];
    k.MaxPosX  = MaxPosX[i];
    k.PosY     = 0.0;
    k.VelX     = VelX[i];
    k.VelY     = 0.0;

    /// Add to global vector:
    knights.push_back(k);
    };
};






/// ------------------------------------------------------------------------------------------------------


/// STRUCT for KING:
struct King {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID = 0;
bool MoveRight = true;
vector3df StartPos = vector3df(0,0,0);
f32 MinPosX  = 0.0;
f32 MaxPosX  = 0.0;
f32 VelX     = 0.0;
f32 VelY     = 0.0;
f32 PosY     = 0.0;
f32 Rotation = 0.0;
};
/// GLOBAL:
std::vector<King> kings;

void Scene::CreateKings(){

    /// REMOVE from...
    for(unsigned int i=0; i<kings.size(); i++){
    if(kings[i].Body && kings[i].Body->isInWorld()){ world->removeCollisionObject(kings[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    kings.clear();

    /// Temporary:
    std::vector<vector3df> StartPos;
    std::vector<f32> MinPosX;
    std::vector<f32> MaxPosX;
    std::vector<f32> VelX;

    StartPos.push_back(vector3df(4600,1000,850));
    MinPosX.push_back(4600); MaxPosX.push_back(5670); VelX.push_back(2);


    /// NODES:
    for(unsigned int i=0; i<StartPos.size(); i++){
	scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("king_01.X"));
	node->setParent(nodeLevel); /// Attach to parent: Level
	node->setName("king");
	node->setPosition(StartPos[i]);
	node->setRotation(vector3df(0,0,0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_SOLID);
    // nodeSoldier[i]->setMaterialType(video::EMT_REFLECTION_2_LAYER); // EMT_REFLECTION_2_LAYER

	/// Settings and textures to materials:
    for(unsigned int m=0; m<node->getMaterialCount(); m++){
    if(i==0){ node->getMaterial(m).setTexture(0, driver->getTexture("king_01.jpg")); };
	};

    /// Get personage text name and attach it:
    if(NAMES_TEXTS==true){
	personageName[1]->setParent(node); personageName[1]->setPosition(vector3df(0,115,0));
    };


	u32 mass = 80;
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	/// shape->getPointer()->getAabb(); /// Get shape dimensions
	shape->calculateLocalInertia(mass, vector3df(0,0,0));
	IRigidBody* body = world->addRigidBody(shape, COL_KINGS, kingsCollidesWith);
	body->setFriction(0.0f);
	body->setDamping(0.0f, 0.0f);
	body->setRestitution(0.85f); /// Its the bouncing factor (to affect other bodies too)
    body->setContactProcessingThreshold(btScalar(0.0)); /// For MUCH better collision detection
	body->getAttributes()->addBool("king", true);
	/// body->setLinearVelocity(vector3df(0,10,0));
	body->setLinearFactor(vector3df(0,1,0));  // Allowed positions axis
    body->setAngularFactor(vector3df(0,0,0)); // Allowed rotations axis
    // body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States

    /// Create object:
    King k;
    k.Node      = node;
    k.Body      = body;
    k.BodyID    = body->getUniqueID();
    k.MoveRight = true;
    k.StartPos  = StartPos[i];
    k.MinPosX   = MinPosX[i];
    k.MaxPosX   = MaxPosX[i];
    k.VelX      = VelX[i];
    k.VelY      = 0.0;
    k.PosY      = 0.0;
    k.Rotation  = 0.0;

    /// Add to object vector:
    kings.push_back(k);
    };

};


/// ------------------------------------------------------------------------------------------------------


/// STRUCT for MUSICIANS:
struct Musician {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID = 0;
f32 Speed = 0.0;
};
/// GLOBAL:
std::vector<Musician> musicians;

void Scene::CreateMusicians(){

    /// REMOVE from...
    for(unsigned int i=0; i<musicians.size(); i++){
    if(musicians[i].Body && musicians[i].Body->isInWorld()){ world->removeCollisionObject(musicians[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    musicians.clear();

    /// Temporary:
    std::vector<vector3df> StartPos;
    StartPos.push_back(vector3df(3870,1000,850));

    /// NODES:
    for(unsigned int i=0; i<StartPos.size(); i++){
	scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("musicians_01.X"));
	node->setParent(nodeLevel); /// Attach to parent: Level
	node->setName("musician");
	node->setPosition(StartPos[i]);
	node->setRotation(vector3df(0,0,0));
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_SOLID);
    // nodeSoldier[i]->setMaterialType(video::EMT_REFLECTION_2_LAYER); // EMT_REFLECTION_2_LAYER

	/// Settings and textures to materials:
    for(unsigned int m=0; m<node->getMaterialCount(); m++){
    if(i==0){ node->getMaterial(m).setTexture(0, driver->getTexture("musicians_01A.jpg")); };
	};

    /// Get personage text name and attach it:
    if(NAMES_TEXTS==true){
	personageName[24]->setParent(node); personageName[24]->setPosition(vector3df(0,100,0));
    };

	u32 mass = 160;
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	/// shape->getPointer()->getAabb(); /// Get shape dimensions
	shape->calculateLocalInertia(mass, vector3df(0,0,0));
	IRigidBody* body = world->addRigidBody(shape, COL_MUSICIANS, musiciansCollidesWith);
	body->setFriction(0.0f);
	body->setDamping(0.0f, 0.0f);
	body->setRestitution(0.85f); /// Its the bouncing factor (to affect other bodies too)
    body->setContactProcessingThreshold(btScalar(0.0)); /// For MUCH better collision detection
	body->getAttributes()->addBool("musician", true);
	/// body->setLinearVelocity(vector3df(0,10,0));
	body->setLinearFactor(vector3df(0,1,0));  // Allowed positions axis
    body->setAngularFactor(vector3df(0,0,0)); // Allowed rotations axis
    // body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States

    /// Create object:
    Musician mu;
    mu.Node      = node;
    mu.Body      = body;
    mu.BodyID    = body->getUniqueID();
    mu.Speed     = 8.0;

    /// Add to object vector:
    musicians.push_back(mu);
    };

};



/// ------------------------------------------------------------------------------------------------------


/// STRUCT for BULL:
struct Bull {
IAnimatedMeshSceneNode* Node;
IRigidBody* Body;
int BodyID = 0;
f32 Speed = 0.0;
};
/// GLOBAL:
std::vector<Bull> bulls;

void Scene::CreateBulls(){

    /// REMOVE from...
    for(unsigned int i=0; i<bulls.size(); i++){
    if(bulls[i].Body && bulls[i].Body->isInWorld()){ world->removeCollisionObject(bulls[i].Body); }; /// Removes from: Bullet && Irrlicht (Its using irrBullet Wrapper)
    };

    /// Reset:
    bulls.clear();

    /// Temporary:
    std::vector<vector3df> StartPos;
    StartPos.push_back(vector3df(4150,235,405));

    /// NODES:
    for(unsigned int i=0; i<StartPos.size(); i++){
	scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("bull_01.X"));
	node->setParent(nodeLevel); /// Attach to parent: Level
	node->setName("bull");
	node->setPosition(StartPos[i]);
	node->setRotation(vector3df(0,15,0)); /// Rotated it just a little bit
	node->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    node->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides !!! Affects water reflex !!!
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    node->setMaterialType(video::EMT_SOLID);
    // nodeSoldier[i]->setMaterialType(video::EMT_REFLECTION_2_LAYER); // EMT_REFLECTION_2_LAYER


	u32 mass = 250;
	ICollisionShape* shape = new IBoxShape(node, mass, true);
	shape->setLocalScaling(vector3df(1.0,1.0,1.0), EScalingPair::ESP_COLLISIONSHAPE); // ESP_BOTH, ESP_COLLISIONSHAPE
	shape->setMargin(1.0f);
	/// shape->getPointer()->getAabb(); /// Get shape dimensions
	shape->calculateLocalInertia(mass, vector3df(0,0,0));
	IRigidBody* body = world->addRigidBody(shape, COL_BULLS, bullsCollidesWith);
	body->setFriction(0.5f);
	body->setDamping(0.0f, 0.0f);
	body->setRestitution(0.85f); /// Its the bouncing factor (to affect other bodies too)
    body->setContactProcessingThreshold(btScalar(0.0)); /// For MUCH better collision detection
	body->getAttributes()->addBool("bull", true);
	/// body->setLinearVelocity(vector3df(0,10,0));
	body->setLinearFactor(vector3df(0,1,0));  // Allowed positions axis
    body->setAngularFactor(vector3df(0,0,0)); // Allowed rotations axis
    // body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE, EAS_DISABLE_DEACTIVATION, EAS_WANTS_DEACTIVATION
    body->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    // body->activate(true); /// IMPORTANT --> http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States

    /// Create object:
    Bull bu;
    bu.Node      = node;
    bu.Body      = body;
    bu.BodyID    = body->getUniqueID();
    bu.Speed     = 1.2;

    /// Add to object vector:
    bulls.push_back(bu);
    };

};






void Scene::StartMaterials(){

    /// Go through all the scene nodes:
    /// core::list<scene::ISceneNode*> children = smgr->getRootSceneNode()->getChildren();
    core::list<scene::ISceneNode*> children = nodeLevel->getChildren(); /// Get the main node
    core::list<scene::ISceneNode*>::Iterator it = children.begin();
	for(; it!=children.end(); it++){
    /// Custom settings and textures to materials:
    for(unsigned int m=0; m<(*it)->getMaterialCount(); m++){
	(*it)->getMaterial(m).AmbientColor  = video::SColor(0, 0, 0, 0);
    (*it)->getMaterial(m).SpecularColor = video::SColor(0, 0, 0, 0);
    (*it)->getMaterial(m).DiffuseColor  = video::SColor(0, 0, 0, 0);
    (*it)->getMaterial(m).EmissiveColor = video::SColor(0, 0, 0, 0);
    (*it)->getMaterial(m).ColorMaterial = video::ECM_DIFFUSE; /// Default is: video::ECM_DIFFUSE
    (*it)->getMaterial(m).Shininess = 0.f;
	};
    };
};


void Scene::FadeMaterials(){

    /// FADEIN Materials:
    if(fadein_materials==true){
    if(fadeInMaterials>=255){ fadeInMaterials = 255; fadein_materials = false; };
    /// Go through all the scene nodes:
    /// core::list<scene::ISceneNode*> children = smgr->getRootSceneNode()->getChildren();
    core::list<scene::ISceneNode*> children = nodeLevel->getChildren(); /// Get the main node
    core::list<scene::ISceneNode*>::Iterator it = children.begin();
	for(; it!=children.end(); it++){
    /// Custom settings and textures to materials:
    for(unsigned int m=0; m<(*it)->getMaterialCount(); m++){
    /// (*it)->getMaterial(m).setTexture(0, driver->getTexture("empty.jpg"));
	(*it)->getMaterial(m).AmbientColor  = video::SColor(0, fadeInMaterials, fadeInMaterials, fadeInMaterials);
    (*it)->getMaterial(m).SpecularColor = video::SColor(0, 0, 0, 0);
    (*it)->getMaterial(m).DiffuseColor  = video::SColor(0, 0, 0, 0);
    (*it)->getMaterial(m).EmissiveColor = video::SColor(0, 0, 0, 0);
    (*it)->getMaterial(m).ColorMaterial = video::ECM_DIFFUSE; /// Default is: video::ECM_DIFFUSE
    (*it)->getMaterial(m).Shininess = 1.f;
    /// Specific shinny nodes:
    if((*it)->getName()=="jail" || (*it)->getName()=="bell" || (*it)->getName()=="cannon"){
    (*it)->getMaterial(m).SpecularColor = video::SColor(0, 150, 150, 150);
    (*it)->getMaterial(m).Shininess = 10.f;
    };
	};
    };
    fadeInMaterials+=0.20*frameDeltaTime;
    };


    /// FADEOUT Materials:
    if(fadeout_materials==true){
    if(fadeOutMaterials<=0){ fadeOutMaterials = 0; fadeout_materials = false; };
    /// Go through all the scene nodes:
    /// core::list<scene::ISceneNode*> children = smgr->getRootSceneNode()->getChildren();
    core::list<scene::ISceneNode*> children = nodeLevel->getChildren(); /// Get the main node
    core::list<scene::ISceneNode*>::Iterator it = children.begin();
	for(; it!=children.end(); it++){
    /// Custom settings and textures to materials:
    for(unsigned int m=0; m<(*it)->getMaterialCount(); m++){
	(*it)->getMaterial(m).AmbientColor  = video::SColor(0, fadeOutMaterials, fadeOutMaterials, fadeOutMaterials);
    (*it)->getMaterial(m).SpecularColor = video::SColor(0, 0, 0, 0);
    (*it)->getMaterial(m).DiffuseColor  = video::SColor(0, 0, 0, 0);
    (*it)->getMaterial(m).EmissiveColor = video::SColor(0, 0, 0, 0);
    (*it)->getMaterial(m).ColorMaterial = video::ECM_DIFFUSE; /// Default is: video::ECM_DIFFUSE
    (*it)->getMaterial(m).Shininess = 0.f;
	};
    };
    fadeOutMaterials-=0.55*frameDeltaTime;
    };

};


void Scene::Loop(){
/// ...
};

