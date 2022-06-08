float GetHeight(const vector3df& pos, float maxHeight) 
{ 
   scene::ISceneCollisionManager* colMan = device->getSceneManager()->getSceneCollisionManager(); 

    vector3df yAxis(0,maxHeight,0); 
    line3d<f32> ray(pos, pos-yAxis); 
    

    vector3df ptoCol(0,0,0); 
    triangle3df triangle; 

    if(colMan->getCollisionPoint(ray, terrainTSelector, ptoCol, triangle)) { 
      //infoColision->setText(strInfo.c_str()); 
        return ptoCol.Y; 
    } 
    else 
    { 
        return -maxHeight; 
    } 

} 
