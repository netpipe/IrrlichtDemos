///*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
//#include "cSoccer.h"
//
////! extern Irrlicht stuff
//extern datIrrPointers irrPointers;
//
//Vec3 playerPosition[9] = {
//  Vec3(4,0,0),
//  Vec3(7,0,-5),
//  Vec3(7,0,5),
//  Vec3(10,0,-3),
//  Vec3(10,0,3),
//  Vec3(15,0, -8),
//  Vec3(15,0,0),
//  Vec3(15,0,8),
//  Vec3(4,0,0)
//};
//
////! The Ball
//// constructor
//Ball::Ball(AABBox* bbox, irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent)
//:ISceneNode(parent,smgr), m_bbox(bbox){
//  reset();
//  //! disable lighting
//  Material.Lighting = false;
//}
//// reset state
//void Ball::reset(){
//  SimpleVehicle::reset(); // reset the vehicle
//  speed(0.0f);         // speed along Forward direction.
//  maxForce(9.0f);      // steering force is clipped to this magnitude
//  maxSpeed(9.0f);         // velocity is clipped to this magnitude
//  position(0,0,0);
////  setTrailParameters(100, 6000);
//  clearTrailHistory();    // prevent long streaks due to teleportation
//  // load the mesh
//  Mesh = getMesh_VehicleDisk();
//  Box = Mesh.getBoundingbox();
//  setScale(irr::core::vector3df(0.1, 0.1, 0.1));
//  Mesh.setColor(irr::video::SColor(255, 255,255,255));
//}
//// per frame simulation update
//void Ball::update(const float currentTime, const float elapsedTime){
//  applyBrakingForce(1.5f, elapsedTime);
//  applySteeringForce(velocity(), elapsedTime);
//  // are we now outside the field?
//  if(!m_bbox->InsideX(position())){
//    Vec3 d = velocity();
//    regenerateOrthonormalBasis(Vec3(-d.x, d.y, d.z));
//    applySteeringForce(velocity(), elapsedTime);
//  }
//  if(!m_bbox->InsideZ(position())){
//    Vec3 d = velocity();
//    regenerateOrthonormalBasis(Vec3(d.x, d.y, -d.z));
//    applySteeringForce(velocity(), elapsedTime);
//  }
//  // update Irrlicht node
//  setPosition(position().vector3df());
//  irr::scene::ISceneNode::setRotation(forward().vector3df().getHorizontalAngle());
//  // update motion trail
////  recordTrailVertex(currentTime, position());
//}
//void Ball::kick(Vec3 dir, const float elapsedTime){
//  OPENSTEER_UNUSED_PARAMETER(elapsedTime);
//  speed(dir.length());
//  regenerateOrthonormalBasis(dir);
//}
//// draw this character/vehicle into the scene
//void Ball::draw(){
//  // Irrlicht does all the work ;)
//}
//void Ball::clearTrailHistory(){
//  // we use our own trail mesh
//  Trail.faceIndices.clear();
//  Trail.faceVertices.clear();
//  Trail.lineIndices.clear();
//  Trail.lineVertices.clear();
//  // create initial vertex
//  irr::video::S3DVertex vert;
//  vert.Pos = position().vector3df();
//  vert.Color = irr::video::SColor(255, 255,0,0);
//  Trail.lineVertices.push_back(vert);
//
//}
//void Ball::recordTrailVertex(const float currentTime, const Vec3& pos){
//  // we use our own trail mesh
//  Trail.lineIndices.push_back(Trail.lineVertices.size() - 1);
//  Trail.lineIndices.push_back(Trail.lineVertices.size());
//  irr::video::S3DVertex vert;
//  vert.Pos = irr::core::vector3df(pos.x,pos.y,pos.z);
//  vert.Color = irr::video::SColor(255, 255,0,0);
//  Trail.lineVertices.push_back(vert);
//}
//// SceneNode
//void Ball::OnRegisterSceneNode(){
//  if(IsVisible) irrPointers.SMgr->registerNodeForRendering(this);
//  ISceneNode::OnRegisterSceneNode();
//}
//void Ball::render(){
//  irrPointers.Driver->setMaterial(Material);
//  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::scene::ISceneNode::AbsoluteTransformation);
//  Mesh.draw(irrPointers.Driver);
//  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
//  Trail.draw(irrPointers.Driver);
//}
//const irr::core::aabbox3d<irr::f32>& Ball::getBoundingBox() const{
//  return Box;
//}
//irr::u32 Ball::getMaterialCount(){
//  return 1;
//}
//irr::video::SMaterial& Ball::getMaterial(irr::u32 i){
//  return Material;
//}
//void Ball::setMaterial(irr::video::SMaterial newMaterial){
//  Material = newMaterial;
//}
//
////! The Player
//// constructor
//Player::Player(std::vector<Player*> others, std::vector<Player*> allplayers, Ball* ball, bool isTeamA, int id, irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent)
//:ISceneNode(parent,smgr), m_others(others), m_AllPlayers(allplayers), m_Ball(ball), b_ImTeamA(isTeamA), m_MyID(id){
//  reset();
//  //! disable lighting
//  Material.Lighting = false;
//}
//
//// reset state
//void Player::reset(){
//  SimpleVehicle::reset(); // reset the vehicle
//  speed(0.0f);         // speed along Forward direction.
//  maxForce(3000.7f);      // steering force is clipped to this magnitude
//  maxSpeed(10);         // velocity is clipped to this magnitude
//  // Place me on my part of the field, looking at oponnents goal
//  position(b_ImTeamA ? frandom01() * 20 : -frandom01() * 20, 0, (frandom01() - 0.5f) * 20);
//  if(m_MyID < 9){
//    if(b_ImTeamA)
//      position(playerPosition[m_MyID]);
//    else
//      position(Vec3(-playerPosition[m_MyID].x, playerPosition[m_MyID].y, playerPosition[m_MyID].z));
//  }
//  m_home = position();
//  clearTrailHistory();    // prevent long streaks due to teleportation
//  // load the mesh
//  Mesh = getMesh_VehicleDisk();
//  Box = Mesh.getBoundingbox();
//  setScale(irr::core::vector3df(1.0, 0.5, 1.0));
//}
//// per frame simulation update
//void Player::update(const float currentTime, const float elapsedTime){
//  // if I hit the ball, kick it.
//  const float distToBall = Vec3::distance(position(), m_Ball->position());
//  const float sumOfRadii = radius() + m_Ball->radius();
//  if(distToBall < sumOfRadii) m_Ball->kick((m_Ball->position() - position()) * 50, elapsedTime);
//  // otherwise consider avoiding collisions with others
//  Vec3 collisionAvoidance = steerToAvoidNeighbors(1, (AVGroup&)m_AllPlayers);
//  if(collisionAvoidance != Vec3::zero)
//    applySteeringForce (collisionAvoidance, elapsedTime);
//  else{
//    float distHomeToBall = Vec3::distance (m_home, m_Ball->position());
//    if(distHomeToBall < 12.0f){
//      // go for ball if I'm on the 'right' side of the ball
//      if(b_ImTeamA ? position().x > m_Ball->position().x : position().x < m_Ball->position().x){
//        Vec3 seekTarget = steerForSeekEx(m_Ball->position());
//        applySteeringForce (seekTarget, elapsedTime);
//      }else{
//        if(distHomeToBall < 12.0f){
//          float Z = m_Ball->position().z - position().z > 0 ? -1.0f : 1.0f;
//          Vec3 behindBall = m_Ball->position() + (b_ImTeamA ? Vec3(2.0f,0.0f,Z) : Vec3(-2.0f,0.0f,Z));
//          Vec3 behindBallForce = steerForSeekEx(behindBall);
//          annotationLine(position(), behindBall , Color(0.0f,1.0f,0.0f));
//          Vec3 evadeTarget = steerForFleeEx(m_Ball->position());
//          applySteeringForce(behindBallForce*10.0f + evadeTarget, elapsedTime);
//        }
//      }
//    }else{
//      // Go home
//      Vec3 seekTarget = steerForSeekEx(m_home);
//      Vec3 seekHome = steerForSeekEx(m_home);
//      applySteeringForce(seekTarget+seekHome, elapsedTime);
//    }
//  }
//  // update Irrlicht node
//  setPosition(position().vector3df());
//  irr::scene::ISceneNode::setRotation(forward().vector3df().getHorizontalAngle());
//  // update motion trail
////  recordTrailVertex(currentTime, position());
//}
//// draw this character/vehicle into the scene
//void Player::draw(){
//  // Irrlicht does all the work ;)
//}
//
//void Player::clearTrailHistory(){
//  // we use our own trail mesh
//  Trail.faceIndices.clear();
//  Trail.faceVertices.clear();
//  Trail.lineIndices.clear();
//  Trail.lineVertices.clear();
//  // create initial vertex
//  irr::video::S3DVertex vert;
//  vert.Pos = position().vector3df();
//  vert.Color = irr::video::SColor(255, 255,0,0);
//  Trail.lineVertices.push_back(vert);
//
//}
//void Player::recordTrailVertex(const float currentTime, const Vec3& pos){
//  // we use our own trail mesh
//  Trail.lineIndices.push_back(Trail.lineVertices.size() - 1);
//  Trail.lineIndices.push_back(Trail.lineVertices.size());
//  irr::video::S3DVertex vert;
//  vert.Pos = irr::core::vector3df(pos.x,pos.y,pos.z);
//  vert.Color = irr::video::SColor(255, 255,0,0);
//  Trail.lineVertices.push_back(vert);
//}
//
//// SceneNode
//void Player::OnRegisterSceneNode(){
//  if(IsVisible) irrPointers.SMgr->registerNodeForRendering(this);
//  ISceneNode::OnRegisterSceneNode();
//}
//void Player::render(){
//  irrPointers.Driver->setMaterial(Material);
//  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::scene::ISceneNode::AbsoluteTransformation);
//  Mesh.draw(irrPointers.Driver);
//  irrPointers.Driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
//  Trail.draw(irrPointers.Driver);
//}
//const irr::core::aabbox3d<irr::f32>& Player::getBoundingBox() const{
//  return Box;
//}
//irr::u32 Player::getMaterialCount(){
//  return 1;
//}
//irr::video::SMaterial& Player::getMaterial(irr::u32 i){
//  return Material;
//}
//void Player::setMaterial(irr::video::SMaterial newMaterial){
//  Material = newMaterial;
//}
//
