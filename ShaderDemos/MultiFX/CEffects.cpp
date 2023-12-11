/*-----------------------------------------

Programmed by Fabre Cédric

(Copland/simson) 02/10/2006

Thanks to the Irrlicht team

for this good 3D engine !

This code has not been tested under Linux.

If you find any errors or corrections,

in advance thanks you for any feedbacks.

-------------------------------------------

Contact : cf46@coder3d.com

-----------------------------------------*/



#include <irrlicht.h>

#include "CEffects.h"



using namespace irr;



CQuadSceneNode::CQuadSceneNode(scene::ISceneNode* parent,scene::ISceneManager* mgr,s32 id): scene::ISceneNode(parent, mgr, id)

{

    Material.Lighting = false;
    Material.Wireframe = false;
    Material.ZBuffer = false;
    Material.ZWriteEnable = false;
    Material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
    Material.BackfaceCulling = false;
    Material.MaterialTypeParam = 0.01f;


    VertexQuad[0].TCoords.set(0.0f, 0.0f);
    VertexQuad[1].TCoords.set(1.0f, 0.0f);
    VertexQuad[2].TCoords.set(0.0f, 1.0f);
    VertexQuad[3].TCoords.set(1.0f, 1.0f);

    VertexQuad[0].Color = video::SColor(10,60,60,60);
    VertexQuad[1].Color = video::SColor(10,60,60,60);
    VertexQuad[2].Color = video::SColor(10,60,60,60);
    VertexQuad[3].Color = video::SColor(10,60,60,60);

    Indices[0] = 0;
    Indices[1] = 1;
    Indices[2] = 2;
    Indices[3] = 3;
    Indices[4] = 2;
    Indices[5] = 1;


   Box.reset(VertexQuad[0].Pos);



   for (s32 i=1; i<4; ++i)

   {

      Box.addInternalPoint(VertexQuad[i].Pos);

   }

   AutomaticCullingState = scene::EAC_OFF;

}


void CQuadSceneNode::OnRegisterSceneNode()

{

   if (IsVisible)

      SceneManager->registerNodeForRendering(this);

   ISceneNode::OnRegisterSceneNode();

}


void CQuadSceneNode::Update()
{
    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    scene::ICameraSceneNode *camera = SceneManager->getActiveCamera();

    f32 resultat = cos((camera->getFOV()/2));
    resultat = 10.001f/resultat;
    resultat = (resultat*resultat) - (10.001f*10.001f);
    resultat = sqrt(resultat);
    f32 ScaleY = resultat;
    f32 ScaleX = resultat * camera->getAspectRatio();

    core::vector3df Scale3D(ScaleX,ScaleY,0.0f);

    VertexQuad[0].Pos = Scale3D*core::vector3df(-1.0f,1.0f,0);//Haut Gauche
    VertexQuad[1].Pos = Scale3D*core::vector3df(1.0f,1.0f,0);//Haut Droite
    VertexQuad[2].Pos = Scale3D*core::vector3df(-1.0f,-1.0f,0);//Bas Gauche
    VertexQuad[3].Pos = Scale3D*core::vector3df(1.0f,-1.0f,0);//Bas Droite

    Box.reset(VertexQuad[0].Pos);
      for (s32 i=0; i<4; ++i)
         Box.addInternalPoint(VertexQuad[i].Pos);
}


void CQuadSceneNode::render()

{
    video::IVideoDriver* driver = SceneManager->getVideoDriver();

    scene::ICameraSceneNode *camera = SceneManager->getActiveCamera();

    core::matrix4 mv = camera->getViewMatrix();
    mv.makeInverse();

    core::matrix4 translation;
    translation.setTranslation(core::vector3df(0,0,10.001f+getScale().Z));
    mv*=translation;

    Update();

    driver->setTransform(video::ETS_WORLD, mv);

    driver->setMaterial(Material);
    driver->drawIndexedTriangleList(&VertexQuad[0], 4, &Indices[0], 2);

}



const core::aabbox3d<f32>& CQuadSceneNode::getBoundingBox() const

