//Ground.cpp
#include "ground.h"

Ground::Ground(){
}

Ground::Ground(ISceneManager* smgr,IVideoDriver* driver, unsigned tile){
     this->tileset = tile;
     this->smgr = smgr;
     this->driver = driver;
}
void Ground::Drop(){
     for(short i = 0; i < MaxX; i++)
       for(short j = 0; j < MaxY;j++){
         lvl[i][j]->grab();
         lvl[i][j]->drop();
         }
     return;
     
}
void Ground::ZeroLevel(unsigned short lvlArray[MaxX][MaxY]){
     for(short i = 0; i < MaxX; i++)
        for(short j = 0; j < MaxY;j++)
        {
           if(lvlArray[i][j] == 1)
             lvl[i][j]->setVisible(true);
           else
             lvl[i][j]->setVisible(false); 

          lvl[i][j]->setPosition(core::vector3df(-35,(j*-100)+100,((i*25)- 62.5)  ));
        }
     return;
}
void Ground::SetShaders(bool set){
     shaders = set;
     //ToDo:
     //Add code to allow dynamic shader changing
}

void Ground::SetLevel(unsigned short lvlArray[MaxX][MaxY]){
     video::ITexture* defaulttexture;
     video::ITexture* normalMap;
     scene::IMesh* tangentMesh; 
     switch(tileset){
         case 0: //Lava
          grnd = smgr->getMesh("Media/Models/floor.MY3D");
          defaulttexture = driver->getTexture("Media/Models/rockwall.bmp");
          if(shaders){
              normalMap = driver->getTexture("Media/Models/rockwall_height.bmp");
              driver->makeNormalMapTexture(normalMap, 9.0f);
              tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(grnd->getMesh(0));    
          }
          LevelFriction = 0.003;         
         break;
         case 1: //Iceworld
          grnd = smgr->getMesh("Media/Models/floor.MY3D");
          defaulttexture = driver->getTexture("Media/Models/water05.jpg");
          if(shaders){
              normalMap = driver->getTexture("Media/Models/rockwall_height.bmp");
              driver->makeNormalMapTexture(normalMap, 9.0f);
              tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(grnd->getMesh(0));    
          }
          LevelFriction = 0.0015;  
         break;
         case 2: //Jungle
          grnd = smgr->getMesh("Media/Models/floor.MY3D");
          defaulttexture = driver->getTexture("Media/Models/jungle.png");
          if(shaders){
              normalMap = driver->getTexture("Media/Models/rockwall_height.bmp");
              driver->makeNormalMapTexture(normalMap, 9.0f);
              tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(grnd->getMesh(0));    
          }
          LevelFriction = 0.0015;  
         break;
         case 3: //Space
         break; 
     }
     
     /*
     grnd = smgr->getMesh("Media/Models/floor.MY3D");

     video::ITexture* defaulttexture = driver->getTexture("Media/Models/rockwall.bmp");
video::ITexture* normalMap;
    scene::IMesh* tangentMesh;
//   
     if(shaders){
         normalMap = driver->getTexture("Media/Models/rockwall_height.bmp");
         driver->makeNormalMapTexture(normalMap, 9.0f);
         tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(grnd->getMesh(0));    
     }
//     
     LevelFriction = 0.002;
     */
    // mapNode = smgr->addOctTreeSceneNode(grnd);
  //    selector = smgr->createOctTreeTriangleSelector(grnd->getmesh(0),0,2);
   //IAnimatedMeshSceneNode.
    //rnd->getMesh()
     //l = 
     
     for(short i = 0; i < MaxX; i++)
        for(short j = 0; j < MaxY;j++)
       {
             //lvl[i][j] = smgr->addAnimatedMeshSceneNode(grnd);
             if(shaders){
    		   lvl[i][j] = smgr->addMeshSceneNode(tangentMesh);
    		   lvl[i][j]->setMaterialTexture(0,	defaulttexture);
    		   lvl[i][j]->setMaterialTexture(1,	normalMap);
               lvl[i][j]->setMaterialType(video::EMT_PARALLAX_MAP_SOLID); 
    	       lvl[i][j]->getMaterial(0).MaterialTypeParam = 0.035f; // adjust height for parallax effect
    	       lvl[i][j]->setMaterialFlag(video::EMF_LIGHTING, false);
             }else{
               lvl[i][j] = smgr->addAnimatedMeshSceneNode(grnd);
               lvl[i][j]->setMaterialTexture(0,	defaulttexture);
               lvl[i][j]->setMaterialFlag(video::EMF_LIGHTING, true);
             }
                    //lvl[i][j]->setMaterialTexture(0, defaulttexture);
           lvl[i][j]->setPosition(core::vector3df(-35,(j*-100)+100,((i*25)- 62.5)  ));
           lvl[i][j]->setMaterialFlag(video::EMF_FOG_ENABLE, true);
                    //lvl[i][j]->setMaterialFlag(video::EMF_LIGHTING, true);
           if(lvlArray[i][j] == 0)
               lvl[i][j]->setVisible(false);
           //lvl[i][j] = smgr->addAnimatedMeshSceneNode(grnd);
           //lvl[i][j]->setPosition(core::vector3df(-5,j*-12,((i*6)- 15)  ));
        } 
       //lvl[1][1] = smgr->addAnimatedMeshSceneNode(grnd);
       //lvl[1][1]->setMaterialTexture(0, defaulttexture);
       //lvl[1][1]->setPosition(core::vector3df(-25,-50,0  ));
       
       //Y,0,X
	 if(shaders)
        tangentMesh->drop();
     grnd->drop();
     return;
}

