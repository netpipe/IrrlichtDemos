/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
#include "cSoccerPlugIn.h"

//! extern Irrlicht stuff
extern datIrrPointers irrPointers;
extern irr::gui::IGUIStaticText* Text2;

//! !!! Create an instance of the plugin !!!
MicTestPlugIn PI05_Soccer;

//! Create Scene
void MicTestPlugIn::open(){
  irr::scene::ISceneManager* SMgr = 0;
  irr::scene::ISceneNode* SRoot = 0;
  if(irrPointers.SMgr){
    SMgr = irrPointers.SMgr;
    SRoot = irrPointers.SMgr->getRootSceneNode();
#ifdef useIrrExtensions13
    //! Create "ground plane"
    if(nGround) irrPointers.SMgr->addToDeletionQueue(nGround);
    nGround = irrPointers.SMgr->addGridSceneNode(1,2);
    nGround->setPosition(irr::core::vector3df(0,0,0));
    nGround->setScale(irr::core::vector3df(20,0,10));
    nGround->setAccentlineColor(nGround->getGridColor());
    //! Create "nGoal1"
    if(nGoal1) irrPointers.SMgr->addToDeletionQueue(nGoal2);
    nGoal1 = irrPointers.SMgr->addGridSceneNode(1,2);
    nGoal1->setPosition(irr::core::vector3df(-20,0,0));
    nGoal1->setScale(irr::core::vector3df(1,0,7));
    nGoal1->setAccentlineColor(nGoal1->getGridColor());
    //! Create "nGoal2"
    if(nGoal2) irrPointers.SMgr->addToDeletionQueue(nGoal2);
    nGoal2 = irrPointers.SMgr->addGridSceneNode(1,2);
    nGoal2->setPosition(irr::core::vector3df(20,0,0));
    nGoal2->setScale(irr::core::vector3df(1,0,7));
    nGoal2->setAccentlineColor(nGoal2->getGridColor());
#endif
  }
  // Make a field
  m_bbox = new AABBox(Vec3(-20,0,-10), Vec3(20,0,10));
  // Red goal
  m_TeamAGoal = new AABBox(Vec3(-21,0,-7), Vec3(-19,0,7));
  // Blue Goal
  m_TeamBGoal = new AABBox(Vec3(19,0,-7), Vec3(21,0,7));
  // Make a ball
  m_Ball = new Ball(m_bbox, SMgr, SRoot);
  // Build team A
  m_PlayerCountA = 8;
  for(unsigned int i=0; i < m_PlayerCountA ; i++){
    Player* pMicTest = new Player(TeamA, m_AllPlayers, m_Ball, true, i, SMgr, SRoot);
    pMicTest->Mesh.setColor(irr::video::SColor(255, 0,200,0));
    TeamA.push_back(pMicTest);
    m_AllPlayers.push_back(pMicTest);
  }
  // Build Team B
  m_PlayerCountB = 8;
  for(unsigned int i=0; i < m_PlayerCountB ; i++){
    Player *pMicTest = new Player(TeamB, m_AllPlayers, m_Ball, false, i, SMgr, SRoot);
    pMicTest->Mesh.setColor(irr::video::SColor(255, 0,0,200));
    TeamB.push_back(pMicTest);
    m_AllPlayers.push_back(pMicTest);
  }
  OpenSteerDemo::selectedVehicle = m_Ball;
  m_redScore = 0;
  m_blueScore = 0;
  setInfoText(Text2);
}
//! Reset Scene
void MicTestPlugIn::reset(){
  //! reset vehicle
  for(unsigned int i=0; i < m_PlayerCountA ; i++)
    TeamA[i]->reset();
  for(unsigned int i=0; i < m_PlayerCountB ; i++)
    TeamB[i]->reset();
  m_Ball->reset();
}
//! Clear Scene
void MicTestPlugIn::close(){
#ifdef useIrrExtensions13
  if(nGround) irrPointers.SMgr->addToDeletionQueue(nGround);
  nGround = NULL;
  if(nGoal1) irrPointers.SMgr->addToDeletionQueue(nGoal1);
  nGoal1 = NULL;
  if(nGoal2) irrPointers.SMgr->addToDeletionQueue(nGoal2);
  nGoal2 = NULL;
#endif
  for(unsigned int i=0; i < m_PlayerCountA ; i++)
    irrPointers.SMgr->addToDeletionQueue(TeamA[i]);
  TeamA.clear();
  for(unsigned int i=0; i < m_PlayerCountB ; i++)
    irrPointers.SMgr->addToDeletionQueue(TeamB[i]);
  TeamB.clear ();
  irrPointers.SMgr->addToDeletionQueue(m_Ball);
  m_AllPlayers.clear();
}
//! Update Simulation
void MicTestPlugIn::update(const float currentTime, const float elapsedTime){
  // update simulation of test vehicle
  for(unsigned int i=0; i < m_PlayerCountA ; i++)
    TeamA[i]->update(currentTime, elapsedTime);
  for(unsigned int i=0; i < m_PlayerCountB ; i++)
    TeamB[i]->update(currentTime, elapsedTime);
  m_Ball->update(currentTime, elapsedTime);
  if(m_TeamAGoal->InsideX(m_Ball->position()) && m_TeamAGoal->InsideZ(m_Ball->position())){
    m_Ball->reset();	// Ball in blue teams goal, red scores
    m_redScore++;
  }
  if(m_TeamBGoal->InsideX(m_Ball->position()) && m_TeamBGoal->InsideZ(m_Ball->position())){
    m_Ball->reset();	// Ball in red teams goal, blue scores
    m_blueScore++;
  }
}
//! Redraw Scene (Irrlicht does all the stuff)
void MicTestPlugIn::redraw(const float currentTime, const float elapsedTime){
}

void MicTestPlugIn::setInfoText(irr::gui::IGUIStaticText* st){
  irr::core::stringw txt;

  st->setText(txt.c_str());
}
