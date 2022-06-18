#include "Elevator.hpp"

Elevator *Elevator::m_elevator = NULL;
Elevator Elevator::m_default_elevator_buffer;

Elevator *Elevator::Instance()
{
  if(!m_elevator)
    m_elevator = &m_default_elevator_buffer;

  return m_elevator;
}

void Elevator::SetDefaultValues(void)
{
  eleup=true;
  elevatorExists = false;
  ridingElevator = false;
}

void Elevator::Add(ISceneNode *node)
{
  elevatorNode.push_back(node);

  tempPos.Y = node->getPosition().Y;
  tempPos.X = tempPos.Y;
  elevatorNodePos.push_back(tempPos);

  elevatorExists = true;
}

void Elevator::Clear(void)
{
  for(u32 i = 0; i < elevatorNode.size(); i++)
  {
    elevatorNode[i]->remove();
    elevatorNode.erase(i);
  }
  elevatorNode.clear();
  elevatorNodePos.clear();
}

void Elevator::CheckCollisionWithPlayer(void)
{
  //! Check if player is on the elevatorNode.
  if(elevatorExists)
  {
    for(u32 i = 0; i < elevatorNode.size(); i++)
    {
      elevatorBox = elevatorNode[i]->getTransformedBoundingBox();
      elevatorBoxExpanded = Collision::Instance()->expandBox(elevatorBox, 1000.0f);

      playerPos = Player::Instance()->getPlayerNode()->getPosition();
   //   playerPos.Y -= 25.0f;

      if(elevatorBoxExpanded.isPointInside(playerPos))
        ridingElevator = true;
      else
        ridingElevator = false;
    }
  }
}

void Elevator::UpdatePlayerPosition(void)
{
  if(elevatorExists){
    if(ridingElevator){
      for(u32 i = 0; i < elevatorNode.size(); i++){

        if( elevatorPos <= 0 && !eleup)
        {eleup=1;}
        else if(elevatorPos == 200 && eleup)
        {eleup=0;}

        else if(!eleup){
            elevatorPos--;
            elevatorNode[i]->setPosition(vector3df( elevatorNode[i]->getPosition().X,
                                                    elevatorNode[i]->getPosition().Y-1,
                                                    elevatorNode[i]->getPosition().Z));

        }else{
            elevatorPos++;
            elevatorNode[i]->setPosition(vector3df( elevatorNode[i]->getPosition().X,
                                                    elevatorNode[i]->getPosition().Y+1,
                                                    elevatorNode[i]->getPosition().Z));

            if (ridingElevator && Player::Instance()->isFalling){
             }else{
              playerNewPos = Player::Instance()->getPlayerNode()->getAbsolutePosition();
              playerNewPos.Y = elevatorNode[i]->getAbsolutePosition().Y;
              Player::Instance()->getPlayerNode()->setPosition(playerNewPos);
             }
        }
      }}

      else{ // update elevator anyway even if not ridingElevator
                for(u32 i = 0; i < elevatorNode.size(); i++){
            if( elevatorPos <= 0 && !eleup)
        {eleup=1;}
        else if(elevatorPos == 200 && eleup)
        {eleup=0;}

        else if(!eleup){
            elevatorPos--;
            elevatorNode[i]->setPosition(vector3df( elevatorNode[i]->getPosition().X,
                                                    elevatorNode[i]->getPosition().Y-1,
                                                    elevatorNode[i]->getPosition().Z));
            //elevatorNode[i]->updateAbsolutePosition();
        }else{
            elevatorPos++;
            elevatorNode[i]->setPosition(vector3df( elevatorNode[i]->getPosition().X,
                                                    elevatorNode[i]->getPosition().Y+1,
                                                    elevatorNode[i]->getPosition().Z));
        }
      }
    }
  }
}