//Rather then using encrypted files, we store this data in the .EXE
/*
void Ground::CutLevel(unsigned short lvlArray[MaxX][MaxY],start,end){
     for(short i = 0;j < maxX; i++)
        for(short j = start;start < end;i++)
          if(lvlArray[MaxX][MaxY]=1)
              lvl[i][j]->drop();
     
} */
void Ground::Flip(unsigned short index,unsigned short lvlArray[MaxX][MaxY]){

  for(short i = 0; i < MaxX; i++){
    // if( lvlArray[i][slice] == 1) index%10
           lvl[i][index%(unsigned short)MaxY]->setPosition(core::vector3df(-35,((index+(unsigned short)MaxY)*-100)+100,((i*25)- 62.5)  ));
        //   lvlArray[i][slice] = lvlArray[i][index%(unsigned short)MaxY];
}
     for(unsigned i = 0;i < (int)MaxX;i++){
           lvl[i][index%(unsigned short)MaxY]->setVisible(true);
           lvlArray[i][index%(unsigned short)MaxY] = 1;
     }
     unsigned loopCount = (int)index/(int)MaxY;
     if(loopCount > (int)MaxX)
        loopCount = MaxX; //Caps difficulty
        
     for(int i = -1; i < (int)index/(int)MaxY;i++){
          unsigned short rndum = rand()%(5);
          if (lvlArray[rndum][index%(unsigned short)MaxY] == 1){
                     lvl[rndum][index%(unsigned short)MaxY]->setVisible(false);
                     lvlArray[rndum][index%(unsigned short)MaxY] = 0;
            // lvl[rndum][slice]->grab();
         //    lvl[rndum][slice]->drop();
          //   lvlArray[rndum][slice] = 0;
          }
     }
            
}

float Ground::GetFriction(){
         return LevelFriction;
}
void Ground::LoadLevel(unsigned short lvlArray[MaxX][MaxY]){
   //  for(short i = 0; i < 5; i++)
   //     for(short j = 0; j < 60;j++){
   //       lvlArray[i][j] = 1;
   //       if( i == 3 && j > 10)
   //           lvlArray[i][j] = 0;
  //        }
    //                    lvlArray[3][12] = 1;
       //                 lvlArray[3][17] = 1;
for(short i = 0; i < (short)MaxX; i++)
  for(short j = 0; j < (short)MaxY;j++)
          lvlArray[i][j] = 1;
          
for(short j = 5; j < MaxY;j++)
    lvlArray[rand()%(5)][j] = 0;
          //Random Gen   
} 
