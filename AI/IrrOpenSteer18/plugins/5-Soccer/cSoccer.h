//#ifndef C_Soccer_H
//#define C_Soccer_H
//
//#include <irrlicht.h>
//#include "../../cOpenSteerDemo.h"
//#include "../../cMeshTool.h"
//
//using namespace OpenSteer;
//
//// a box object for the field and the goals.
//class AABBox{
//  public:
//    AABBox(Vec3 &min, Vec3& max): m_min(min), m_max(max){}
//    AABBox(Vec3 min, Vec3 max): m_min(min), m_max(max){}
//    bool	InsideX(const Vec3 p){if(p.x < m_min.x || p.x > m_max.x)	return false;return true;}
//    bool	InsideZ(const Vec3 p){if(p.z < m_min.z || p.z > m_max.z)	return false;return true;}
//    void	draw(){
//        Vec3 b,c;
//        b = Vec3(m_min.x, 0, m_max.z);
//        c = Vec3(m_max.x, 0, m_min.z);
//        Color color(1.0f,1.0f,0.0f);
//// TODO (Acki#1#): convert to Irrlicht
////            drawLineAlpha(m_min, b, color, 1.0f);
////            drawLineAlpha(b, m_max, color, 1.0f);
////            drawLineAlpha(m_max, c, color, 1.0f);
////            drawLineAlpha(c,m_min, color, 1.0f);
//    }
//  private:
//    Vec3 m_min;
//    Vec3 m_max;
//};
//
//class Ball : public SimpleVehicle, public irr::scene::ISceneNode{
//  private:
//    datMesh Mesh;
//    datMesh Trail;
//    irr::core::aabbox3df Box;
//    irr::video::SMaterial Material;
//    AABBox *m_bbox;
//
//  public:
//    // constructor
//    Ball(AABBox* bbox, irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent);
//    // reset state
//    void reset();
//    // per frame simulation update
//    void update(const float currentTime, const float elapsedTime);
//    void kick(Vec3 dir, const float elapsedTime);
//    // draw this character/vehicle into the scene
//    void draw();
//
//    void recordTrailVertex(const float currentTime, const Vec3& pos);
//    void clearTrailHistory();
//
//    //! SceneNode stuff
//    virtual void OnRegisterSceneNode();
//    virtual void render();
//    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
//    virtual irr::u32 getMaterialCount();
//    virtual irr::video::SMaterial& getMaterial(irr::u32 i);
//    virtual void setMaterial(irr::video::SMaterial newMaterial);
//
//};
//
//class Player : public SimpleVehicle, public irr::scene::ISceneNode{
//  private:
//    datMesh Trail;
//    irr::core::aabbox3df Box;
//    irr::video::SMaterial Material;
//
//    const std::vector<Player*>	m_others;
//    const std::vector<Player*>	m_AllPlayers;
//    Ball*	m_Ball;
//    bool	b_ImTeamA;
//    int		m_MyID;
//    Vec3	m_home;
//
//  public:
//    datMesh Mesh;
//    // constructor
//    Player(std::vector<Player*> others,
//           std::vector<Player*> allplayers,
//           Ball* ball,
//           bool isTeamA,
//           int id,
//           irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent
//          );
//    // reset state
//    void reset();
//    // per frame simulation update
//    void update(const float currentTime, const float elapsedTime);
//    // draw this character/vehicle into the scene
//    void draw();
//
//    void recordTrailVertex(const float currentTime, const Vec3& pos);
//    void clearTrailHistory();
//
//    //! SceneNode stuff
//    virtual void OnRegisterSceneNode();
//    virtual void render();
//    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
//    virtual irr::u32 getMaterialCount();
//    virtual irr::video::SMaterial& getMaterial(irr::u32 i);
//    virtual void setMaterial(irr::video::SMaterial newMaterial);
//
//};
//
//#endif // C_Soccer_H