{

   return Box;

}


u32 CQuadSceneNode::getMaterialCount() const
{
    return 1;
}

video::SMaterial& CQuadSceneNode::getMaterial(u32 i)
{
    return Material;
}


//Méthode de création du Glow

void CEffects::CreateGlow(scene::ICameraSceneNode* CamGlow,scene::ISceneManager* smgr,video::IVideoDriver* driver,s32 GlowSize=64)

{

   //On vérifi que la carte vidéo soit cappable d'utiliser le Render To Texture

   if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET))

   {

      //Ici on creer notre Render Texture

      this->RTexGlow=driver->addRenderTargetTexture(core::dimension2d<u32>(GlowSize,GlowSize),"TT1");



      //Ici on va créer plusieurs quad pour simuler un effet de Blur

      for (int nbquad=0;nbquad<=3;nbquad++)

      {

         this->QuadGlow[nbquad] = new CQuadSceneNode(smgr->getRootSceneNode(), smgr, -1);

         QuadGlow[nbquad]->setScale(irr::core::vector3df(0,0,-nbquad/10.0f));

         this->QuadGlow[nbquad]->setMaterialTexture(0,RTexGlow);

         this->QuadGlow[nbquad]->drop();

      }

   }

}



//Méthode de rendu du Glow

void CEffects::RenderGlow(scene::ISceneManager* smgr,video::IVideoDriver* driver,s32 gA=0,s32 gR=0,s32 gG=0,s32 gB=0)

{

   //On test si le Render Texture existe bien

   if (this->RTexGlow != NULL)

   {

      for (int i=0;i<=3;i++)

      {

         if (this->QuadGlow[i] != NULL)

         {
                this->QuadGlow[i]->setVisible(false);

         }

      }



      //On affiche notre scene

      driver->setRenderTarget(this->RTexGlow, true, true, video::SColor(gA,gR,gG,gB));


      smgr->drawAll();


      driver->setRenderTarget(0);

      for (int i=0;i<=3;i++)
      {
         if (this->QuadGlow[i] != NULL)
         {
                this->QuadGlow[i]->setVisible(true);
         }
      }

   }

}



//Méthode de suppression du Glow

void CEffects::DeleteGlow()

{

   if (RTexGlow){RTexGlow->drop();}

   for (int i=0;i<=3;i++)

   {

      if (QuadGlow[i] != NULL){QuadGlow[i]->remove();}

   }

}



//Méthode de création du filtre Dream
void CEffects::CreateDream(scene::ICameraSceneNode* CamDream,scene::ISceneManager* smgr,video::IVideoDriver* driver,s32 DreamSize=64)
{
   //On vérifi que la carte vidéo soit cappable d'utiliser le Render To Texture
   if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET))
   {
      //Ici on creer notre Render Texture
      this->RTexDream=driver->addRenderTargetTexture(core::dimension2d<u32>(DreamSize,DreamSize),"TT2");

      //Ici on va créer plusieurs quad pour simuler un effet de Blur
      for (int nbquad=0;nbquad<=6;nbquad++)
      {
         this->QuadDream[nbquad] = new CQuadSceneNode(smgr->getRootSceneNode(), smgr, -1);
         QuadDream[nbquad]->setScale(irr::core::vector3df(0,0,-nbquad/1.5f));
         this->QuadDream[nbquad]->setMaterialTexture(0,RTexDream);
         this->QuadDream[nbquad]->drop();
      }
   }
}

//Méthode de rendu du filtre Dream
void CEffects::RenderDream(scene::ISceneManager* smgr,video::IVideoDriver* driver,s32 gA=0,s32 gR=0,s32 gG=0,s32 gB=0)
{
   //On test si le Render Texture existe bien
   if (this->RTexDream != NULL)
   {
      for (int i=0;i<=6;i++)
      {
         if (this->QuadDream[i] != NULL)
         {
                this->QuadDream[i]->setVisible(false);
         }
      }

      //On affiche notre scene
      driver->setRenderTarget(this->RTexDream, true, true, video::SColor(gA,gR,gG,gB));

      smgr->drawAll();

      driver->setRenderTarget(0);

      for (int i=0;i<=6;i++)
      {
         if (this->QuadDream[i] != NULL)
         {
                this->QuadDream[i]->setVisible(true);
         }
      }
   }
}


