#ifndef _Elevator_HPP_
#define _Elevator_HPP_

#include "Globals.hpp"

class Elevator
{
  private:

    bool elevatorExists;
    bool ridingElevator;
    aabbox3d<f32> elevatorBox, elevatorBoxExpanded;
    vector2df tempPos;
    vector3df playerPos, playerNewPos;
    float elevatorPos;


    array<ISceneNode*> elevatorNode;
    array<vector2df> elevatorNodePos;

  public:
    bool eleup;
    vector3df elevatorStart;
    static Elevator *Instance();

    void SetDefaultValues(void);
    void Add(ISceneNode *node);
    void Clear(void);
    void CheckCollisionWithPlayer(void);
    void UpdatePlayerPosition(void);

  protected:

    static Elevator *m_elevator;
    static Elevator m_default_elevator_buffer;

};

#endif
