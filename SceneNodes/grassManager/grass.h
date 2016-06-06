//grass.cpp
//since 03.04.2010; by Adam 'entity' Krupicka

struct grassPlaneTemplate{
  IMeshSceneNode* mesh;
  int probability;
  int minHeight;
  int maxHeight;
  float YTranslation;
};

struct grass{
  private:
    ISceneManager* smgr;
    const IGeometryCreator* geom;
    
    array<grassPlaneTemplate>planeTemplate;
    
    array<ISceneNode*>grassNodes;
    array<vector3df>grassNodePos;
    array<short int>grassNodeType;
    array<bool>isGrassNodePosFree;
    
    array<aabbox3d<f32> >noGrassZone;
    float noGrassZoneFadeOut;

    ITerrainSceneNode* terrain;
    int planeSize;
    int texturesPerPlane;
    int distance;
    float density;
    
    bool lighting;
    bool aniso;
    bool fog;
    
    int helperForDeleting;
    
    int count;
    
  
  public:
    grass()
    {
      terrain= 0;
      noGrassZoneFadeOut= 0.4;
      planeSize= 5;
      texturesPerPlane= 1;
      lighting= true;
      aniso= false;
      fog= false;
      distance= 100;
      density= 1000;
      count= 0;
      helperForDeleting= 0;
      
      srand((unsigned)time(0));
    }
    
    int getCount()
    {
      return count;
    }
    
    void setSmgr(ISceneManager* newSmgr)
    {
      smgr= newSmgr;
      geom= smgr->getGeometryCreator();
      return;
    }
    
    void setParametres(int newDistance, float newDensity=3, int newPlaneSize= 5)
    {
      distance= newDistance;
      density= newDensity;
      planeSize= newPlaneSize;
      return;
    }
    
    void setTerrain(ITerrainSceneNode* newTerrain)
    {
      terrain= newTerrain;
      return;
    }
    
    void setTexturesPerPlane(int newTexturesPerPlane= 2)
    {
      texturesPerPlane= newTexturesPerPlane;
      return;
    }
    
    void setNoGrassFadeOut(float newNoGrassZoneFadeOut)
    {
      noGrassZoneFadeOut= newNoGrassZoneFadeOut;
      return;
    }
    
    void setLighting(bool newLighting)
    {
      lighting= newLighting;
      return;
    }
    
    void setAnisoFiltering(bool newAniso)
    {
      aniso= newAniso;
      return;
    }
    
    void setFog(bool newFog)
    {
      fog= newFog;
      return;
    }
    
    void addNoGrassZone(aabbox3d<f32> newNoGrassZone)
    {
      noGrassZone.push_back(newNoGrassZone);
      return;
    }
    
    void addGrassPlaneTemplate(ITexture* newTexture, int newProbability= 100, float newYTranslation= 0.5, int newMinHeight= -99999, int newMaxHeight= 99999)
    {
      grassPlaneTemplate newGrassPlaneTemplate;
      newGrassPlaneTemplate.probability= newProbability;
      newGrassPlaneTemplate.minHeight= newMinHeight;
      newGrassPlaneTemplate.maxHeight= newMaxHeight;
      newGrassPlaneTemplate.YTranslation= newYTranslation;
      
      SMaterial* grassMaterial= new SMaterial;
      grassMaterial->BackfaceCulling= false;
      grassMaterial->MaterialType= EMT_TRANSPARENT_ALPHA_CHANNEL_REF ;//EMT_TRANSPARENT_ALPHA_CHANNEL
      grassMaterial->setTexture(0, newTexture);
      
      IMesh* grassMesh= geom->createPlaneMesh(dimension2d<f32>(planeSize* texturesPerPlane,planeSize), dimension2d<u32>(1,1), grassMaterial, dimension2d<f32>(texturesPerPlane,1));
      newGrassPlaneTemplate.mesh= smgr->addMeshSceneNode(grassMesh);
      newGrassPlaneTemplate.mesh->setPosition(vector3df(0,-99999,0));
      newGrassPlaneTemplate.mesh->getMesh()->setHardwareMappingHint(EHM_STATIC);
      newGrassPlaneTemplate.mesh->setName("plsDoNotDeleteMe");
      newGrassPlaneTemplate.mesh->setMaterialFlag(EMF_FOG_ENABLE, fog);
      newGrassPlaneTemplate.mesh->setMaterialFlag(EMF_LIGHTING, lighting);
      
      newGrassPlaneTemplate.mesh->setMaterialFlag(EMF_TRILINEAR_FILTER, aniso);
      newGrassPlaneTemplate.mesh->setMaterialFlag(EMF_ANISOTROPIC_FILTER, aniso);
      
      planeTemplate.push_back(newGrassPlaneTemplate);
      delete grassMaterial;
      return;
    }
    
