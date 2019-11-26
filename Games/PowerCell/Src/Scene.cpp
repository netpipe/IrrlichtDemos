#include "Scene.hpp"

Scene *Scene::m_scene = NULL;
Scene Scene::m_default_scene_buffer;

void Scene::SetDefaultValues()
{
  gameOver = false;
  STATE_CURRENT = STATE_MENU_MAIN;
}

Scene *Scene::Instance()
{
  if(!m_scene)
    m_scene = &m_default_scene_buffer;

  return m_scene;
}

void Scene::EndGame(void)
{
  irrDevice->getCursorControl()->setVisible(true);

  //Sound::PlaySound("sound18.mp3");

  if(Player::Instance()->getIsDead())
  {
    messageCleared = irrDevice->getSceneManager()->addTextSceneNode(
      irrDevice->getGUIEnvironment()->getFont("fonts/cellfont1.png"), L"Out of Power!", SColor(200,255,255,255), Player::Instance()->getPlayerNode());
  }
  else
  {
    messageCleared = irrDevice->getSceneManager()->addTextSceneNode(
      irrDevice->getGUIEnvironment()->getFont("fonts/cellfont1.png"), L"Stage Cleared!", SColor(200,255,255,255), Player::Instance()->getPlayerNode());
  }

  ISceneNodeAnimator *anim = irrDevice->getSceneManager()->createDeleteAnimator(3000);
  messageCleared->addAnimator(anim);
  anim->drop();


  endCamera = irrDevice->getSceneManager()->addCameraSceneNodeFPS();
  endCamera->setInputReceiverEnabled(false);
  endCamera->setPosition(vector3df(
    Player::Instance()->getPlayerNode()->getAbsolutePosition().X,
    Player::Instance()->getPlayerNode()->getAbsolutePosition().Y,
    Player::Instance()->getPlayerNode()->getAbsolutePosition().Z
  ));
  endCamera->setTarget(Player::Instance()->getPlayerNode()->getPosition());

  anim = irrDevice->getSceneManager()->createFlyCircleAnimator(vector3df(
    //Player::Instance()->getPlayerNode()->getPosition().X+100,
    //Player::Instance()->getPlayerNode()->getPosition().Y+100,
    //Player::Instance()->getPlayerNode()->getPosition().Z+50
    Player::Instance()->getPlayerNode()->getAbsolutePosition().X,
    Player::Instance()->getPlayerNode()->getAbsolutePosition().Y,
    Player::Instance()->getPlayerNode()->getAbsolutePosition().Z
  ), 100, 0.001f);
  endCamera->addAnimator(anim);
  anim->drop();

  if(Player::Instance()->getIsDead())
  {
  //  Player::Instance()->getPlayerNode()->setMD2Animation("deathb");
  //  Player::Instance()->getPlayerNode()->setFrameLoop(690,708);
  }
  else
  {
   // Player::Instance()->getPlayerNode()->setMD2Animation("salute");
  //  Player::Instance()->getPlayerNode()->setFrameLoop(300,400);
  }

  Player::Instance()->getPlayerNode()->setLoopMode(false);

  irrDevice->getSceneManager()->setActiveCamera(endCamera);
}

void Scene::UpdateEndCamera(vector3df pos)
{
  endCamera->setTarget(pos);
}

bool Scene::GameOver(void)
{
  return gameOver;
}

void Scene::GameOver(bool val)
{
  gameOver = val;
}

void Scene::SetState(int state)
{
  STATE_CURRENT = state;
}

int Scene::GetState(void)
{
  return STATE_CURRENT;
}