//Méthode de suppression du filtre Dream
void CEffects::DeleteDream()
{
   if (RTexDream){RTexDream->drop();}
   for (int i=0;i<=6;i++)
   {
      if (QuadDream[i] != NULL){QuadDream[i]->remove();}
   }
}

void CEffects::CreateVision(scene::ICameraSceneNode* CamVision,scene::ISceneManager* smgr,video::IVideoDriver* driver,irr::c8* NoiseFile,s32 VisionSize)
{
    irr::video::ITexture* NoiseTexture = driver->getTexture(NoiseFile);

   //On vérifi que la carte vidéo soit cappable d'utiliser le Render To Texture
   if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET))
   {
      //Ici on creer notre Render Texture
      this->RTexVision=driver->addRenderTargetTexture(core::dimension2d<u32>(VisionSize,VisionSize),"TT3");

      //Ici on va créer plusieurs quad pour simuler un effet de Blur
      for (int nbquad=0;nbquad<=1;nbquad++)
      {
         this->QuadVision[nbquad] = new CQuadSceneNode(smgr->getRootSceneNode(), smgr, -1);

            QuadVision[nbquad]->getMaterial(0).DiffuseColor = video::SColor(10,10,120,10);
         QuadVision[nbquad]->getMaterial(0).EmissiveColor = video::SColor(10,10,120,10);
         QuadVision[nbquad]->getMaterial(0).AmbientColor = video::SColor(10,10,120,10);
         QuadVision[nbquad]->getMaterial(0).SpecularColor = video::SColor(10,10,120,10);
            QuadVision[nbquad]->getMaterial(0).Lighting=true;

            if (nbquad < 1)
            {
                this->QuadVision[nbquad]->setMaterialTexture(0,RTexVision);
            }
            else
            {
                QuadVision[nbquad]->setMaterialTexture(0,NoiseTexture);
            }

            QuadVision[nbquad]->setScale(irr::core::vector3df(0,0,-0.1f));

         this->QuadVision[nbquad]->drop();
      }
   }
}

void CEffects::RenderVision(scene::ISceneManager* smgr,video::IVideoDriver* driver,s32 gA,s32 gR,s32 gG,s32 gB)
{
    //On test si le Render Texture existe bien
   if (this->RTexVision != NULL)
   {
      for (int i=0;i<=1;i++)
      {
         if (this->QuadVision[i] != NULL)
         {
                this->QuadVision[i]->setVisible(false);
         }
      }

      //On affiche notre scene
      driver->setRenderTarget(this->RTexVision, true, true, video::SColor(gA,gR,gG,gB));

      smgr->drawAll();

      driver->setRenderTarget(0);

        static int o=0;
        o++;
        if (o>=3)
        {
            o=0;
        }
        QuadVision[1]->setScale(irr::core::vector3df(0,0,-o/10.0f));

      for (int i=0;i<=1;i++)
      {
         if (this->QuadVision[i] != NULL)
         {
                this->QuadVision[i]->setVisible(true);
         }
      }
   }
}

void CEffects::DeleteVision()
{
    if (RTexVision){RTexVision->drop();}
   for (int i=0;i<=1;i++)
   {
      if (QuadVision[i] != NULL){QuadVision[i]->remove();}
   }
}

void CEffects::RenderNothing()
{
    for (int i=0;i<=3;i++)
    {
        if (this->QuadGlow[i] != NULL)
        {
            this->QuadGlow[i]->setVisible(false);
        }
    }

    for (int i=0;i<=6;i++)
    {
        if (this->QuadDream[i] != NULL)
        {
            this->QuadDream[i]->setVisible(false);
        }
    }

    for (int i=0;i<=1;i++)
    {
        if (this->QuadVision[i] != NULL)
        {
            this->QuadVision[i]->setVisible(false);
        }
    }
}
