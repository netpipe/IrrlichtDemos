///*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com)*/
//#ifndef C_MicTestPlugIn_H
//#define C_MicTestPlugIn_H
//
//#include <irrlicht.h>
//#include "../../cOpenSteerDemo.h"
//#include "cSoccer.h"
//
//using namespace OpenSteer;
//
//// PlugIn for OpenSteerDemo
//class MicTestPlugIn : public PlugIn{
//  private:
//    //! GridSceneNode for the ground
//#ifdef useIrrExtensions13
//    irr::scene::IGridSceneNode* nGround;
//    irr::scene::IGridSceneNode* nGoal1;
//    irr::scene::IGridSceneNode* nGoal2;
//#endif
//    const AVGroup& allVehicles () {return (const AVGroup&) TeamA;}
//    unsigned int	m_PlayerCountA;
//    unsigned int	m_PlayerCountB;
//    std::vector<Player*> TeamA;
//    std::vector<Player*> TeamB;
//    std::vector<Player*> m_AllPlayers;
//    Ball	  *m_Ball;
//    AABBox	*m_bbox;
//    AABBox	*m_TeamAGoal;
//    AABBox	*m_TeamBGoal;
////    int junk;
//    int		m_redScore;
//    int		m_blueScore;
//
//  public:
//    const char* name(){           return "5 - Simple Soccer";}
//    float selectionOrderSortKey(){ return 5.0; } /*!< the selection order for "Next PlugIn" */
//
//    void setInfoText(irr::gui::IGUIStaticText* st);
//
//    //! Create Scene
//    void open();
//    //! Reset Scene
//    void reset();
//    //! Clear Scene
//    void close();
//    //! Update Simulation
//    void update(const float currentTime, const float elapsedTime);
//    //! Redraw Scene (Irrlicht does all the stuff)
//    void redraw(const float currentTime, const float elapsedTime);
//
//    virtual ~MicTestPlugIn(){}
//};
//
//#endif // C_MicTestPlugIn_H