void Scene::LoadStage(stringc Stage)
{
  irrDevice->getCursorControl()->setVisible(false);
  Output::Instance()->w("Loading character model.\n");
//"PlayerModel/nskinrd.jpg";
  stringc t ="PlayerModel/Speedtroid-1-4-4-unwrap-1-3_Cube.png";
  stringc m = "PlayerModel/speedtroid249.blend.x";

  IAnimatedMesh* mesh = irrDevice->getSceneManager()->getMesh(m.c_str());
  Player::Instance()->setPlayerNode(irrDevice->getSceneManager()->addAnimatedMeshSceneNode(mesh));


  if (Player::Instance()->getPlayerNode())
  {
    Player::Instance()->getPlayerNode()->setAnimationSpeed(40);
    Player::Instance()->getPlayerNode()->setScale(vector3df(5.f,5.f,5.f));
      Player::Instance()->getPlayerNode()->setFrameLoop(1,50);
 //   Player::Instance()->getPlayerNode()->setMD2Animation(EMAT_STAND);
    Player::Instance()->getPlayerNode()->setMaterialTexture( 0, irrDevice->getVideoDriver()->getTexture(t.c_str()) );
    Player::Instance()->getPlayerNode()->setMaterialFlag(EMF_LIGHTING, false);
  }

  /*
  Add skydome
  */
  Output::Instance()->w("Loading sky dome.\n");
  irrDevice->getSceneManager()->addSkyDomeSceneNode(irrDevice->getVideoDriver()->getTexture(
    "Skybox/sky_aboveclouds.jpg"),
    30, 30, 1, 2);


//		Set fog values. Placed here because stages should have unique settings.

	irrDevice->getVideoDriver()->setFog(
		SColor(0,56,61,48),
		EFT_FOG_LINEAR,
		250.0f,
		1500.0f,
		0.01f,
		false,false);

  /*
  Add meshes
  */
  Output::Instance()->w("Loading stage.\n");
    core::stringc myStr( Stage );myStr += L".irr";
  irrDevice->getFileSystem()->changeWorkingDirectoryTo(Stage.c_str());
  irrDevice->getSceneManager()->loadScene(myStr.c_str());
  irrDevice->getFileSystem()->changeWorkingDirectoryTo("..");

  /*
  To look at the mesh, we place a camera into 3d space at the position
  (0, 30, -40). The camera looks from there to (0,5,0).
  */
  Output::Instance()->w("Creating camera.\n");
  Camera::Instance()->createCamera();

  //! ----------------------------------------------------
  //! Add collision
  //! ----------------------------------------------------
  Output::Instance()->w("Creating root collision.\n");
  Collision::Instance()->createRootCollision();

  Output::Instance()->w("Creating player collision.\n");
  Collision::Instance()->createPlayerCollision();

  //! ----------------------------------------------------
  //! Create blob shadow for player. Created after recurse-meta, since it adds collision to all meshes.
  //! ----------------------------------------------------
  Output::Instance()->w("Creating player shadow.\n");
  Player::Instance()->createShadow();

  //! ----------------------------------------------------
  //! Add power cells.
  //! ----------------------------------------------------
  Output::Instance()->w("Adding power cells.\n");

  ISceneNode *node = 0;
  u32 i = 0;

  while((node = irrDevice->getSceneManager()->getSceneNodeFromName("powercell", 0)))
  {
    if(node)
    {
      PowerCell::Instance()->Add(node->getPosition());
      node->remove();
      i++;
    }
  }

  if(PowerCell::Instance()->GetPowerCellCount() <= 0)
  {
    PowerCell::Instance()->Add(vector3df(0,0,0));
    i++;
  }

  PlayerHelper::Instance()->SetPowerCellInfo(0,i);

  GameGUI::Instance()->Init();

  GameGUI::Instance()->updatePowerCellInfo(
    PlayerHelper::Instance()->GetPowerCellsAquired(),
    PlayerHelper::Instance()->GetPowerCellsTotal());

  //! ----------------------------------------------------
  //! Add health cells.
  //! ----------------------------------------------------
  Output::Instance()->w("Adding health cells.\n");

  node = 0;
  i = 0;
  while((node = irrDevice->getSceneManager()->getSceneNodeFromName("healthcell", 0)))
  {
    if(node)
    {
      HealthCell::Instance()->Add(node->getPosition());
      node->remove();
      i++;
    }
  }

  if(HealthCell::Instance()->GetTotalHealthCells() <= 0)
  {
    HealthCell::Instance()->Add(vector3df(0,0,0));
  }

  GameGUI::Instance()->updateHealthInfo(PlayerHelper::Instance()->GetHealth());

  //! ----------------------------------------------------
  //! Player start node.
  //! ----------------------------------------------------
  Output::Instance()->w("Setting player start.\n");

  while((node = irrDevice->getSceneManager()->getSceneNodeFromName("playerstart", 0)))
  {
    if(node)
    {
     // startPoS=  node->getAbsolutePosition();
      Player::Instance()->getPlayerNode()->setPosition(node->getPosition());
      Player::Instance()->getPlayerNode()->setRotation(node->getRotation());
  //    Player::Instance()->getPlayerNode()->updateAbsolutePosition();

      Output::Instance()->w("\
        Player: %f %f %f\n\
        Start: %f %f %f\n",
        Player::Instance()->getPlayerNode()->getPosition().X,
        Player::Instance()->getPlayerNode()->getPosition().Y,
        Player::Instance()->getPlayerNode()->getPosition().Z,
        node->getPosition().X,
        node->getPosition().Y,
        node->getPosition().Z);

      node->remove();
    }
  }
}