    void makeReady()
    {
      aabbox3d<f32> terrainBox= terrain->getTransformedBoundingBox();
      float minX= terrainBox.MinEdge.X;
      float minZ= terrainBox.MinEdge.Z;
      srand(time(0));
      
      for(float i= minX; i< minX+ terrainBox.getExtent().X; i+= density)
      {
        for(float ii= minZ; ii< minZ+ terrainBox.getExtent().Z; ii+= density)
        {
          float height= terrain->getHeight(i, ii);
          if(height> -99990)
          {
            bool typingDone= false;
            bool canBePlanted= true;
            for(int iii= 0; iii< planeTemplate.size(); iii++)
            {
              if(!typingDone)
              {
                if(height> planeTemplate[iii].minHeight && height< planeTemplate[iii].maxHeight)
                {
                  if(rand()%100< planeTemplate[iii].probability)
                  {
                    for(int a= 0; a< noGrassZone.size(); a++)
                    {
                      if(noGrassZone[a].isPointInside(vector3df(i, height+planeTemplate[iii].YTranslation , ii)))
                        canBePlanted= false;
                        
                      //fading out of no grass zones
                      aabbox3d<f32>biggerBox= noGrassZone[a];
                      vector3df minEdge= biggerBox.MinEdge;
                      vector3df maxEdge= biggerBox.MaxEdge;
                      vector3df extent= biggerBox.getExtent();
                      extent*= noGrassZoneFadeOut;
                      
                      biggerBox.addInternalPoint(minEdge- extent);
                      biggerBox.addInternalPoint(maxEdge+ extent);
                      
                      if(biggerBox.isPointInside(vector3df(i, height+planeTemplate[iii].YTranslation , ii)) 
                         && rand()%100< 50)
                           canBePlanted= false;
                    }
                    
                    if(canBePlanted)
                    {
                      grassNodeType.push_back(iii);
                      grassNodePos.push_back(vector3df(i, height+planeTemplate[iii].YTranslation , ii));
                      grassNodes.push_back(0);
                      isGrassNodePosFree.push_back(true);
                    
                      typingDone= true;
                    }
                  } //please kill me already, this func is pure chaos
                    //also, I no more have idea what I did
                   //ok, maybe a bit
                }
              }
            }
            
          }
          
        }
      }
    }//this one's a killer func with all those {{{{{{{{{}}}}}}}}}'s 
    
    void doGrassStuff(vector3df center)
    {
      if(helperForDeleting==0)helperForDeleting= 1;
        else helperForDeleting= 0;
      
      for(int i= helperForDeleting; i< grassNodePos.size(); i+=2)
      {
        line3d<f32> ray;
        ray.start= center;
        ray.end= grassNodePos[i];
        if(ray.getLength()< distance && isGrassNodePosFree[i])
        {
          grassNodes[i]= planeTemplate[grassNodeType[i]].mesh->clone();
          grassNodes[i]->setPosition(grassNodePos[i]);
          grassNodes[i]->setRotation(vector3df(-90, rand()%360, 0));
          isGrassNodePosFree[i]= false;
          count++;
        }
        else if(ray.getLength()> distance && !isGrassNodePosFree[i])
        {
          grassNodes[i]->remove();
          isGrassNodePosFree[i]= true;
          count--;
        }
      }
             
      
      return;
    }
    
};
