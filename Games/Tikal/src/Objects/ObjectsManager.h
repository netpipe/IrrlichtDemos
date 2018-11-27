#include <irrlicht.h>
#include <vector>
#include <iostream>
#include "shadercb.h"
#include "TKTerrainSceneNode.h"
#include "TKOceanSceneNode.h"
#include "TKSkySceneNode.h"

#include "TKTreeSceneNode.h"
#include "TKGroundVegetationSceneNode.h"
#include "TKRockSceneNode.h"

#include "TKStoneResource.h"
#include "TKFoodResource.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

///TODO: colocar variaveis e metodos em ordem alfabetica, obedecer ordem: public, protected, private

#ifndef __OBJECTSMANAGER__
#define __OBJECTSMANAGER__

//___________________________________________________________________________________________ObjectsManager
class ObjectsManager
{
public:
    ///Construtor da classe
    ObjectsManager(IrrlichtDevice *idevice);

    ///Destrutor, cleanup
    ~ObjectsManager();///TODO: drop() em todas as texturas criadas, delete objetos

    ///Create Ground Vegetation
    TKGroundVegetationSceneNode* createGroundVegetation(ISceneNode* parent, vector3df pos);

    ///Create Rock
    TKRockSceneNode* createRock(ISceneNode* parent, vector3df pos);

    ///Criar um objeto do tipo Ocean
    TKOceanSceneNode* createOcean(stringc heightMapFile);

    ///Criar recurso Food
    TKFoodResource* createFoodResource(vector3df pos);

    ///Criar recurso Stone
    TKStoneResource* createStoneResource(vector3df pos);

    ///Criar skybox
    TKSkySceneNode* createSky();

    ///Criar um objeto do tipo Tree
    TKTreeSceneNode* createTree(ISceneNode* parent, vector3df pos);

    ///Criar um objeto do tipo Terrain
    TKTerrainSceneNode* createTerrain(stringc heightMapFile);

    ///Clear World! remove tudo da memoria
    void clearWorld();

    ///Carregar todos os recursos necessários
    void loadAllResources();

    ///Atualizar objetos dinamicos
    void update();

    ///Obtem o triagle selector da terrain
    ITriangleSelector* getTerrainTriangleSelector();///TODO: Metodo getTerrainTriangleSelector() por estar Obsoleto!


    E_MATERIAL_TYPE getTerrainMaterial();

    ITexture* getMiniMapTexture();

    vector<ITriangleSelector*> getTriangleSelectors();

    vector<vector3df> getResourcesPos();
    vector<stringc> getResourcesType();

private:
    ///device principal da Irrlicht
    IrrlichtDevice* device;

    ///SceneManger principal
    ISceneManager* smgr;

    ///Shader Callback
    MyShaderCallBack* mc;

    ///Materiais
    s32 materialTerrain;
    s32 materialOcean;

    ///Real Time Textures
    ITexture* ShadowMapTex;///TODO: remover isso se nao usar shadowmap
    ITexture* oceanReflectionTexture;
    ITexture* fowTexture;

    ///Vetores de objetos
    vector<TKTreeSceneNode*> vectorTree;
    vector<TKGroundVegetationSceneNode*> vectorGroundVegetation;
    vector<TKRockSceneNode*> vectorRock;
    vector<TKStoneResource*> vectorStoneResource;

    ///light, terrain e ocean terão apenas uma instancia.
    TKTerrainSceneNode* terrain;
    TKOceanSceneNode* ocean;
    ILightSceneNode* light;

    ///Camera para RTT   TODO: verificar se realmente é necessária
    ICameraSceneNode* RTTCamera;

    TKSkySceneNode* sky;

    ///Terrain Size
    static const int MAPSIZE=2;

    static const int DRAW_DISTANCE=50;

    ///MiniMap texture
    ITexture* miniMapTexture;

    ///triagleSelectors
    vector<ITriangleSelector*> selectors;

    vector<vector3df> foodResources;
    vector<vector3df> stoneResources;
    vector<vector3df> woodResources;
};

#endif
